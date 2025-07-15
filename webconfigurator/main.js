// CONSTANTS
const TIMEOUT_DURATION = 50; // millis
const TIMEINTERVAL_SPINNER = 100; // millis
const MAX_NUM_SLOTS = 9;
const BAUD_RATE = 9600;
const slotsContainer = document.getElementById('slots');
let spinnerShouldShow = false;
let gettingData=false;
let gettingSlot=0;
let writingData=false;
let writingSlot=0;
let port;
let reader;
let writer;
let messageBuffer = '';
let timeoutId = null;

// BUILD DOM
function buildDocument() {
    for (let i = 0; i <= MAX_NUM_SLOTS; i++) {
        const slotDiv = document.createElement('div');
        slotDiv.className = 'slot';
        slotDiv.innerHTML = `
            <h3 style="display:inline"><input type="checkbox" id="active-${i}" onClick="setSlotVisibility(${i})">&nbsp;Slot ${i}</h3><button id="send-button-${i}" onclick="writeSlot(${i})" class="writeslotred">Clear slot...</button> <span id="writeConfirm-${i}" class="writeConfirm hidden">✅</span>
            <div id="slot-content-${i}" style="display:none">
                <fieldset>
                    <legend>Movement, spacing & character width</legend>
                    <label for="textType-${i}">Type</label>
                    <select id="textType-${i}" value="1" onChange="setTextOptions(${i})">
                        <option value="0">None</option>
                        <option value="1" selected>Static</option>
                        <option value="2">Scrolling</option>
                    </select>

                    &nbsp;<label id="offset-interval-label-${i}" for="offset-interval-${i}">X-Offset</label>
                    <input type="number" class="numInput" id="offset-interval-${i}" min="0" max="255" value="0"> 
                    
                    &nbsp;<label for="char-space-${i}">Char Space</label>
                    <input type="number" class="numInput" id="char-space-${i}" min="0" max="60" value="2"> 

                    &nbsp;<label for="char-scaler-${i}">Char Scaler</label>
                    <input type="number" class="numInput" id="char-scaler-${i}" min="1" max="5" value="1">

                </fieldset>
                <div id="text-options-${i}">
                    <fieldset>
                    <legend>Text for display (can be up to 75 chars...)</legend>
                        <input type="text" id="text-${i}" size="75" maxlength="75" value="" placeholder="Text to display goes here...">
                    </fieldset>
                </div>
                <fieldset>
                    <legend>Animation options</legend>
                    <label id="duration-label-${i}" for="duration-${i}">Duration</label>
                    <input type="number" class="numInput" id="duration-${i}" min="1" max="60" value="1"> 
        
                    &nbsp;<label for="animation-${i}">Animation</label>
                    <select id="animation-${i}">
                        <option value="0">Static Off</option>
                        <option value="1" selected>Static On</option>
                        <option value="2">Matrix</option>
                        <option value="3">Sweep</option>
                        <option value="4">Wave</option>
                        <option value="5">Lines</option>
                        <option value="6">Lines Fill</option>
                        <option value="7">Rotate</option>
                        <option value="8">Rotate Fill</option>
                        <option value="9">Circles</option>
                    </select>
                    
                </fieldset>
            </div>
        `;
        slotsContainer.appendChild(slotDiv);
}
}

// CONVENIENCE

function withTimeout(promise, timeout) {
    return Promise
        .race([
            promise,
            new Promise((_, reject) =>
                setTimeout(() => reject(new Error('Timeout')), timeout)
            )
        ]);
}

function toggleClassForElementWithIdTo( element_id, className ) {
    e = document.getElementById( element_id );
    if( e ) {
        // console.log( "ELEMENT: "+element_id+" CLASS: "+className );
        let classesToRemove = e.classList;
        e.classList.remove( classesToRemove );
        e.classList.add( className );
    }
    else {
        console.log( "ERROR: ELEMENT FOR ID "+element_id+" NOT FOUND." );
    }
}

function checkIfBrowserCanDoSerialCommunication() {
    try {
        if( navigator && "serial" in navigator ) {
            toggleClassForElementWithIdTo('connect', "disconnected");
            document.getElementById('connect').innerHTML = 'Connect via USB ...';
            toggleClassForElementWithIdTo('status',"statusred");
            document.getElementById('status').innerHTML = '(Disconnected)';
        }
        else {
            toggleClassForElementWithIdTo('connect',"unconnectable");
            document.getElementById('connect').innerHTML = 'Connections via USB not supported by this Browser!';
            toggleClassForElementWithIdTo('status',"statusyellow");
            document.getElementById('status').innerHTML = '(Incompatible)';
            document.getElementById('help').innerHTML = 'Please try to use a different Webbrowser for this operation! Chrome e.g. is one which is known to work fine with this configurator.';
        }
    }
    catch( error ) {
    }
    finally {
        spinnerShouldShow = false;
    }
}

function showSpinner() {
    //console.log( "SHOULD SHOW SPINNER: "+ document.getElementById('spinner').classList );
    if( "spinnershow" == document.getElementById('spinner').classList ) {
        //console.log( "NOTHING TO DO SHOW" );        
    }
    else {
        //console.log( "NOW SHOWING..." );
        toggleClassForElementWithIdTo('spinner',"spinnershow");
    }
}

function hideSpinner() {
    //console.log( "SHOULD HIDE SPINNER: "+ document.getElementById('spinner').classList );
    if( "spinnerhide" == document.getElementById('spinner').classList ) {
        //console.log( "NOTHING TO DO HIDE" );
    }
    else {
        //console.log( "NOW HIDING..." );
        toggleClassForElementWithIdTo('spinner',"spinnerhide");
    }
}

function spinnerCheck() {
    if( spinnerShouldShow ) {
        showSpinner();
    }
    else {
        hideSpinner();
    }
}

// SLOT HANDLING
function writeSlot(slt) {
    spinnerShouldShow = true;
    if(document.getElementById("active-"+slt).checked) {
        cmd = "W "+slt+" ";
        cmd += document.getElementById("animation-"+slt).value + " ";
        cmd += document.getElementById("duration-"+slt).value + " ";
        cmd += document.getElementById("textType-"+slt).value + " ";
        cmd += document.getElementById("offset-interval-"+slt).value + " ";
        cmd += document.getElementById("char-scaler-"+slt).value + " ";
        cmd += document.getElementById("char-space-"+slt).value + " ";
        cmd += document.getElementById("text-"+slt).value;
        sendUART(cmd+"\n");
        console.log(cmd);
    }
    else {
        sendUART("D "+slt+"\n");
        console.log("D "+slt+"\n");
    }
    if( writingData && writingSlot < MAX_NUM_SLOTS ) {
        spinnerShouldShow = true;
        writingSlot++;
        // launch delayed next write
        setTimeout(() => {
            writeSlot( writingSlot );
        }, "1000");
    }
    else {
        writingData = false;
        spinnerShouldShow = false;
    }
    writeConfirmation(slt);
}

function askWriteAllSlots() {
    if( confirm("Really write all slots now?\n\nWARNING: Inactive slots will be cleared!") ) {
        writeAllSlots();
    }
}

function writeAllSlots() {
    spinnerShouldShow = true;
    console.log("SENDING ALL SLOTS...");
    writingData = true;
    writingSlot = 0;
    writeSlot( writingSlot );
}

function writeConfirmation(slt) {
    document.getElementById("writeConfirm-"+slt).classList.remove('hidden');
    setTimeout(() => {
        document.getElementById("writeConfirm-"+slt).classList.add('hidden');
    }, "1000");
}

function deleteSlot(index) {
    document.getElementById(`text-${index}`).value = '';
    document.getElementById(`animation-${index}`).value = 'none';
    document.getElementById(`textType-${index}`).checked = false;
    document.getElementById(`offset-interval-${index}`).value = '';
    document.getElementById(`duration-${index}`).value = '';
    document.getElementById(`x-offset-${index}`).value = '';
    document.getElementById(`char-scaler-${index}`).value = '';
    document.getElementById(`active-${index}`).checked = false;
}

function setSlotVisibility(slt) {
    if(document.getElementById("active-"+slt).checked) {
        document.getElementById("slot-content-"+slt).style="";
        toggleClassForElementWithIdTo("send-button-"+slt,"writeslot");
        document.getElementById("send-button-"+slt).innerHTML = "Send slot...";

    } else {
        document.getElementById("slot-content-"+slt).style="display:none";
        toggleClassForElementWithIdTo("send-button-"+slt,"writeslotred");
        document.getElementById("send-button-"+slt).innerHTML = "Clear slot...";
    }
}

function receiveUART(msg) {
    msg=msg.replaceAll("\x00", "");
    if(msg.startsWith("Slot ")) {
        let slot = msg.charAt(5);
        if(msg.includes(":\r\n")) {
            let lines=msg.split("\r\n");
            let dat=lines[1].split(' ');
            let slotText=lines[2];
            
            if(dat.length<7) {
                document.getElementById("textType-"+slot).value=0;
                document.getElementById("animation-"+slot).value=dat[0];
                document.getElementById("duration-"+slot).value=dat[1];
            } else {
                document.getElementById("text-"+slot).value=slotText;

                document.getElementById("animation-"+slot).value=dat[1];
                document.getElementById("duration-"+slot).value=dat[2];
                document.getElementById("textType-"+slot).value=dat[3];
                document.getElementById("offset-interval-"+slot).value=dat[4];
                document.getElementById("char-scaler-"+slot).value=dat[5];
                document.getElementById("char-space-"+slot).value=dat[6];
            }
            document.getElementById("active-"+slot).checked = true;
            setSlotVisibility( slot );
            setTextOptions(slot);
        } else if(msg.includes("not active")) {
            document.getElementById("active-"+slot).checked = false;
            setSlotVisibility( slot );
        }
    }
    
   console.log(msg);
    
    if(gettingData) {
        if(gettingSlot < MAX_NUM_SLOTS) {
            gettingSlot++; // SEND COMMAND TO READ NEXT SLOT...
            sendUART("R "+gettingSlot+"\n");
            console.log(gettingSlot);
        }
        else {
            gettingData=false;
        }
    }
}

function connectedUART() {
    spinnerShouldShow = true;
    setTimeout( async () => {
        console.log("UART Connected");
        spinnerShouldShow = false;
        toggleClassForElementWithIdTo('connect',"connected");
        document.getElementById('connect').innerHTML = 'Connected via USB';
        toggleClassForElementWithIdTo('status',"statusgreen");
        document.getElementById('status').innerHTML = '(Connected)';
        gettingData = true;
        gettingSlot = 0;
        try {
            const result = await withTimeout(sendUART("R 0\n"), 5000);
        }
        catch( error ) { // TIMEOUT WHILE CONNECTION TRY
            console.error( error );
            spinnerShouldShow = false;
            toggleClassForElementWithIdTo( 'connect', "disconnected" );
            document.getElementById('connect').innerHTML = 'Connect via USB ...';
            console.error("Port is not open. Please connect to the port first.");
            toggleClassForElementWithIdTo( 'status', "statusred" );
            document.getElementById('status').innerHTML = '(Disconnected)';
        }
    }, "3000");
}

// FORM CONTENT UPDATE HANDLING
function setTextOptions(slt) {
    textType = document.getElementById("textType-"+slt).value;
    if(textType==1) {
        
        document.getElementById("duration-label-"+slt).innerHTML = "Duration:";
        document.getElementById("offset-interval-label-"+slt).innerHTML = "X Offset:";
        document.getElementById("text-options-"+slt).style="";
    } else if(textType==2) {
        document.getElementById("duration-label-"+slt).innerHTML = "Scroll Cycles:";
        document.getElementById("offset-interval-label-"+slt).innerHTML = "Scroll Interval:";
        document.getElementById("text-options-"+slt).style="";
    } else {
        document.getElementById("text-options-"+slt).style="display:none";
    }
}

// SEND & RECEIVE OPERATIONS
// Function to send a message via UART
async function sendUART(msg) {
    spinnerShouldShow = true;
  if (port) {
    const textEncoder = new TextEncoder();
    writer = port.writable.getWriter();
    await writer.write(textEncoder.encode(msg));
    writer.releaseLock();
  }
  else {
    spinnerShouldShow = false;
    toggleClassForElementWithIdTo( 'connect', "disconnected" );
    document.getElementById('connect').innerHTML = 'Connect via USB ...';
    console.error("Port is not open. Please connect to the port first.");
    toggleClassForElementWithIdTo( 'status', "statusred" );
    document.getElementById('status').innerHTML = '(Disconnected)';
    // alert("Port is not open. Please connect to the port first.");
  }
}

// Function to read from UART until timeout
async function readLoop() {
  while( true ) {
    const { value, done } = await reader.read().then(
    );
    if( done ) {
      reader.releaseLock();
      break;
    }
    handleIncomingData(value)
  }
}

// Function to handle incoming data and detect message completion
async function handleIncomingData(data) {
  messageBuffer += data;

  // Clear and reset the timeout every time new data is received
  if (timeoutId) {
    clearTimeout(timeoutId);
  }
  // Set a new timeout to process the message if no new data is received within timeout
  timeoutId = setTimeout(() => {
    processMessage(messageBuffer);
    messageBuffer = ''; // Clear the buffer for the next message
  }, TIMEOUT_DURATION);
}

// Function to process and handle a complete message
async function processMessage(message) {
    spinnerShouldShow = true;
  receiveUART(message);
  spinnerShouldShow = false;
}

async function shutdown() {
    // try to cleanup stuff if badge gets unplugged
    spinnerShouldShow = false;
    if( port ) {
        try {
            writer.releaseLock();
            reader.releaseLock();
        }
        catch( error ) {
            console.log( "ERROR: " + error );
            spinnerShouldShow = false;
        }
        try {
            await port.close();
            if ("serial" in navigator && "forget" in SerialPort.prototype) {
                await port.forget();
            }
        }
        catch( error ) {
            console.log( "ERROR: " + error );
            spinnerShouldShow = false;
        }
        finally {
            toggleClassForElementWithIdTo('connect', "disconnected");
            document.getElementById('connect').innerHTML = 'Connect via USB ...';
            toggleClassForElementWithIdTo('status',"statusred");
            document.getElementById('status').innerHTML = '(Disconnected)';
        }
    }
}

// LAUNCH OF SITE
buildDocument();

// ATTACH Function to connect to the UART port TO CONNECT-BUTTON
document.getElementById('connect').addEventListener('click', async () => {
    // Request a port and open a connection.
  port = await navigator.serial.requestPort();
  spinnerShouldShow = true;
  await port.open({ baudRate: BAUD_RATE });

  // Setup reading
  const textDecoder = new TextDecoderStream();
  const readableStreamClosed = port.readable.pipeTo(textDecoder.writable);
  reader = textDecoder.readable.getReader();

  // Start reading from the port
  readLoop();
  connectedUART();
});

// ADD LISTENERS
if( navigator && ("serial" in navigator) ) {
  navigator.serial.addEventListener("connect", (event) => {
    toggleClassForElementWithIdTo('status',"statusgreen");
    document.getElementById('status').innerHTML = '(Connected)';
    toggleClassForElementWithIdTo('connect',"connected" );
    document.getElementById('connect').innerHTML = 'Connected via USB';
  });

  navigator.serial.addEventListener("disconnect", (event) => {
    toggleClassForElementWithIdTo('status', "statusred");
      document.getElementById('status').innerHTML = '(Disconnected)';
      toggleClassForElementWithIdTo('connect',"disconnected");
      document.getElementById('connect').innerHTML = 'Connect via USB ...';
      shutdown();
  });
}

// SPINNER MONITORING (USES BOOl FLAG BECAUSE THIS WORKS BEST)
setInterval( spinnerCheck, TIMEINTERVAL_SPINNER );

// CHECK IF RIGHT/COMPATIBLE BROWSER
checkIfBrowserCanDoSerialCommunication();