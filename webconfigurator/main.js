// CONSTANTS
const TIMEOUT_NO_MORE_DATA_EXPECTED = 50; // millis
const TIMEINTERVAL_SPINNER = 100; // millis
const TIMEINTERVAL_PORTS = 10000; // millis
const TIMEINTERVAL_WAIT_FOR_DOWNLOAD = 2000; // millis
const MAX_NUM_SLOTS = 10;
const BAUD_RATE = 9600;
let openPortCheckTimer = null;
let spinnerCheckTimer = null;
let prepareDownloadTimer = null;
let spinnerShouldShow = false;
let gettingData = false;
let gettingSlot = 0;
let writingData = false;
let writingSlot = 0;
let port = null;
let ports = null; // LIST OF PORTS WE HAVE PERMISSION TO
let reader = null;
let readableStreamClosed = null;
let readerAborter = null;
let writer = null;
let messageBuffer = "";
let timeoutId = null;
let defaultDeviceConfigurationSet = null;
let shouldPrepareDownloadAfterChange = false;
let deviceInfo = { boardId: "", build: "", firmware: "" };
const slotsContainer = document.getElementById("slots");

/********************************
 * PLANNED FEATURES / FUTURE
 *********************************/
/*

- export setup as JSON [DONE]
- save a setup of all slots under a NAME in localStorage [CANCELLED]
- save a setup of all slots as a local file which can be shared easily [DONE]
- load/import a setup to send it to device [DONE]
- import setup from JSON/file etc. [DONE]
- better unified window overlay [DONE]
- simulated effects-preview for the slots configured

*/

/********************************
 * CONFIGURATION DATA MODEL CLASSES
 *********************************/

// CARRIES META DATA ABOUT FILE FORMAT VERSION
class DeviceSetMeta {
  // METADATA FOR FILE FORMAT
  static VERSION = "2.0";
  FILE_VERSION = DeviceSetMeta.VERSION;
  AUTHOR = "blazr";
  DEVICE = "HSHB LED Tag";
  DATECREATED = new Date();
  DATECHANGED = null;

  constructor(FILE_VERSION, AUTHOR, DEVICE, DATECREATED, DATECHANGED) {
    this.FILE_VERSION = FILE_VERSION;
    this.AUTHOR = AUTHOR;
    this.DEVICE = DEVICE;
    this.DATECREATED = DATECREATED;
    this.DATECHANGED = DATECHANGED;
  }
}

// CLASS DEFINING AN EFFECT SETUP
class DeviceSet {
  META = null;

  constructor(displayName, uniqueIdentifier) {
    this.displayName = displayName;
    if (uniqueIdentifier == null) {
      this.uniqueIdentifier = self.crypto.randomUUID();
    } else {
      this.uniqueIdentifier = uniqueIdentifier;
    }
    this.slots = [];
    this.META = new DeviceSetMeta();
  }

  json() {
    return JSON.stringify(this);
  }

  getSlots() {
    return this.slots;
  }

  numberOfSlots() {
    return this.slots.length;
  }

  addSlot(slotToAdd) {
    this.slots.push(slotToAdd);
  }

  slotWithIndex(index) {
    return this.slots[index];
  }
}

// CLASS FUNCTION TO DESERIALIZE A NEW SETUP FROm JSON
DeviceSet.fromJSON = function (json) {
  let deviceSet = new DeviceSet(json.displayName, json.uniqueIdentifier);
  deviceSet.META = new DeviceSetMeta(
    json.META.FILE_VERSION,
    json.META.AUTHOR,
    json.META.DEVICE,
    json.META.DATECREATED,
    json.META.DATECHANGED,
  );
  json.slots.forEach((slot, i) => {
    // console.warn( ""+slot );
    let slotToAdd = DeviceSlot.fromJSON(slot);
    // slotToAdd.debug();
    deviceSet.addSlot(slotToAdd);
  });
  return deviceSet;
};

// CLASS DEFINING ONE SLOT
class DeviceSlot {
  // CLASS CONSTANTS DEFINING VALUE LIMITS
  static MAX_NUM_TEXT_TYPE_IDS = 3;
  static MAX_NUM_ANIMATION_IDS = 12;
  static MAX_TEXT_LENGTH = 75;
  static MIN_OFFSET_X = 0;
  static MAX_OFFSET_X = 255;
  static MIN_CHAR_SPACE = 0;
  static MAX_CHAR_SPACE = 60;
  static MIN_CHAR_SCALE = 1;
  static MAX_CHAR_SCALE = 5;
  static MIN_DURATION = 1;
  static MAX_DURATION = 60;
  static TITLE_TYPE =
    "Choose if text should be displayed as fixed text or as scrolling text.";
  static TITLE_TEXT =
    "Enter the text which should be statically displayed or scrolled. Short text should be static, long texts should be scrolled.";
  static TITLE_OFFSET_X =
    "Offset defines the amount of pixels (LEDs) from the left edge to start placing fixed text. Scroll Interval defines a delay for scrolling speed, 0 means fastest speed.";
  static TITLE_CHAR_SPACE =
    "The amount of pixels (LEDs) between each text character displayed.";
  static TITLE_CHAR_SCALE =
    "The multiplier for the width of a character. 2 means double the width for each character.";
  static TITLE_DURATION =
    "Duration in seconds a static text should be displayed. Scroll cycles defines the amount of iterations the text gets scrolled through.";
  static TITLE_ANIMATION =
    "The animation effect which will be shown on the upper logo part of the badge.";

  constructor(
    slotId,
    textTypeId,
    xOffset,
    charSpace,
    charScale,
    text,
    duration,
    animationId,
    active,
  ) {
    this.slotId = slotId;
    this.textTypeId = textTypeId;
    this.xOffset = xOffset;
    this.charSpace = charSpace;
    this.charScale = charScale;
    this.text = text;
    this.duration = duration;
    this.animationId = animationId;
    this.active = active;
  }

  debug() {
    console.log("Slot " + this.slotId + ":\n" + JSON.stringify(this) + "\n\n");
  }

  displayName() {
    return "Slot " + this.slotId;
  }
  selectedTextTypeForId(optionId) {
    if (this.textTypeId == optionId) {
      return "selected";
    } else {
      return "";
    }
  }

  selectedAnimationForId(animationId) {
    if (this.animationId == animationId) {
      return "selected";
    } else {
      return "";
    }
  }

  htmlDiv() {
    let i = this.slotId;
    const slotDiv = document.createElement("div");
    slotDiv.className = "slot";
    let html = "";
    html += `<h3 style="display:inline" id="slot-name-${i}" class=""><input type="checkbox" id="active-${i}" onClick="setSlotVisibility(${i})">&nbsp;Slot ${i}</h3><button id="send-button-${i}" onclick="writeSlot(${i})" class="writeslotred">Clear slot...</button> <span id="writeConfirm-${i}" class="writeConfirm hidden">✅</span>`;
    html += `<div id="slot-content-${i}" style="display:none">`;
    html += `<fieldset><legend>Movement, spacing & character width</legend>`;
    html += `<label for="textType-${i}" title="${DeviceSlot.TITLE_TYPE}">Type</label>`;
    html += `<select id="textType-${i}" onChange="setTextOptionsForSlot(${i});dirty();">`;
    html += `<option id="textTypeOption-${i}-0" value="0">None</option>`;
    html += `<option id="textTypeOption-${i}-1" value="1">Static</option>`;
    html += `<option id="textTypeOption-${i}-2" value="2">Scrolling</option>`;
    html += `</select>`;
    html += `&nbsp;<label id="offset-interval-label-${i}" for="offset-interval-${i}" title="${DeviceSlot.TITLE_OFFSET_X}">X-Offset</label>`;
    html += `<input type="number" class="numInput" id="offset-interval-${i}" min="${DeviceSlot.MIN_OFFSET_X}" max="${DeviceSlot.MAX_OFFSET_X}" value="${this.xOffset}" onchange="dirty();"> `;
    html += `&nbsp;<label for="char-space-${i}" title="${DeviceSlot.TITLE_CHAR_SPACE}">Char Space</label>`;
    html += `<input type="number" class="numInput" id="char-space-${i}" min="${DeviceSlot.MIN_CHAR_SPACE}" max="${DeviceSlot.MAX_CHAR_SPACE}" value="${this.charSpace}" onchange="dirty();"> `;
    html += `&nbsp;<label for="char-scaler-${i}" title="${DeviceSlot.TITLE_CHAR_SCALE}">Char Scaler</label>`;
    html += `<input type="number" class="numInput" id="char-scaler-${i}" min="${DeviceSlot.MIN_CHAR_SCALE}" max="${DeviceSlot.MAX_CHAR_SCALE}" value="${this.charScale}" onchange="dirty();">`;
    html += `</fieldset>`;
    html += `<div id="text-options-${i}">`;
    html += `<fieldset><legend id="scrolltextinfo-${i}" title="${DeviceSlot.TITLE_TEXT}">Text for display (can be up to ${DeviceSlot.MAX_TEXT_LENGTH} chars...)</legend>`;
    html += `<input type="text" id="text-${i}" size="${DeviceSlot.MAX_TEXT_LENGTH}" maxlength="${DeviceSlot.MAX_TEXT_LENGTH}" value="${this.text}" placeholder="Text to display goes here..." oninput="dirty();showRemainingChars(${i});">`;
    html += `</fieldset>`;
    html += `</div>`;
    html += `<fieldset><legend>Animation options</legend>`;
    html += `<label id="duration-label-${i}" for="duration-${i}" title="${DeviceSlot.TITLE_DURATION}">Duration seconds</label>`;
    html += `<input type="number" class="numInput" id="duration-${i}" min="${DeviceSlot.MIN_DURATION}" max="${DeviceSlot.MAX_DURATION}" value="${this.duration}" onchange="dirty();"> `;
    html += `&nbsp;<label for="animation-${i}" title="${DeviceSlot.TITLE_ANIMATION}">Animation</label>`;
    html += `<select id="animation-${i}" onchange="dirty();">`;
    html += `<option id="animationOption-${i}-0" value="0">Static Off</option>`;
    html += `<option id="animationOption-${i}-1" value="1">Static On</option>`;
    html += `<option id="animationOption-${i}-2" value="2">Matrix</option>`;
    html += `<option id="animationOption-${i}-3" value="3">Sweep</option>`;
    html += `<option id="animationOption-${i}-4" value="4">Wave</option>`;
    html += `<option id="animationOption-${i}-5" value="5">Lines</option>`;
    html += `<option id="animationOption-${i}-6" value="6">Lines Fill</option>`;
    html += `<option id="animationOption-${i}-7" value="7">Rotate</option>`;
    html += `<option id="animationOption-${i}-8" value="8">Rotate Fill</option>`;
    html += `<option id="animationOption-${i}-9" value="9">Circles</option>`;
    html += `<option id="animationOption-${i}-10" value="10">Sparkles</option>`;
    html += `<option id="animationOption-${i}-11" value="11">Stripes</option>`;
    html += `</select></fieldset>`;
    html += `</div>`;
    slotDiv.innerHTML = html;
    return slotDiv;
  }
}

// CLASS FUNCTION TO DESERIALIZE A NEW SLOT FROM JSON
DeviceSlot.fromJSON = function (json) {
  return new DeviceSlot(
    json.slotId,
    json.textTypeId,
    json.xOffset,
    json.charSpace,
    json.charScale,
    json.text,
    json.duration,
    json.animationId,
    json.active,
  );
};

/********************************
 * CONFIGURATION SETUP HANDLING
 *********************************/

// GET THE CURRENT ACTIVE DEVICE CONFIG
function defaultSet() {
  if (defaultDeviceConfigurationSet == null) {
    defaultDeviceConfigurationSet = new DeviceSet(
      "Default Configuration",
      null,
    );
    for (let i = 0; i < MAX_NUM_SLOTS; i++) {
      let currentSlot = new DeviceSlot(i, 1, 0, 2, 1, "", 1, 1, false);
      defaultDeviceConfigurationSet.addSlot(currentSlot);
    }
  }
  return defaultDeviceConfigurationSet;
}

function editConfigName() {
  let placeholder = defaultSet().displayName;
  let configName = prompt(
    "Please enter a name for your configuration set",
    placeholder,
  );
  if (configName != null) {
    defaultSet().displayName = configName;
    hasTouchedOrChangedTheConfigurationData();
    document.getElementById("setselector").textContent =
      defaultSet().displayName;
  } else {
    // do nothing
  }
}

// MARKS CONFIGURATION AS DIRTY, WE NED TO PREPARE NEW CONFIG DOWNLOAD
function hasTouchedOrChangedTheConfigurationData() {
  if (prepareDownloadTimer) {
    clearTimeout(prepareDownloadTimer);
  }
  prepareDownloadTimer = setTimeout(
    configurationSynchronize,
    TIMEINTERVAL_WAIT_FOR_DOWNLOAD,
  );
  toggleClassForElementWithIdTo("download", "downloadhidden");
  toggleClassForElementWithIdTo("share", "sharehidden");
}

// SHORTER NAME FOR HANDLERS
function dirty() {
  hasTouchedOrChangedTheConfigurationData();
}

function escapeHtml(unsafe) {
  return unsafe
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;")
    .replace(/'/g, "&#039;");
}

function showRemainingChars(slotId) {
  let textField = document.getElementById("text-" + slotId);
  let textInfo = document.getElementById("scrolltextinfo-" + slotId);
  let charsRemaining = DeviceSlot.MAX_TEXT_LENGTH - textField.value.length;
  textInfo.textContent =
    "Text for display (can be up to " +
    DeviceSlot.MAX_TEXT_LENGTH +
    " chars... " +
    charsRemaining +
    " remaining)";
}

// TRANSFORM DATA INTO DATA-URL FOR DOWNLOAD FROM CURRENT CONFIG SET
function prepareDownload() {
  const nowDate = new Date();
  const nowTime = nowDate.getTime();
  const formattedDate = `${nowDate.getDate()}-${
    nowDate.getMonth() + 1
  }-${nowDate.getFullYear()}-${nowDate.getHours()}-${nowDate.getMinutes()}`;
  const downloadButton = document.getElementById("download");
  defaultSet().META.DATECHANGED = nowDate;
  let dataToDownload = defaultSet().json();
  downloadButton.href =
    "data:application/octet-stream;base64," + btoa(dataToDownload);
  downloadButton.download = "hshb-tag-configuration-" + formattedDate + ".json";
  toggleClassForElementWithIdTo("download", "download");
  const shareButton = document.getElementById("share");
  let embedShareText =
    '<a href="data:application/octet-stream;base64,' +
    btoa(dataToDownload) +
    '" download="' +
    "shared-configuration-" +
    formattedDate +
    ".json" +
    '">' +
    defaultSet().displayName +
    "</a>";
  shareButton.value = embedShareText;
  toggleClassForElementWithIdTo("share", "share");
}

// SHOW WINDOW WHERE EMBED CODE IS SHOWN
function showEmbedShare() {
  const shareButton = document.getElementById("share");
  windowShowWithTypeAndTitleAndInnerHtml(
    "SHARE",
    "share",
    "Copied to clipboard.",
    "<code style='word-break:break-all;'>" +
      escapeHtml(shareButton.value) +
      "</code>",
  );
  navigator.clipboard.writeText(shareButton.value);
}

// SHOW UPLOAD/IMPORT WINDOW
function prepareUpload() {
  let htmlInjection = `<form><input type="file" id="fileinput" /><div id="uploadStatus" style="margin-top:20px;margin-bottom:20px;">&nbsp;</div></form>`;
  windowShowWithTypeAndTitleAndInnerHtml(
    "IMPORT",
    "info",
    "Please choose one .json configuration file stored to be imported.",
    htmlInjection,
  );
  let inputElement = document.getElementById("fileinput");
  inputElement.addEventListener("change", checkFiles, false);
}

// EXECUTED TO IMPORT A CONFIGURATION AS JSON
async function checkFiles() {
  let fileList = this.files;
  if (fileList == null || fileList == undefined) return;
  // console.log( "FILE: GOT LIST..." );
  let numFiles = fileList.length;
  // console.log( "FILE: "+numFiles+" FILES" );
  let selectedFile = this.files[0];
  // console.log( "FILE: "+selectedFile );
  console.log("FILE NAME: " + selectedFile.name);
  console.log("FILE SIZE: " + selectedFile.size);
  console.log("FILE TYPE: " + selectedFile.type);
  if (selectedFile.type == "application/json") {
    // PROCESS FILE
    // Read the file
    const reader = new FileReader();
    reader.onload = () => {
      let importedSet = null;
      try {
        importedJson = JSON.parse(reader.result);
        importedSet = DeviceSet.fromJSON(importedJson);
      } catch (error) {
        let detectedFileVersion = null;
        try {
          if ("META" in importedJson) {
            if ("FILE_VERSION" in importedJson.META) {
              detectedFileVersion = importedJson.META.FILE_VERSION;
            }
          }
        } catch (error) {
          // do nothing
        }
        if (detectedFileVersion != null) {
          document.getElementById("uploadStatus").textContent =
            "ERROR: Configuration file not readable.<br>Detected file format uses version " +
            detectedFileVersion +
            ", but this configurator is already on version " +
            DeviceSetMeta.VERSION +
            ". Maybe the format is incompatible.";
        } else {
          document.getElementById("uploadStatus").innerHTML =
            "ERROR: Configuration file not readable/parsable.<br><code>" +
            error +
            "</code>";
        }
        console.error(error);
      }
      if (importedSet != null) {
        document.getElementById("uploadStatus").innerHTML =
          "Imported successfully.";
        let inputElement = document.getElementById("fileinput");
        inputElement.value = null;
        defaultDeviceConfigurationSet = importedSet;
        document.getElementById("window").style.display = "none";
        loadDeviceSetIntoUI(defaultSet());
        prepareDownload();
      } else {
        document.getElementById("uploadStatus").innerHTML =
          "Data was empty. Nothing imported!";
      }
    };
    reader.onerror = () => {
      document.getElementById("uploadStatus").textContent =
        "Error reading the file. Please try again.";
    };
    reader.readAsText(selectedFile);
  } else {
    document.getElementById("uploadStatus").textContent =
      "Wrong file format! Please upload .json files only.";
  }
}

// APPLIES AN IMPORTED SETUP TO THE USER INTERFACE
function loadDeviceSetIntoUI(deviceconfiguration) {
  document.getElementById("setselector").textContent =
    deviceconfiguration.displayName;
  // ITERATE OVER ALL SLOTS AND APPLY VALUES TO UI
  slotsContainer.innerHTML =
    "<h2 class='blink'>Importing, please wait ... </h2>";
  spinnerShouldShow = true;
  setTimeout(() => {
    slotsContainer.innerHTML = "";
    injectHtmlForSlotsToDOM();
    // ADJUST VISIBILITY OF SLOTS
    for (let i = 0; i < defaultSet().slots.length; i++) {
      let slot = defaultSet().slotWithIndex(i);
      document.getElementById("active-" + i).checked = slot.active;
      setSlotVisibility(i);
      // SET SELECT-OPTION_VALUES
      document.getElementById(`textType-${i}`).value = slot.textTypeId;
      document.getElementById(`animation-${i}`).value = slot.animationId;
      setTextOptionsForSlot(i);
      showRemainingChars(i);
      spinnerShouldShow = false;
    }
  }, "3000");
}

// PULLS VALUES OUT OF SLOT FORMFIELDS TO STORE IN CONFIGURATION MODEL (CALLED AFTER ALL CHANGES TO SETUP)
async function configurationSynchronize() {
  let slotsToSync = defaultSet().getSlots();
  for (let i = 0; i < slotsToSync.length; i++) {
    let slot = slotsToSync[i];
    slot.textTypeId = Number(document.getElementById(`textType-${i}`).value);
    slot.animationId = Number(document.getElementById(`animation-${i}`).value);

    slot.text = document.getElementById("text-" + i).value;
    slot.duration = Number(document.getElementById("duration-" + i).value);
    slot.xOffset = Number(
      document.getElementById("offset-interval-" + i).value,
    );
    slot.charScale = Number(document.getElementById("char-scaler-" + i).value);
    slot.charSpace = Number(document.getElementById("char-space-" + i).value);
    slot.active = document.getElementById("active-" + i).checked;
    // slot.debug();
  }
  // UPDATE THE CONFIG DATA TO BE READY FOR DOWNLOAD
  prepareDownload();
}

/********************************
 * CREATE AND INJECT SOME HTML FOR PAGE
 *********************************/

// BUILD DEFAULT CONFIG SET
defaultSet();

// BUILD DOM
function injectHtmlForSlotsToDOM() {
  document.getElementById("setselector").innerHTML =
    defaultSet().displayName + " (" + defaultSet().numberOfSlots() + " slots)";
  for (let i = 0; i < MAX_NUM_SLOTS; i++) {
    let currentSlot = defaultSet().slotWithIndex(i);
    slotsContainer.appendChild(currentSlot.htmlDiv());
  }
}

// DERIVE THE VERION OF WEBCONFIGURATOR FROM DeviceSetMeta-CLASS PROPERTY
document.getElementById("version").innerHTML =
  "Version " + DeviceSetMeta.VERSION;

/********************************
 * GENERAL HELPER FUNCTIONS
 *********************************/

// SCROLL SMOTHLY TO An ELEMENT
function scrollToElementWithId(element_id) {
  try {
    let targetElement = document.getElementById(element_id);
    scrollOptions = { behavior: "smooth", block: "center", inline: "nearest" };
    targetElement.scrollIntoView(scrollOptions);
  } catch (error) {
    // do nothing
  }
}

// SHOW A MESSAGE IN A WINDOW WITH A CERTAIN STYLE AND CONTENT
function windowShowWithTypeAndTitleAndInnerHtml(
  title,
  type,
  footer,
  html_content,
) {
  document.getElementById("windowtitle").innerHTML = title;
  document.getElementById("windowfooter").innerHTML = footer;
  document.getElementById("windowcontent").innerHTML = html_content;
  if (type == "error") {
    document.getElementById("windowcontent").style.backgroundColor = "#990225";
    document.getElementById("window").style.backgroundColor = "#cb0141";
  }
  if (type == "info") {
    document.getElementById("windowcontent").style.backgroundColor = "#017dcb";
    document.getElementById("window").style.backgroundColor = "#1f97e1";
  }
  if (type == "share") {
    document.getElementById("windowcontent").style.backgroundColor = "#8a007c";
    document.getElementById("window").style.backgroundColor = "#b1009f";
  }
  document.getElementById("window").style.display = "block";
}

// DISMISS ANY CUSTOM WINDOW
function windowHide(element_id) {
  document.getElementById(element_id).style.display = "none";
}

// STORE A VALUE INTO PERSISTENT LOCAL STORE (COOKIES DON'T WORK IN LOCAL MODE)
function setLocalStore(cname, cvalue, shouldEncodeBase64) {
  // console.log( "LOCAL STORE: WRITING ... NAME: "+cname+"  VALUE: "+cvalue );
  let encodedValue = cvalue;
  if (shouldEncodeBase64) {
    encodedValue = btoa(cvalue);
    // console.log( "LOCAL STORE: WROTE (BASE64) ... "+encodedValue );
  } else {
    // console.log( "LOCAL STORE: WROTE ... "+encodedValue );
  }
  localStorage.setItem(cname, encodedValue);
}

// RETRIVE A VALUE FROM PERSISTENT LOCAL STORE
function getLocalStore(cname, shouldDecodeBase64) {
  // console.log( "LOCAL STORE: GETTING ... "+cname );
  let extractedBase64Value = localStorage.getItem(cname);
  if (extractedBase64Value == null || extractedBase64Value == undefined) {
    // console.log( "LOCAL STORE: NOT FOUND '"+cname+"'");
    return undefined;
  }
  if (shouldDecodeBase64) {
    // console.log( "LOCAL STORE: READ (BASE64) ... "+extractedBase64Value );
    return atob(extractedBase64Value);
  } else {
    // console.log( "LOCAL STORE: READ ... "+extractedBase64Value );
    return extractedBase64Value;
  }
}

// CALL ANY FUNCTION WITH A HARD TINMEOUT LIMIT
function withTimeout(promise, timeout) {
  return Promise.race([
    promise,
    new Promise((_, reject) =>
      setTimeout(() => reject(new Error("Timeout")), timeout),
    ),
  ]);
}

// SET CSS-STYLE-CLASS FOR AN ELEMENT WITH ID
function toggleClassForElementWithIdTo(element_id, className) {
  e = document.getElementById(element_id);
  if (e) {
    // console.log( "ELEMENT: "+element_id+" CLASS: "+className );
    let classesToRemove = e.classList;
    if (classesToRemove != null && classesToRemove.length > 0) {
      e.classList.remove(classesToRemove);
    }
    if (className != null && className.length > 0) {
      e.classList.add(className);
    }
  } else {
    console.log("ERROR: ELEMENT FOR ID " + element_id + " NOT FOUND.");
  }
}

// UPDATES THE ACTIVITY SPINNER ACCORING TO CURRENT STATE (GETS CALLED VERY OFTEN)
function spinnerCheck() {
  if (spinnerShouldShow) {
    if ("spinnershow" == document.getElementById("spinner").classList) {
      //console.log( "NOTHING TO DO SHOW" );
    } else {
      toggleClassForElementWithIdTo("spinner", "spinnershow");
    }
  } else {
    if ("spinnerhide" == document.getElementById("spinner").classList) {
      //console.log( "NOTHING TO DO HIDE" );
    } else {
      toggleClassForElementWithIdTo("spinner", "spinnerhide");
    }
  }
}

/********************************
 * HTML RELATED ACTIONS TRIGGERED VIA THE UI
 *********************************/

// USER WANTS ALL SLOTS TO BE WRITTEN TO DEVICE
function askWriteAllSlots() {
  if (port == null) return;
  if (
    confirm(
      "Really write all slots now?\n\nWARNING: Inactive slots will be cleared!",
    )
  ) {
    writeAllSlots();
  }
}

// CONFIGURING WRITE PROCESS TO WRITE ALL SLOTS
function writeAllSlots() {
  if (port == null) return;
  spinnerShouldShow = true;
  console.log("SENDING ALL SLOTS...");
  writingData = true;
  writingSlot = 0;
  writeSlot(writingSlot);
}

// WRITING ONE SLOT AND CONTINUING TO WRITE MORE SLOTS IF CONFIGURED
function writeSlot(slt) {
  if (port == null) return;
  spinnerShouldShow = true;
  if (document.getElementById("active-" + slt).checked) {
    // WRITE THIS SLOT
    cmd = "W " + slt + " ";
    cmd += document.getElementById("animation-" + slt).value + " ";
    cmd += document.getElementById("duration-" + slt).value + " ";
    cmd += document.getElementById("textType-" + slt).value + " ";
    cmd += document.getElementById("offset-interval-" + slt).value + " ";
    cmd += document.getElementById("char-scaler-" + slt).value + " ";
    cmd += document.getElementById("char-space-" + slt).value + " ";
    cmd += document.getElementById("text-" + slt).value;
    sendUART(cmd + "\n");
  } else {
    // DELETE SLOT INSTEAD, BECAUSE NOt CHECKED
    sendUART("D " + slt + "\n");
  }
  if (writingData && writingSlot < MAX_NUM_SLOTS - 1) {
    spinnerShouldShow = true;
    writingSlot++;
    // launch delayed next write
    setTimeout(() => {
      toggleClassForElementWithIdTo(`slot-name-${writingSlot}`, "blink");
      writeSlot(writingSlot);
    }, "2000");
  } else {
    // STOP WRITINg SLOTS WHEN LAST SLOT WAS WRITTEN
    writingData = false;
    spinnerShouldShow = false;
  }
  writeConfirmation(slt);
}

// SHOW AND HIDE SMALL GREEN CHECKMARK
function writeConfirmation(slt) {
  if (writingData) {
    scrollToElementWithId(`slot-name-${slt + 1}`);
  }

  document.getElementById("writeConfirm-" + slt).classList.remove("hidden");
  setTimeout(() => {
    document.getElementById("writeConfirm-" + slt).classList.add("hidden");
    toggleClassForElementWithIdTo(`slot-name-${slt}`, "");
    if (writingData == false) {
      scrollToElementWithId("spinner");
    }
  }, "1000");
}

// ** NOT USED **
function deleteSlot(index) {
  document.getElementById(`text-${index}`).value = "";
  document.getElementById(`animation-${index}`).value = "none";
  document.getElementById(`textType-${index}`).checked = false;
  document.getElementById(`offset-interval-${index}`).value = "";
  document.getElementById(`duration-${index}`).value = "";
  document.getElementById(`x-offset-${index}`).value = "";
  document.getElementById(`char-scaler-${index}`).value = "";
  document.getElementById(`active-${index}`).checked = false;
  hasTouchedOrChangedTheConfigurationData();
}

// ADJUST HTML VISIBILITY FOR SLOT
function setSlotVisibility(slt) {
  if (document.getElementById("active-" + slt).checked) {
    document.getElementById("slot-content-" + slt).style = "";
    toggleClassForElementWithIdTo("send-button-" + slt, "writeslot");
    document.getElementById("send-button-" + slt).innerHTML = "Send slot...";
  } else {
    document.getElementById("slot-content-" + slt).style = "display:none";
    toggleClassForElementWithIdTo("send-button-" + slt, "writeslotred");
    document.getElementById("send-button-" + slt).innerHTML = "Clear slot...";
  }
  hasTouchedOrChangedTheConfigurationData();
}

// ADJUST HTML FORM VALUES FOR SLOT
function setTextOptionsForSlot(slt) {
  textType = document.getElementById("textType-" + slt).value;
  if (textType == 1) {
    document.getElementById("duration-label-" + slt).innerHTML =
      "Duration seconds:";
    document.getElementById("offset-interval-label-" + slt).innerHTML =
      "X Offset:";
    document.getElementById("text-options-" + slt).style = "";
  } else if (textType == 2) {
    document.getElementById("duration-label-" + slt).innerHTML =
      "Scroll Cycles:";
    document.getElementById("offset-interval-label-" + slt).innerHTML =
      "Scroll Interval:";
    document.getElementById("text-options-" + slt).style = "";
  } else {
    document.getElementById("text-options-" + slt).style = "display:none";
  }
}

/********************************
 * CORE DEVICE COMMUNICATION
 *********************************/

// READLOOP WHICh RUNNS PERMANENTLY TO READ UNTIL TIMEOUT
async function readLoop() {
  try {
    console.log("UART READING: WAITING FOR DATA ...");
    while (true) {
      // WILL READ AS LONG A NEW DATE IS COMING
      const { value, done } = await reader.read().then();
      if (done) {
        reader.releaseLock();
        break;
      }
      handleIncomingData(value);
      console.log("UART READING: GOT DATA ...");
    }
    console.log("UART READING: EXIT/CANCELLING READLOOP");
  } catch (error) {
    console.error("READ LOOP: FAILED WITH ERROR\n" + error);
    reader = null;
    windowShowWithTypeAndTitleAndInnerHtml(
      "ERROR",
      "error",
      "Error while trying to read from device.",
      error,
    );
    if (error.className == "NetworkError") {
      console.log("UART ERROR: Could not READ from device. (NetworkError)");
    }
  }
}

// HANDLE INCOMING DATA FROm READLOOP AND DETECT IF MESSAGE IS COMPLETE
async function handleIncomingData(data) {
  spinnerShouldShow = true;
  messageBuffer += data;
  // Clear and reset the timeout every time new data is received
  if (timeoutId) {
    clearTimeout(timeoutId);
  }
  // Set a new timeout to process the message if no new data is received within timeout
  timeoutId = setTimeout(() => {
    // ASSUME IF AFTER TIMEOUT NO MORE DATA COMING IN THAT MESSAGE IS COMPLETE, SO PROCESS IT AND RESET BUFFER
    spinnerShouldShow = false;
    processMessage(messageBuffer);
    messageBuffer = ""; // Clear the buffer for the next message
  }, TIMEOUT_NO_MORE_DATA_EXPECTED);
}

// HANDLES A COMPLETE RECEIVED MESSAGE FROM DEVICE
async function processMessage(message) {
  receiveUART(message);
  spinnerShouldShow = false;
}

// MESSAGE RECEIVED, NOW DECIDE HOW TO CONTINUE AFTER RECEIVING THE MESSAGE...
function receiveUART(msg) {
  msg = msg.replaceAll("\x00", "");
  if (msg.startsWith("Slot ")) {
    let slot = msg.charAt(5);
    if (msg.includes(":\r\n")) {
      let lines = msg.split("\r\n");
      let dat = lines[1].split(" ");
      let slotText = lines[2];

      if (dat.length < 7) {
        document.getElementById("textType-" + slot).value = 0;
        document.getElementById("animation-" + slot).value = dat[0];
        document.getElementById("duration-" + slot).value = dat[1];
      } else {
        document.getElementById("text-" + slot).value = slotText;
        document.getElementById("animation-" + slot).value = dat[1];
        document.getElementById("duration-" + slot).value = dat[2];
        document.getElementById("textType-" + slot).value = dat[3];
        document.getElementById("offset-interval-" + slot).value = dat[4];
        document.getElementById("char-scaler-" + slot).value = dat[5];
        document.getElementById("char-space-" + slot).value = dat[6];
      }
      document.getElementById("active-" + slot).checked = true;
      setSlotVisibility(slot);
      setTextOptionsForSlot(slot);
      showRemainingChars(slot);
    } else if (msg.includes("not active")) {
      document.getElementById("active-" + slot).checked = false;
      setSlotVisibility(slot);
    }
    configurationSynchronize();
  }
  if (msg.includes("board_id")) {
    deviceInfo = JSON.parse(msg);
    document.getElementById("hardwareinfo").innerHTML =
      "(Board ID: " +
      deviceInfo["board_id"] +
      ", Firmware: " +
      deviceInfo["firmware"] +
      ", Build: " +
      deviceInfo["build"] +
      " )";
  }

  console.log("UART RECEIVED: " + msg);

  if (gettingData) {
    if (gettingSlot < MAX_NUM_SLOTS - 1) {
      gettingSlot++; // SEND COMMAND TO READ NEXT SLOT...
      sendUART("R " + gettingSlot + "\n");
    } else {
      gettingData = false;
      sendUART("I\n"); // FETCH HARDWARE INFO
    }
  }
}

// SEND MESSAGE TO DEVICE
async function sendUART(msg) {
  console.log("UART SENDING: " + msg);
  if (port) {
    spinnerShouldShow = true;
    try {
      const textEncoder = new TextEncoder();
      writer = port.writable.getWriter();
      await writer.write(textEncoder.encode(msg));
      writer.releaseLock();
      console.log("UART SENDING: SUCCESS");
    } catch (error) {
      console.log("UART SENDING: ERROR\n" + error);
      writer = null;
      if (error.className == "NetworkError") {
        console.error("UART SENDING: (NetworkError)");
      }
      windowShowWithTypeAndTitleAndInnerHtml(
        "ERROR",
        "error",
        "An error occurred while trying to send data.",
        "SENDING TO DEVICE FAILED.<br><br><code>" +
          msg +
          "</code><br><br>NOT SENT, LOST CONNECTION.",
      );
    }
  } else {
    // IF THE PORT IS GONE, GO TO DISCONNECTED STATE
    setStateToDisconnected();
    // windowShowWithTypeAndTitleAndInnerHtml("ERROR", "error", "Device lost connection.", "Port is not open. Please connect to the port first.");
  }
}

// CALLED A SECOND AFTER WE HAVE FOUND VALID PORT FOR INITIAL COMMUNICATION WITH DEVICE (DEVICE HAS TO ANSWER WITHIN 3 SECONDS)
function connectedUART() {
  setTimeout(async () => {
    console.log("UART CONNECTED. REQUESTING DEVICE DATA...");
    setStateToConnected(); // WE HAVE A VALID CONNECTION NOW
    // CONFIGURE TO FETCH ALL DATA FROM DEVICE STARTING WITH SLOT 0
    try {
      gettingData = true;
      gettingSlot = 0;
      defaultSet.displayName = "Current Device Config";
      sendUART("R 0\n");
    } catch (error) {
      // TIMEOUT WHILE TRYING TO GET ALL DATA
      console.error(error);
      setStateToDisconnected(); // WE HAVE NO WORKINg COMMUNICATION, ASSUME DISCONNECTED
    }
  }, "3000");
}

/********************************
 * SERIAL CONNECTION OPEN/CLOSE
 *********************************/

// IF THERE ARE ACCESSIBLE PORTS, IT RETURNS THE FIRST PORT FOUND
async function getAvailablePort() {
  ports = await navigator.serial.getPorts();
  let currentPort = null;
  let portFound = null;
  for (let i = 0; i < ports.length; i++) {
    currentPort = ports[i];
    const { usbProductId, usbVendorId } = currentPort.getInfo();
    // console.log( "AVAILABLE PORTS: PORT #"+i+" AVAILABLE: usbProductId: "+usbProductId+" usbVendorId: "+usbVendorId );
    if (currentPort != null) {
      portFound = currentPort;
      break;
    }
  }
  if (portFound == null) {
    // console.log( "AVAILABLE PORT: NONE." );
  } else {
    console.log("AVAILABLE PORT: " + portFound);
  }
  return portFound;
}

// CHECK IF WE ALREADY HAVE ACCESS TO A PORT (PERMISSION)
async function hasAlreadyWorkingPort() {
  try {
    ports = await navigator.serial.getPorts();
    accessiblePort = ports[0];
    if (
      accessiblePort != null &&
      accessiblePort.connected &&
      accessiblePort.readable
    ) {
      return true;
    } else {
      return false;
    }
  } catch (error) {
    console.error(error);
    return false;
  }
}

// STOPS ALL AUTO-RECONNECT TIMERS & SPINNER TIMER AND CLOSES PORT
async function disconnectFromDevice() {
  if (openPortCheckTimer) {
    clearInterval(openPortCheckTimer);
    openPortCheckTimer = null;
  }
  if (spinnerCheckTimer) {
    clearInterval(spinnerCheckTimer);
    spinnerCheckTimer = null;
  }
  if (port != null) {
    await releaseAllLocks();
    try {
      console.log("PORT: CLOSING...");
      await port.close();
      console.log("PORT: CLOSED.");
      port = null;
      setStateToDisconnected();
    } catch (error) {
      console.error(error);
    }
    spinnerShouldShow = false;
    spinnerCheck();
  }
}

// THE PORT SCANNER WILL INITIATE A RECONNECT
function reconnectToDevice() {
  // START SCANNING PORT AVAILABILITY CONTINOUSLY IN INTERVALS
  if (openPortCheckTimer == null) {
    openPortCheckTimer = setInterval(checkOpenPort, TIMEINTERVAL_PORTS);
  }
  // SPINNER MONITORING (USES BOOl FLAG BECAUSE THIS WORKS BEST)
  if (spinnerCheckTimer == null) {
    spinnerCheckTimer = setInterval(spinnerCheck, TIMEINTERVAL_SPINNER);
  }
  checkOpenPort();
}

// USED TO RELEASE LOCKS BEFORE WE CAN CLOSE THE PORT OF THE CONNECTION
async function releaseAllLocks() {
  try {
    if (port != null) {
      console.log("PORT: RELEASING LOCKS... ");
      if (port.readable) {
        console.warn("READABLE LOCKED: " + port.readable.locked);
      }
      if (port.writable) {
        console.warn("WRITABLE LOCKED: " + port.writable.locked);
      }

      if (port.readable && port.readable.locked) {
        readerAborter.abort();
        reader.cancel();
        await readableStreamClosed.catch(Object);
      }
      if (port.writable && port.writable.locked) {
        writer.cancel();
      }

      if (port.readable) {
        console.info("READABLE LOCKED: " + port.readable.locked);
      }
      if (port.writable) {
        console.info("WRITABLE LOCKED: " + port.writable.locked);
      }
    }
  } catch (error) {
    console.error(error);
  }
}

// CHECKS IF WE HAVE A VALID PORT AND IF THE PORT IS PROPERLY ACESSIBLE (GETS CALLED A LOT)
async function checkOpenPort() {
  // CHECKS IF WE HAVE PENDING UPDATES TO THE CONFIGURATION
  accessiblePort = await getAvailablePort();
  if (accessiblePort) {
    // TRY TO RECONNECT WHEN PORT IS THERE BUT NOT CONNECTED
    port = accessiblePort;
    console.log(
      "PORT IS " + (accessiblePort.connected ? "CONNECTED" : "DISCONNECTED"),
    );
    console.log("PORT IS " + (accessiblePort.readable ? "OPEN" : "CLOSED"));
    if (accessiblePort.connected) {
      // IS AT LEAST CONNECTED, TRY READING...
      try {
        if (!accessiblePort.readable) {
          // NOT READABLE TRY CLOSE AND REOPENING
          // TRY RELEASING LOCKS
          await releaseAllLocks();
          // TRY OPENING DYSFUNCTIONAL PORT
          try {
            await accessiblePort.open({ baudRate: BAUD_RATE });
            const textDecoder = new TextDecoderStream();
            readerAborter = new AbortController();
            readableStreamClosed = accessiblePort.readable.pipeTo(
              textDecoder.writable,
              readerAborter,
            );
            reader = textDecoder.readable.getReader();
            // LAUNCH READ LOOP...
            readLoop();
            // FETCH INFOS FROM TAG
            connectedUART();
          } catch (error) {
            console.log(error);
            let errorString = "" + error;
            if (errorString.indexOf("NetworkError") > -1) {
              windowShowWithTypeAndTitleAndInnerHtml(
                "ERROR",
                "error",
                "Port already in use.",
                "The serial port / USB port is currently already used by another browser window / tab / instance and blocked! Unable to open a connection right now.",
              );
            }
          }
        } else {
          // READABLE, ALL FINE DO NOTHING
          // DO NOTHING
          port = accessiblePort;
        }
      } catch (error) {
        console.error(error);
      }
    } else {
      // NOT CONNECTED, TRY CONNECTION & READING
      try {
        // TRY RELEASING LOCKS
        await releaseAllLocks();
        await accessiblePort.open({ baudRate: BAUD_RATE });
        const textDecoder = new TextDecoderStream();
        readerAborter = new AbortController();
        readableStreamClosed = accessiblePort.readable.pipeTo(
          textDecoder.writable,
          readerAborter,
        );
        reader = textDecoder.readable.getReader();
        // LAUNCH READ LOOP...
        readLoop();
        // FETCH INFOS FROM TAG
        connectedUART();
      } catch (error) {
        console.error(error);
      }
    }
  } else {
    if (hasAlreadyWorkingPort()) {
      port = await getAvailablePort();
      // RE-READ INFOS FROM DEVICE...
      reconnectToDevice();
    } else {
      console.log("NO DEVICE CONNECTED.");
    }
  }
}

/********************************
 * DEVICE NAME HANDLING
 *********************************/

// TRIES TO FIND STORED NAME FOR DEVICE WITH PRODUCT ID
function getDeviceName() {
  if (port == null) return null;
  const { usbProductId, usbVendorId } = port.getInfo();
  if (usbProductId == undefined || usbVendorId == undefined) return undefined;
  let usbDeviceName = null;
  let isKnownDevice = false;
  let storedProductId = getLocalStore("usbProductId", false);
  if ("" + usbProductId == "" + storedProductId) {
    // ALREADY KNOWN PRODUCT
    isKnownDevice = true;
    usbDeviceName = getLocalStore("knownDeviceName-" + usbProductId, true);
  } else {
    // STORE NEW INFO
    isKnownDevice = false;
    setLocalStore("usbProductId", usbProductId, false);
  }
  let storedVendorId = getLocalStore("usbVendorId", false);
  if ("" + usbVendorId == "" + storedVendorId) {
    // ALREADY KNOWN VENDOR
    isKnownDevice = true;
  } else {
    // STORE NEW INFO
    isKnownDevice = false;
    setLocalStore("usbVendorId", usbVendorId, false);
  }
  console.log("DEVICE NAME FOUND: " + usbDeviceName);
  return usbDeviceName;
}

// ENTER A HUMAN READABLE NAME FOR THE DEVICE CONNECTED
function askForDevicename() {
  if (port == null) return null;
  let deviceName = getDeviceName();
  const { usbProductId } = port.getInfo();
  if (usbProductId != undefined) {
    let placeholder = deviceName ? deviceName : "HSHB Name Tag";
    let usbDeviceName = prompt(
      "Please enter a name for your device",
      placeholder,
    );
    if (usbDeviceName != null) {
      if (usbDeviceName == "") {
        localStorage.removeItem("knownDeviceName-" + usbProductId);
      } else {
        setLocalStore("knownDeviceName-" + usbProductId, usbDeviceName, true);
      }
    }
  }
  setStateForDeviceName();
}

// UPDATES THE DISPLAY FOR THE DEVICE NAME
function setStateForDeviceName() {
  let usbDeviceName = getDeviceName();
  if (usbDeviceName != null) {
    document.getElementById("devicename").innerHTML = usbDeviceName;
    toggleClassForElementWithIdTo("devicename", "devicenameshown");
  } else {
    // TRY TO REPLACE WITH MEANINGFUL STUFF
    if (port != null) {
      const { usbProductId, usbVendorId } = port.getInfo();
      document.getElementById("devicename").innerHTML =
        "Unknown (prodId: " + usbProductId + " vendId: " + usbVendorId + ")";
    } else {
      document.getElementById("devicename").innerHTML = "Unknown device";
    }
    toggleClassForElementWithIdTo("devicename", "devicenameunknown");
  }
}

/********************************
 * CORE CONNECTION STATE HANDLING
 *********************************/

// PUTS THE WHOLE UI INTO INCOMPATIBLE STATE
function setStateIncompatible() {
  spinnerShouldShow = false;
  // STATUS
  toggleClassForElementWithIdTo("status", "statusyellow");
  document.getElementById("status").innerHTML = "(Incompatible)";
  // HELP TEXT
  document.getElementById("help").innerHTML =
    "Please try to use a different browser for this operation! Chrome e.g. is one which is known to work fine with this configurator.";
  // BUTTON
  toggleClassForElementWithIdTo("connect", "unconnectable");
  document.getElementById("connect").innerHTML =
    "Connections via USB not supported by this Browser!";
  // DEVICE
  toggleClassForElementWithIdTo("devicename", "devicenameunknown");
  document.getElementById("devicename").innerHTML = "Unknown device";
  if (openPortCheckTimer) {
    clearInterval(openPortCheckTimer);
    openPortCheckTimer = null;
  }
}

// PUTS THE WHOLE UI INTO DISCONNECTED STATE
function setStateToDisconnected() {
  spinnerShouldShow = false;
  // STATUS
  toggleClassForElementWithIdTo("status", "statusred");
  document.getElementById("status").innerHTML = "(Disconnected)";
  // BUTTON
  toggleClassForElementWithIdTo("connect", "disconnected");
  document.getElementById("connect").innerHTML = "Connect via USB ...";
  // DISCONNECT BUTTON
  toggleClassForElementWithIdTo("disconnectbutton", "disconnectbuttonhidden");
  disconnectbutton;
  // DEVICE
  toggleClassForElementWithIdTo("devicename", "devicenameunknown");
  document.getElementById("devicename").innerHTML = "Unknown device";
  document.getElementById("hardwareinfo").innerHTML = "";

  if (openPortCheckTimer) {
    clearInterval(openPortCheckTimer);
    openPortCheckTimer = null;
  }
}

// PUTS THE WHOLE UI INTO CONNECTED STATE
function setStateToConnected() {
  spinnerShouldShow = false;
  // STATUS
  toggleClassForElementWithIdTo("status", "statusgreen");
  document.getElementById("status").innerHTML = "(Connected)";
  // BUTTON
  toggleClassForElementWithIdTo("connect", "connected");
  document.getElementById("connect").innerHTML = "Connected via USB";
  // DISCONNECT BUTTON
  toggleClassForElementWithIdTo("disconnectbutton", "disconnectbutton");
  // DEVICE
  setStateForDeviceName();
  if (openPortCheckTimer == null) {
    openPortCheckTimer = setInterval(checkOpenPort, TIMEINTERVAL_PORTS);
  }
}

// CHECKS IF THE BROWSER SUPPORTS WEB SERIAL API
function isBrowserCompatible() {
  try {
    if (navigator && "serial" in navigator) {
      // DO NOTHIN WE ARE FINE
      return true;
    } else {
      setStateIncompatible();
      return false;
    }
  } catch (error) {
    console.error(error);
    return false;
  }
}

/********************************
 * LAUNCH OF WEBSITE
 *********************************/

// ASSEMBLE HTML CODE NEEDED TO FILL SITE FOR LAUNCH
injectHtmlForSlotsToDOM();

// ATTACH CLICK LISTENER TO CONNECT-BUTTON
document.getElementById("connect").addEventListener("click", async () => {
  // REQUEST A PORT AND OPEN THE CONNECTION
  try {
    // REQUEST PORT IF WE HAVE NO PERMISSION TO A PORT YET (BROWSER WILL PRESENT LIST To SELECT FROM)
    port = await navigator.serial.requestPort();
    // TRY TO GET DEVICE NAME IF KNOWN
    setStateForDeviceName();
    reconnectToDevice();
  } catch (error) {
    // USER DID NOT PICK A PORT, NOTHING TO DO
    if (hasAlreadyWorkingPort()) {
      // CHECK IF WE ARE ALREADY A PORT WITH PERMISSION
      port = getAvailablePort();
      // RE-READ INFOS FROM DEVICE INSTEAD
      reconnectToDevice();
    } else {
      console.error(error);
      windowShowWithTypeAndTitleAndInnerHtml(
        "INFO",
        "info",
        "No serial- / USB-port selected.",
        "Please select a serial port for your device from the list the browser presented to you.",
      );
    }
  }
});

// CHECK IF RIGHT/COMPATIBLE BROWSER, IF NOT SHOW THAT TO USER
if (!isBrowserCompatible()) {
  console.log("ERROR: BROWSER DOES NOT PROVIDE WEB SERIAL-/USB-API.");
} else {
  // ADD BROWSER-LISTENER WHAT TO DO IN CASE WE RE-/CONNECT TO DEVICE
  navigator.serial.addEventListener("connect", (event) => {
    setStateToConnected();
  });

  // ADD BROWSER-LISTENER WHAT TO DO IN CASE WE LOOSE CONNECTION TO DEVICE
  navigator.serial.addEventListener("disconnect", (event) => {
    setStateToDisconnected();
  });

  if (hasAlreadyWorkingPort()) {
    port = getAvailablePort();
    // RE-READ INFOS FROM DEVICE...
    reconnectToDevice();
  } else {
    // USER HAS TO MANUALLY CLICK BUTTON TO SELECT A SERIAL PORT
  }
}

/********************************
 * LEGACY STUFF
 *********************************/
