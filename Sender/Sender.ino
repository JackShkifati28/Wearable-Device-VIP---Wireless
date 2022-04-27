/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-many-to-one-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <SparkFun_ADXL345.h>

/*********** COMMUNICATION SELECTION ***********/
  ADXL345 adxl1a = ADXL345(5);           // USE FOR SPI COMMUNICATION, ADXL345(CS_PIN);
  ADXL345 adxl1b = ADXL345(21);
  ADXL345 adxl2a = ADXL345(22);
  ADXL345 adxl2b = ADXL345(32);
  ADXL345 adxl3a = ADXL345(33);
  ADXL345 adxl3b = ADXL345(25);
  ADXL345 adxl4a = ADXL345(26);
  ADXL345 adxl4b = ADXL345(27);
 /******************** SETUP ********************/
 /*          Configure ADXL345 Settings         */


// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    int id; // must be unique for each sender board
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
} struct_message;

// Create a struct_message called myData
struct_message myData;

unsigned long previousMillis = 0;   // Stores last time x, y values was published
const long interval = 1;        // Interval at which to publish sensor readings


// Create peer interface
esp_now_peer_info_t peerInfo;

//insert Wifi SSID
constexpr char WIFI_SSID[] = "REPLACE_WITH_YOUR_SSID";


int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
      for (uint8_t i=0; i<n; i++) {
          if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
              return WiFi.channel(i);
          }
      }
  }
  return 0;
}

void sensorSetUp() {
  Serial.println("Setting up sensors");
  adxl1a.powerOn();             // Power on the ADXL345
  adxl1b.powerOn();
  adxl2a.powerOn();
  adxl2b.powerOn();
  adxl3a.powerOn();
  adxl3b.powerOn();
  adxl4a.powerOn();
  adxl4b.powerOn();
  adxl1a.setRangeSetting(2);    // Give the range settings
  adxl1b.setRangeSetting(2);    // Accepted values are 2g, 4g, 8g or 16g
  adxl2a.setRangeSetting(2);    // Higher Values = Wider Measurement Range
  adxl2b.setRangeSetting(2);    // Lower Values = Greater Sensitivity
  adxl3a.setRangeSetting(2);
  adxl3b.setRangeSetting(2);
  adxl4a.setRangeSetting(2);
  adxl4b.setRangeSetting(2);
  adxl1a.setSpiBit(0);          // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
  adxl1b.setSpiBit(0);          // Default: Set to 1
  adxl2a.setSpiBit(0);          // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library
  adxl2b.setSpiBit(0);
  adxl3a.setSpiBit(0);
  adxl3b.setSpiBit(0);
  adxl4a.setSpiBit(0);
  adxl4b.setSpiBit(0);  
  }

int readX1() {
  }


// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // Set up sensros
  sensorSetUp();

  // Set device as a Wi-Fi Station and set channel
  WiFi.mode(WIFI_STA);

  int32_t channel = getWiFiChannel(WIFI_SSID);

  WiFi.printDiag(Serial); // Uncomment to verify channel number before
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  WiFi.printDiag(Serial); // Uncomment to verify channel change after

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  //peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Accelerometer Readings
  int x1a, y1a, z1a, x1b, y1b, z1b;
  int x2a, y2a, z2a, x2b, y2b, z2b;
  int x3a, y3a, z3a, x3b, y3b, z3b;
  int x4a, y4a, z4a, x4b, y4b, z4b;
  adxl1a.readAccel(&x1a, &y1a, &z1a);
  adxl1b.readAccel(&x1b, &y1b, &z1b);
  adxl2a.readAccel(&x2a, &y2a, &z2a);
  adxl2b.readAccel(&x2b, &y2b, &z2b);
  adxl3a.readAccel(&x3a, &y3a, &z3a);
  adxl3b.readAccel(&x3b, &y3b, &z3b);
  adxl4a.readAccel(&x4a, &y4a, &z4a);
  adxl4b.readAccel(&x4b, &y4b, &z4b);
  
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;

    // Set values to send
    myData.id = 1;
    myData.x1 = x1a-x1b;
    myData.y1 = y1a-y1b;
    myData.z1 = z1a-z1b;
    myData.x2 = x2a-x2b;
    myData.y2 = y2a-y2b;
    myData.z2 = z2a-z2b;
    myData.x3 = x3a-x3b;
    myData.y3 = y3a-y3b;
    myData.z3 = z3a-z3b;
    myData.x4 = x4a-x4b;
    myData.y4 = y4a-y4b;
    myData.z4 = z4a-z4b;
    

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));   
    if (result == ESP_OK) {
      Serial.println("Sent with success");
      }
    else {
      Serial.println("Error sending the data");
      }
    delay(9);
    
    }
  
}
