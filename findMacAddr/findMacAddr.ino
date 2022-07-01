// Complete Instructions to Get and Change ESP MAC Address: https://RandomNerdTutorials.com/get-change-esp32-esp8266-mac-address-arduino/

/*
Running The MAC Sketch
The entire sketch runs in the Setup section, so after loading it to the ESP32, it will likely run before you get a chance to view it on the Serial monitor.
You can press the Reset key on your module to force it to run again.
The MAC Address will be at the bottom of the screen. Copy it to a safe location, so that you can use it later.
*/

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}
 
void loop(){

}
