/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-many-to-one-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <esp_now.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
#include <Arduino_JSON.h>

const char* ssid = "";
const char* password = "";

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int id;
  int x1;
  int y1;
  int z1;
  int x2;
  int y2;
  int z2;
  int x3;
  int y3;
  int z3;
  int x4;
  int y4;
  int z4;
}struct_message;

//create a JSON variable
JSONVar theBoard;



//create an Async Web Server on port 80
AsyncWebServer server(80);
AsyncEventSource events("/events");

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board1;

// Create an array with all the structures
//struct_message boardsStruct[3] = {board1, board2, board3};
struct_message boardsStruct[1] = {board1};

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  
  //print mac address of sender
  char macStr[18];
  //Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  //Serial.println(macStr);

  //do memory copy into myData
  memcpy(&myData, incomingData, sizeof(myData));
  //Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  
  // Update the structures with the new incoming data
  theBoard["ID"] = myData.id;
  theBoard["X1"] = myData.x1;
  theBoard["Y1"] = myData.y1;
  theBoard["Z1"] = myData.z1;
  theBoard["X2"] = myData.x2;
  theBoard["Y2"] = myData.y2;
  theBoard["Z2"] = myData.z2;
  theBoard["X3"] = myData.x3;
  theBoard["Y3"] = myData.y3;
  theBoard["Z3"] = myData.z3;
  theBoard["X4"] = myData.x4;
  theBoard["Y4"] = myData.y4;
  theBoard["Z4"] = myData.z4;
  
  
  String jsonString = JSON.stringify(theBoard);
  
  //send the jsonString to browser as an event
  events.send(jsonString.c_str(), "new_readings", millis());

  //print info on Arduino IDE
  Serial.printf("x1: %d, y1: %d, z1: %d \n", myData.x1, myData.y1, myData.z1);
  Serial.printf("x2: %d, y2: %d, z2: %d \n", myData.x2, myData.y2, myData.z2);
  Serial.printf("x3: %d, y3: %d, z3: %d \n", myData.x3, myData.y3, myData.z3);
  Serial.printf("x4: %d, y4: %d, z4: %d \n", myData.x4, myData.y4, myData.z4);
}

//build web (HTML)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP-NOW DASHBOARD</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #2f4468; color: white; font-size: 1.7rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); }
    .reading { font-size: 2.8rem; }
    .packet { color: #bebebe; }
    .card.temperature { color: #fd7e14; }
    .card.humidity { color: #1b78e2; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>ESP-NOW DASHBOARD</h3>
  </div>
  <div class="content">
    <div class="parameters">
      <div class="X1_Val">
        <h4> BOARD #1 - x1Val</h4><p><span class="reading"><span id="x1"></span></span></p>
      </div>
      <div class="Y1_Val">
        <h4> BOARD #1 - y1Val</h4><p><span class="reading"><span id="y1"></span></span></p>
      </div>
      <div class="Z1_Val">
        <h4> BOARD #1 - z1Val</h4><p><span class="reading"><span id="z1"></span></span></p>
      </div>
      <div class="X2_Val">
        <h4> BOARD #1 - x2Val</h4><p><span class="reading"><span id="x2"></span></span></p>
      </div>
      <div class="Y2_Val">
        <h4> BOARD #1 - y2Val</h4><p><span class="reading"><span id="y2"></span></span></p>
      </div>
      <div class="Z2_Val">
        <h4> BOARD #1 - z2Val</h4><p><span class="reading"><span id="z2"></span></span></p>
      </div>
      <div class="X3_Val">
        <h4> BOARD #1 - x3Val</h4><p><span class="reading"><span id="x3"></span></span></p>
      </div>
      <div class="Y3_Val">
        <h4> BOARD #1 - y3Val</h4><p><span class="reading"><span id="y3"></span></span></p>
      </div>
      <div class="Z3_Val">
        <h4> BOARD #1 - z3Val</h4><p><span class="reading"><span id="z3"></span></span></p>
      </div>
      <div class="X4_Val">
        <h4> BOARD #1 - x4Val</h4><p><span class="reading"><span id="x4"></span></span></p>
      </div>
      <div class="Y4_Val">
        <h4> BOARD #1 - y4Val</h4><p><span class="reading"><span id="y4"></span></span></p>
      </div>
      <div class="Z4_Val">
        <h4> BOARD #1 - z4Val</h4><p><span class="reading"><span id="z4"></span></span></p>
      </div>
    </div>
  </div>

<script>
if (!!window.EventSource) {
 // initialize a new eventsource
 var source = new EventSource('/events');

 //add three default event listeners
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);

 //add event listener for 'new_readings'
 source.addEventListener('new_readings', function(e) {
  console.log("new_readings", e.data);
  var obj = JSON.parse(e.data);
  console.log("obj: ", obj);
  console.log("obj x1: ", obj.X1);
  document.getElementById("x1").innerHTML = obj.X1;
  document.getElementById("y1").innerHTML = obj.Y1;
  document.getElementById("z1").innerHTML = obj.Z1;
  document.getElementById("x2").innerHTML = obj.X2;
  document.getElementById("y2").innerHTML = obj.Y2;
  document.getElementById("z2").innerHTML = obj.Z2;
  document.getElementById("x3").innerHTML = obj.X3;
  document.getElementById("y3").innerHTML = obj.Y3;
  document.getElementById("z3").innerHTML = obj.Z3;
  document.getElementById("x4").innerHTML = obj.X4;
  document.getElementById("y4").innerHTML = obj.Y4;
  document.getElementById("z4").innerHTML = obj.Z4;
  
  
 }, false);
}
</script>
</body>
</html>)rawliteral";

 
void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);
  
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Set device as a Wi-Fi Station
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Setting as a Wi-Fi Station..");
  }
  
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

//  // Connect to Wi-Fi network with SSID and password
//  Serial.print("Setting AP (Access Point)…");
//  // Remove the password parameter, if you want the AP (Access Point) to be open
//  WiFi.softAP(ssid, password);
//
//  IPAddress IP = WiFi.softAPIP();
//  Serial.print("AP IP address: ");
//  Serial.println(IP);

  //Serial.println("Hello");

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //Serial.println("Hello");
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  //handle requests
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  //set up event source on server
  events.onConnect([](AsyncEventSourceClient *client){
  if(client->lastId()){
    Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
  }
  // send event with message "hello!", id current millis
  // and set reconnect delay to 1 second
  client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  //start the server
  server.begin();
  
}
 
void loop() {
  static unsigned long lastEventTime = millis();
  static const unsigned long EVENT_INTERVAL_MS = 5000;
  if ((millis() - lastEventTime) > EVENT_INTERVAL_MS) {
    events.send("ping",NULL,millis());
    lastEventTime = millis();
  }
}
