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
#include "I2Cdev.h"
#include "MPU6050.h"//mpu library
#include "Wire.h"//i2c library
#define TCAADDR 0x70 //mux address

///////////////////
//set up the sensors
MPU6050 sensor_1; 
MPU6050 sensor_2; 
MPU6050 sensor_3; 
MPU6050 sensor_4;
MPU6050 sensor_5; 
MPU6050 sensor_6; 
MPU6050 sensor_7; 
MPU6050 sensor_8;

 
int16_t ax1, ay1, az1, ax2, ay2, az2, ax3, ay3, az3, ax4, ay4, az4;
int16_t ax5, ay5, az5, ax6, ay6, az6, ax7, ay7, az7, ax8, ay8, az8;

const uint16_t MPU = 0x68;  
uint8_t size = 6;

///////////////////


// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0x30, 0xC6, 0xF7, 0x0B, 0x49, 0x24};

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
//constexpr char WIFI_SSID[] = "MyESP32AP";
constexpr char WIFI_SSID[] = "Verizon-MiFi8800L-9216";


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

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void tcaselect(uint8_t i)//read mux buses
{
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void sensorSetUp() {
  Serial.println("Setting up sensors");
  Wire.begin();           //Ininit I2C  and serial port
  Wire.setClock(400000);

  //Sensor 1 and 2 in unit 1
  tcaselect(0);
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  sensor_1.setXAccelOffset(-4810);
  sensor_1.setYAccelOffset(-2600);
  sensor_1.setZAccelOffset(1867);


  tcaselect(1);
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  sensor_2.setXAccelOffset(1043);
  sensor_2.setYAccelOffset(820);
  sensor_2.setZAccelOffset(1099);
 
  
  //Sensor 3 and 4 in unit 2
  tcaselect(2);
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  sensor_3.setXAccelOffset(-753);
  sensor_3.setYAccelOffset(-624);
  sensor_3.setZAccelOffset(2338);
  
  tcaselect(3);
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  sensor_4.setXAccelOffset(287);
  sensor_4.setYAccelOffset(4116);
  sensor_4.setZAccelOffset(1173);
  
  //Sensor 5 and 6 in unit 3
  tcaselect(4);
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  sensor_5.setXAccelOffset(1880 );
  sensor_5.setYAccelOffset(-432);
  sensor_5.setZAccelOffset(345);
  
  tcaselect(5);
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  sensor_6.setXAccelOffset(-3999);
  sensor_6.setYAccelOffset(-2244 );
  sensor_6.setZAccelOffset(1117);

  //Sensor 7 and 8 in unit 4
  tcaselect(6);
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  sensor_7.setXAccelOffset(-3600);
  sensor_7.setYAccelOffset(470);
  sensor_7.setZAccelOffset(916); 
  tcaselect(7);
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  sensor_8.setXAccelOffset(-3449);
  sensor_8.setYAccelOffset(-887);
  sensor_8.setZAccelOffset(1326);
  
 
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
  tcaselect(0);
  Wire.beginTransmission(MPU);  
  ax1 = sensor_1.getAccelerationX();
  ay1 = sensor_1.getAccelerationY();
  az1 = sensor_1.getAccelerationZ();
  Wire.write(0x3B);  // register MPU6050  starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,size,true);// From 0x3B, it gets 6 registers 
  ax1=Wire.read()<<8|Wire.read();//Each value uses 2 registers
  ay1=Wire.read()<<8|Wire.read();
  az1=Wire.read()<<8|Wire.read();
 

  tcaselect(1);
  Wire.beginTransmission(MPU); 
  ax2 = sensor_2.getAccelerationX();
  ay2 = sensor_2.getAccelerationY();
  az2 = sensor_2.getAccelerationZ();
  Wire.write(0x3B);  // register MPU6050  starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,size,true);// From 0x3B, it gets 6 registers 
  ax2=Wire.read()<<8|Wire.read();//Each value uses 2 registers
  ay2=Wire.read()<<8|Wire.read();
  az2=Wire.read()<<8|Wire.read();
 
 
  
  tcaselect(2);
  Wire.beginTransmission(MPU); 
  ax3 = sensor_3.getAccelerationX();
  ay3 = sensor_3.getAccelerationY();
  az3 = sensor_3.getAccelerationZ();
  Wire.write(0x3B);  // register MPU6050  starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,size,true);// From 0x3B, it gets 6 registers 
  ax3=Wire.read()<<8|Wire.read();//Each value uses 2 registers
  ay3=Wire.read()<<8|Wire.read();
  az3=Wire.read()<<8|Wire.read();


  tcaselect(3);
  Wire.beginTransmission(MPU);
  ax4 = sensor_4.getAccelerationX();
  ay4 = sensor_4.getAccelerationY();
  az4 = sensor_4.getAccelerationZ();
  Wire.write(0x3B);  // register MPU6050  starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,size,true);// From 0x3B, it gets 6 registers 
  ax4=Wire.read()<<8|Wire.read();//Each value uses 2 registers
  ay4=Wire.read()<<8|Wire.read();
  az4=Wire.read()<<8|Wire.read();
  
  
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;

    // Set values to send
    myData.id = 1;
    myData.x1 = ax1;
    myData.y1 = ay1;
    myData.z1 = az1;
    myData.x2 = ax2;
    myData.y2 = ay2;
    myData.z2 = az2;
    myData.x3 = ax3;
    myData.y3 = ay3;
    myData.z3 = az3;
    myData.x4 = ax4;
    myData.y4 = ay4;
    myData.z4 = az4;
    

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
