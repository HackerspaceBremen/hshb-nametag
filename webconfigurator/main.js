// CONSTANTS
const TIMEOUT_DURATION = 50; // millis
const TIMEINTERVAL_SPINNER = 100; // millis
const TIMEINTERVAL_PORTS = 10000; // millis
const MAX_NUM_SLOTS = 9;
const BAUD_RATE = 9600;
const COOKIE_EXPIRES_IN_DAYS = 365*10; // 10 Years
const slotsContainer = document.getElementById('slots');
let portScanTimer = null;
let spinnerCheckTimer = null;
let spinnerShouldShow = false;
let gettingData=false;
let gettingSlot=0;
let writingData=false;
let writingSlot=0;
let port = null;
let ports; // LIST OF PORTS WE HAVE PERMISSION TO
// READ
let reader = null;
let readableStreamClosed = null;
let readerAborter = null;
// WRITE
let writer = null;
let writableStreamClosed = null;
let messageBuffer = '';
let timeoutId = null;
let wasDisconnected = false;
let shouldKillReadLoop = false;

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
                    <select id="textType-${i}" value="1" onChange="setTextOptionsForSlot(${i})">
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

/********************************
* GENERAL HELPER FUNCTIONS
*********************************/

// STORE A VALUE INTO PERSISTENT LOCAL STORE (COOKIES DON'T WORK IN LOCAL MODE)
function setLocalStore(cname, cvalue, daysToLive,shouldEncodeBase64) {
    // console.log( "LOCAL STORE: WRITING ... NAME: "+cname+"  VALUE: "+cvalue );
    let encodedValue = cvalue;
    if( shouldEncodeBase64 ) {
        encodedValue = btoa(cvalue);
        // console.log( "LOCAL STORE: WROTE (BASE64) ... "+encodedValue );
    }
    else {
        // console.log( "LOCAL STORE: WROTE ... "+encodedValue );        
    }
    localStorage.setItem(cname, encodedValue);    
}

// RETRIVE A VALUE FROM PERSISTENT LOCAL STORE
function getLocalStore(cname,shouldDecodeBase64) {
    // console.log( "LOCAL STORE: GETTING ... "+cname );
    let extractedBase64Value = localStorage.getItem(cname);
    if( extractedBase64Value == null || extractedBase64Value == undefined ) {
        // console.log( "LOCAL STORE: NOT FOUND '"+cname+"'");
        return undefined;
    }
    if( shouldDecodeBase64 ) {
        // console.log( "LOCAL STORE: READ (BASE64) ... "+extractedBase64Value );
        return atob(extractedBase64Value);
    }
    else {
        // console.log( "LOCAL STORE: READ ... "+extractedBase64Value );
        return extractedBase64Value;
    }
}

// CALL ANY FUNCTION WITH A HARD TINMEOUT LIMIT
function withTimeout(promise, timeout) {
    return Promise
        .race([
            promise,
            new Promise((_, reject) =>
                setTimeout(() => reject(new Error('Timeout')), timeout)
            )
        ]);
}

// SET CSS-STYLE-CLASS FOR AN ELEMENT WITH ID
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

// UPDATES THE ACTIVITY SPINNER ACCORING TO CURRENT STATE (GETS CALLED VERY OFTEN)
function spinnerCheck() {
    if( spinnerShouldShow ) {
        if( "spinnershow" == document.getElementById('spinner').classList ) {
            //console.log( "NOTHING TO DO SHOW" );        
        }
        else {
            toggleClassForElementWithIdTo('spinner',"spinnershow");
        }
    }
    else {
        if( "spinnerhide" == document.getElementById('spinner').classList ) {
            //console.log( "NOTHING TO DO HIDE" );
        }
        else {
            toggleClassForElementWithIdTo('spinner',"spinnerhide");
        }
    }
}

async function releaseAllLocks() {
    try {
        if( port != null ) {
            console.log( "PORT: RELEASING LOCKS... " );
            if( port.readable ) {
                console.warn( "READABLE LOCKED: "+port.readable.locked);
            }
            if( port.writable ) {
                console.warn( "WRITABLE LOCKED: "+port.writable.locked);
            }


            if( port.readable && port.readable.locked ) {
                readerAborter.abort();
                reader.cancel();
                await readableStreamClosed.catch(Object);
            }
            if( port.writable && port.writable.locked ) {
                writer.cancel();
            }

            if( port.readable ) {
                console.info( "READABLE LOCKED: "+port.readable.locked);
            }
            if( port.writable ) {
                console.info( "WRITABLE LOCKED: "+port.writable.locked);
            }
        }
    }
    catch (error) {
        console.error( error );        
    }
}

// CHECKS IF WE HAVE A VALID PORT AND IF THE PORT IS PROPERLY ACESSIBLE (GETS CALLED A LOT)
async function portsCheck() {
    ports = await navigator.serial.getPorts();    
    accessiblePort = ports[0];
    if( accessiblePort ) { // TRY TO RECONNECT WHEN PORT IS THERE BUT NOT CONNECTED
        port = accessiblePort;
        console.log( "PORT IS "+( accessiblePort.connected ? "CONNECTED" : "DISCONNECTED" ) );
        console.log( "PORT IS "+( accessiblePort.readable ? "OPEN" : "CLOSED" ) );
        if( accessiblePort.connected ) { // IS AT LEAST CONNECTED, TRY READING...
            try {
                if( !accessiblePort.readable ) { // NOT READABLE TRY CLOSE AND REOPENING
                    // TRY RELEASING LOCKS
                    await releaseAllLocks();
                    // TRY OPENING DYSFUNCTIONAL PORT
                    try {
                        await accessiblePort.open({ baudRate: BAUD_RATE });    
                        const textDecoder = new TextDecoderStream();
                        readerAborter = new AbortController;
                        readableStreamClosed = accessiblePort.readable.pipeTo(textDecoder.writable, readerAborter);
                        reader = textDecoder.readable.getReader();
                        // LAUNCH READ LOOP...
                        readLoop();
                        // FETCH INFOS FROM TAG
                        connectedUART();
                    }
                    catch (error) {
                        console.error( error );        
                    }
                    
                }
                else { // READABLE, ALL FINE DO NOTHING
                    // DO NOTHING
                    prt = accessiblePort;
                }
            } catch (error) {
                console.error( error );
            }
        }
        else { // NOT CONNECTED, TRY CONNECTION & READING
            try {
                // TRY RELEASING LOCKS
                await releaseAllLocks();                    
                await accessiblePort.open({ baudRate: BAUD_RATE });
                const textDecoder = new TextDecoderStream();
                readerAborter = new AbortController;
                readableStreamClosed = accessiblePort.readable.pipeTo(textDecoder.writable, readerAborter);
                reader = textDecoder.readable.getReader();
                // LAUNCH READ LOOP...
                readLoop();
                // FETCH INFOS FROM TAG
                connectedUART();
            }
            catch( error ) {
                console.error( error );
            }
        }
    }
    else {
        if( hasAlreadyWorkingPort() ) {
            port = getAvailablePort();
            // RE-READ INFOS FROM DEVICE...
            reconnectToDevice();
        }
        else {
            console.log( "NO DEVICE CONNECTED." );
        }
    }
}

/********************************
* HTML RELATED ACTIONS TRIGGERED VIA THE UI
*********************************/

// USER WANTS ALL SLOTS TO BE WRITTEN TO DEVICE
function askWriteAllSlots() {
    if( confirm("Really write all slots now?\n\nWARNING: Inactive slots will be cleared!") ) {
        writeAllSlots();
    }
}

// CONFIGURING WRITE PROCESS TO WRITE ALL SLOTS
function writeAllSlots() {
    spinnerShouldShow = true;
    console.log("SENDING ALL SLOTS...");
    writingData = true;
    writingSlot = 0;
    writeSlot( writingSlot );
}

// WRITING ONE SLOT AND CONTINUING TO WRITE MORE SLOTS IF CONFIGURED
function writeSlot(slt) {
    spinnerShouldShow = true;
    if(document.getElementById("active-"+slt).checked) { // WRITE THIS SLOT
        cmd = "W "+slt+" ";
        cmd += document.getElementById("animation-"+slt).value + " ";
        cmd += document.getElementById("duration-"+slt).value + " ";
        cmd += document.getElementById("textType-"+slt).value + " ";
        cmd += document.getElementById("offset-interval-"+slt).value + " ";
        cmd += document.getElementById("char-scaler-"+slt).value + " ";
        cmd += document.getElementById("char-space-"+slt).value + " ";
        cmd += document.getElementById("text-"+slt).value;
        sendUART(cmd+"\n");
    }
    else { // DELETE SLOT INSTEAD, BECAUSE NOt CHECKED
        sendUART("D "+slt+"\n");
    }
    if( writingData && writingSlot < MAX_NUM_SLOTS ) {
        spinnerShouldShow = true;
        writingSlot++;
        // launch delayed next write
        setTimeout(() => {
            writeSlot( writingSlot );
        }, "1000");
    }
    else { // STOP WRITINg SLOTS WHEN LAST SLOT WAS WRITTEN
        writingData = false;
        spinnerShouldShow = false;
    }
    writeConfirmation(slt);
}

// SHOW AND HIDE SMALL GREEN CHECKMARK
function writeConfirmation(slt) {
    document.getElementById("writeConfirm-"+slt).classList.remove('hidden');
    setTimeout(() => {
        document.getElementById("writeConfirm-"+slt).classList.add('hidden');
    }, "1000");
}

// ** NOT USED **
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

// ADJUST HTML VISIBILITY FOR SLOT
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

// ADJUST HTML FORM VALUES FOR SLOT
function setTextOptionsForSlot(slt) {
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

/********************************
* CORE DEVICE COMMUNICATION
*********************************/

// READLOOP WHICh RUNNS PERMANENTLY TO READ UNTIL TIMEOUT
async function readLoop() {
    try {
        while( true  ) { // WILL READ AS LONG A NEW DATE IS COMING
            const { value, done } = await reader.read().then();
            if( done ) {
                reader.releaseLock();
                break;
            }
            handleIncomingData(value)
            console.log( "UART READING: ..." );
        }
        console.log( "UART READING: EXIT OF READLOOP" );
    }
    catch( error ) {
        reader = null;
        if( error.className == "NetworkError" ) {
            console.log( "UART ERROR: Could not READ from device. (NetworkError)" );
        }        
    }
}

// HANDLE INCOMING DATA FROm READLOOP AND DETECT IF MESSAGE IS COMPLETE
async function handleIncomingData( data ) {
    spinnerShouldShow = true;
    messageBuffer += data;
    // Clear and reset the timeout every time new data is received
    if( timeoutId ) {
        clearTimeout( timeoutId );
    }
    // Set a new timeout to process the message if no new data is received within timeout
    timeoutId = setTimeout(() => { // ASSUME IF AFTER TIMEOUT NO MORE DATA COMING IN THAT MESSAGE IS COMPLETE, SO PROCESS IT AND RESET BUFFER
        spinnerShouldShow = false;
        processMessage( messageBuffer );
        messageBuffer = ''; // Clear the buffer for the next message
    }, TIMEOUT_DURATION);
}

// HANDLES A COMPLETE RECEIVED MESSAGE FROM DEVICE
async function processMessage( message ) {
    receiveUART( message );
    spinnerShouldShow = false;
}

// MESSAGE RECEIVED, NOW DECIDE HOW TO CONTINUE AFTER RECEIVING THE MESSAGE...
function receiveUART( msg ) {
    msg=msg.replaceAll("\x00", "");
    if( msg.startsWith( "Slot " ) ) {
        let slot = msg.charAt(5);
        if(msg.includes(":\r\n")) {
            let lines=msg.split("\r\n");
            let dat=lines[1].split(' ');
            let slotText=lines[2];
            
            if(dat.length<7) {
                document.getElementById("textType-"+slot).value=0;
                document.getElementById("animation-"+slot).value=dat[0];
                document.getElementById("duration-"+slot).value=dat[1];
            }
            else {
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
            setTextOptionsForSlot(slot);
        }
        else if( msg.includes("not active") ) {
            document.getElementById("active-"+slot).checked = false;
            setSlotVisibility( slot );
        }
    }
    
   console.log( "UART RECEIVED: "+msg );
    
    if( gettingData ) {
        if( gettingSlot < MAX_NUM_SLOTS ) {
            gettingSlot++; // SEND COMMAND TO READ NEXT SLOT...
            sendUART( "R "+gettingSlot+"\n" );
        }
        else {
            gettingData=false;
        }
    }
}

// SEND MESSAGE TO DEVICE
async function sendUART( msg ) {
    spinnerShouldShow = true;
    console.log( "UART SENDING: "+msg );
    if( port ) {
        try {
            const textEncoder = new TextEncoder();
            writer = port.writable.getWriter();
            await writer.write(textEncoder.encode(msg));
            writer.releaseLock();
        }
        catch( error ) {
            writer = null;
            if( error.className == "NetworkError" ) {
                console.error( "ERROR: Could not SEND to UART. (NetworkError)" );
            }
        }
    }
    else { // IF THE PORT IS GONE, GO TO DISCONNECTED STATE
        setStateToDisconnected();
        // alert("Port is not open. Please connect to the port first.");
    }
}

// CALLED A SECOND AFTER WE HAVE FOUND VALID PORT FOR INITIAL COMMUNICATION WITH DEVICE (DEVICE HAS TO ANSWER WITHING 3 SECONDS)
function connectedUART() {
    setTimeout( async () => {
        console.log("UART CONNECTED. REQUESTING DEVICE DATA...");
        setStateToConnected(); // WE HAVE A VALID CONNECTION NOW
        // CONFIGURE TO FETCH ALL DATA FROM DEVICE STARTING WITH SLOT 0
        gettingData = true;
        gettingSlot = 0;
        try {
            const result = await withTimeout(sendUART("R 0\n"), 5000);
        }
        catch( error ) { // TIMEOUT WHILE TRYING TO GET ALL DATA
            console.error( error );
            setStateToDisconnected(); // WE HAVE NO WORKINg COMMUNICATION, ASSUME DISCONNECTED
        }
    }, "3000");
}

/********************************
* DEVICE NAME HANDLING
*********************************/

function getDeviceName() {
    if( port == null ) return null;
    const { usbProductId, usbVendorId } = port.getInfo();
    if( usbProductId == undefined || usbVendorId == undefined ) return undefined;
    let usbDeviceName = null;
    let isKnownDevice = false;
    let storedProductId = getLocalStore( "usbProductId", false );
    if( ""+usbProductId == ""+storedProductId ) { // ALREADY KNOWN PRODUCT
        isKnownDevice = true;
        usbDeviceName = getLocalStore( "knownDeviceName-"+usbProductId, true );
    }
    else { // STORE NEW INFO
        isKnownDevice = false;
        setLocalStore( "usbProductId", usbProductId, COOKIE_EXPIRES_IN_DAYS );
    }
    let storedVendorId = getLocalStore( "usbVendorId", false );
    if( ""+usbVendorId == ""+storedVendorId ) { // ALREADY KNOWN VENDOR
        isKnownDevice = true;
    }
    else { // STORE NEW INFO
        isKnownDevice = false;
        setLocalStore( "usbVendorId", usbVendorId );
    }
    console.log( "DEVICE NAME FOUND: "+usbDeviceName );
    return usbDeviceName;
}

function askForDevicename() {
    let deviceName = getDeviceName();
    const { usbProductId } = port.getInfo();
    let placeholder = deviceName ? deviceName : "HSHB Name Tag";
    let usbDeviceName = prompt("Please enter a name for your device", placeholder);
    if (usbDeviceName != null) {
        setLocalStore( "knownDeviceName-"+usbProductId, usbDeviceName, COOKIE_EXPIRES_IN_DAYS, true );
    }
    setStateForDeviceName();
    return usbDeviceName;
}

async function getAvailablePort() {
    await navigator.serial.getPorts().then( (ports) => {
        let currentPort = null;
        for( let i = 0; i < ports.length; i++ ) {
            currentPort = ports[i];
            const { usbProductId, usbVendorId } = currentPort.getInfo();
            // console.log( "AVAILABLE PORTS: PORT #"+i+" AVAILABLE: usbProductId: "+usbProductId+" usbVendorId: "+usbVendorId );
            if( currentPort != null ) {
                return currentPort;
            }
        }
        // console.log( "AVAILABLE PORTS: NONE." );
        return currentPort;
    } );
}

function setStateForDeviceName() {
    let usbDeviceName = getDeviceName();
    if( usbDeviceName != null ) {
        document.getElementById('devicename').innerHTML = usbDeviceName;
        toggleClassForElementWithIdTo("devicename","devicenameshown");
    }
    else {
        document.getElementById('devicename').innerHTML = "Unknown device";
        toggleClassForElementWithIdTo("devicename","devicenameunknown");            
    }
}

/********************************
* CORE CONNECTION STATE HANDLING
*********************************/

function setStateIncompatible() {
    spinnerShouldShow = false;
    // STATUS
    toggleClassForElementWithIdTo('status',"statusyellow");
    document.getElementById('status').innerHTML = '(Incompatible)';
    // HELP TEXT
    document.getElementById('help').innerHTML = 'Please try to use a different browser for this operation! Chrome e.g. is one which is known to work fine with this configurator.';
    // BUTTON
    toggleClassForElementWithIdTo('connect',"unconnectable");
    document.getElementById('connect').innerHTML = 'Connections via USB not supported by this Browser!';
    // DEVICE
    toggleClassForElementWithIdTo("devicename","devicenameunknown");
    document.getElementById('devicename').innerHTML = "Unknown device";
    if( portScanTimer ) {
        clearInterval(portScanTimer);
        portScanTimer = null;
    }    
}

function setStateToDisconnected() {
    spinnerShouldShow = false;
    // STATUS
    toggleClassForElementWithIdTo('status', "statusred");
    document.getElementById('status').innerHTML = '(Disconnected)';
    // BUTTON
    toggleClassForElementWithIdTo('connect',"disconnected");
    document.getElementById('connect').innerHTML = 'Connect via USB ...';
    // DISCONNECT BUTTON
    toggleClassForElementWithIdTo('disconnectbutton',"disconnectbuttonhidden");
    disconnectbutton
    // DEVICE
    toggleClassForElementWithIdTo("devicename","devicenameunknown");
    document.getElementById('devicename').innerHTML = "Unknown device";
    if( portScanTimer ) {
        clearInterval(portScanTimer);
        portScanTimer = null;
    }    
}

function setStateToConnected() {
    spinnerShouldShow = false;
    // STATUS
    toggleClassForElementWithIdTo('status',"statusgreen");
    document.getElementById('status').innerHTML = '(Connected)';
    // BUTTON
    toggleClassForElementWithIdTo('connect',"connected" );
    document.getElementById('connect').innerHTML = 'Connected via USB';
    // DISCONNECT BUTTON
    toggleClassForElementWithIdTo('disconnectbutton',"disconnectbutton");
    // DEVICE
    setStateForDeviceName();
    if( portScanTimer == null ) {
        portScanTimer = setInterval( portsCheck, TIMEINTERVAL_PORTS );        
    }    
}

async function disconnectFromDevice() {
    if( portScanTimer ) {
        clearInterval(portScanTimer);
        portScanTimer = null;
    }    
    if( spinnerCheckTimer ) {
        clearInterval(spinnerCheckTimer);
        spinnerCheckTimer = null;
    }    
    if( port != null ) {
        await releaseAllLocks();
        try {
            console.log( "PORT: CLOSING..." );
            await port.close();
            console.log( "PORT: CLOSED." );
            setStateToDisconnected();
        }
        catch (error) {
            console.error( error );    
        }
    }
}

// THE PORT SCANNER WILL INITIATE A RECONNECT
function reconnectToDevice() {
    // START SCANNING PORT AVAILABILITY CONTINOUSLY IN INTERVALS
    if( portScanTimer == null ) {
        portScanTimer = setInterval( portsCheck, TIMEINTERVAL_PORTS );        
    }
    // SPINNER MONITORING (USES BOOl FLAG BECAUSE THIS WORKS BEST)
    if( spinnerCheckTimer == null ) {
        spinnerCheckTimer = setInterval( spinnerCheck, TIMEINTERVAL_SPINNER );
    }
    portsCheck();
}

function isBrowserCompatible() {
    try {
        if( navigator && "serial" in navigator ) {
            // DO NOTHIN WE ARE FINE
            return true;
        }
        else {
            setStateIncompatible();
            return false;
        }
    }
    catch( error ) {
        console.error( error );
        return false;
    }
}

async function hasAlreadyWorkingPort() {
    try {
        ports = await navigator.serial.getPorts();    
        accessiblePort = ports[0];
        if( accessiblePort != null && accessiblePort.connected && accessiblePort.readable ) {
            return true;
        }
        else {
            return false;
        }
    } 
    catch( error ) {
        console.error( error );
        return false;
    }
}

/********************************
* LAUNCH OF WEBSITE
*********************************/

// ASSEMBLE HTML CODE NEEDED TO FILL SITE FOR LAUNCH
buildDocument();

// ATTACH CLICK LISTENER TO CONNECT-BUTTON
document.getElementById('connect').addEventListener('click', async () => {
    // CHECK IF WE ARE ALREADY CONNECTED FIRST...
    if( hasAlreadyWorkingPort() ) {
        port = getAvailablePort();
        // RE-READ INFOS FROm DEVICE INSTEAD
        reconnectToDevice();
    }
    // REQUEST A PORT AND OPEN THE CONNECTION
    try {
        // REQUEST PORT IF WE HAVE NO PERMISSION TO A PORT YET (BROWSER WILL PRESENT LIST To SELECT FROM)
        port = await navigator.serial.requestPort();
        // TRY TO GET DEVICE NAME IF KNOWN
        setStateForDeviceName();
        reconnectToDevice();
    }
    catch( error ) { // USER DID NOT PICK A PORT, NOTHING TO DO
        if( hasAlreadyWorkingPort() ) { // DISCONNECT MANUALLY
            // 
        }
        else {
            console.error( error );
            alert( "Please select a serial port for your device from the list the browser presented to you." );
        }
    }
});

// ADD BROWSER-LISTENER WHAT TO DO IN CASE WE RE-/CONNECT TO DEVICE
if( navigator && ("serial" in navigator) ) {
    navigator.serial.addEventListener("connect", (event) => {
        setStateToConnected();
    });

    // ADD BROWSER-LISTENER WHAT TO DO IN CASE WE LOOSE CONNECTION TO DEVICE
    navigator.serial.addEventListener("disconnect", (event) => {
        setStateToDisconnected();
    });
}

// CHECK IF RIGHT/COMPATIBLE BROWSER, IF NOT SHOW THAT TO USER
if( !isBrowserCompatible() ) {
    console.log( "ERROR: BROWSER DOES NOT PROVIDE WEB SERIAL-/USB-API." );
}
else {
    if( hasAlreadyWorkingPort() ) {
        port = getAvailablePort();
        // RE-READ INFOS FROM DEVICE...
        reconnectToDevice();
    }
    else {
        // USER HAS TO MANUALLY CLICK BUTTON TO SELECT A SERIAL PORT
    }
}