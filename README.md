# Wearable-Device-VIP---Wireless

This instruction is for the new incomings who will contribute to this project.

## Description

This project serves for the wireless connection part for NYU VIP Team - Smart Wearable for TeleHealth.
This project belongs to the Wireless Team in this VIP. There are two other teams which is the Software Team and the Hardware Team.
The Wireless Team functions as a bridge that converge the works that has been done from the two other teams.
The goal of this project is to design a visual interface as well as extensive functionalities that supports the machine learning algorithms from Software Team to be applied on the wearable device designed by the Hardware Team.
In this project, we want to host an ESP32 web server and use ESP-NOW communication protocol at the same time. You can have several ESP32 boards sending sensor readings via ESP-NOW to one ESP32 receiver that displays all readings on a web server. The boards will be programmed using Arduino IDE.


## Getting Started

### Installing
Before proceeding with this project, make sure you check the following prerequisites.

* Arduino IDE 
    - We’ll program the ESP32/TinyPICO boards using Arduino IDE, so before proceeding with this tutorial, make sure you have the ESP32 board installed in your Arduino IDE. 
    - Follow this tutorial for setting up ESP32 if you need: [Tutorial](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)
    - The set up for TinyPICO Board on Arduino IDE is similar to the above. The only difference is you choose "TinyPICO" from Tools > Board. 
    
* ADXL345 Library
    - Go to Sketch > Include Library > Manage Libraries, search for "SparkFun_ADXL345" and click install.
    
* Arduino_JSON Library
    - Go to Sketch > Include Library > Manage Libraries, search for "Arduino_JSON" and click install.

* Async Web Server Libraries
    - To build the web server you need to install the following libraries:
    - [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
    - [AsyncTCP](https://github.com/me-no-dev/AsyncTCP)
    - These two libraries aren’t available to install through the Arduino Library Manager, so you need to copy the library files to the Arduino Installation Libraries folder. Alternatively, in your Arduino IDE, you can go to Sketch > Include Library > Add .zip Library and select the libraries you’ve just downloaded.
    

### Parts Required
* To follow this tutorial, you need one ESP32 board (as Receiver), one TinyPICO board (as Sender). You also need:
    - 1x PCB Board
    - 2x ~ 8x ADXL345 sensors
    - 1x ~ 2x Wires with Micro-USB
    - 1x Battery
    

### Getting the Receiver Board (ESP32) MAC Address
1. To send messages via ESP-NOW, you need to know the receiver board’s MAC address. Each board has a unique MAC address.
2. Upload the code file that's named as "findMacAddr" to your ESP32 receiver board to get its MAC address.
3. Always hold the 'BOOT' button located as the lower right corner on ESP32 when you are uploading codes to ESP32 board (No need to press anything while uploading codes to TinyPICO board).
4. After uploading the code, press the RST/EN button, and the MAC address should be displayed on the Serial Monitor. Eg. 0x30, 0xC6, 0xF7, 0x0B, 0x49, 0x24. FInd a notebook to take down this Mac address.


### ESP32 Receiver (ESP-NOW + WebServer)
1. The ESP32 receiver board receives the packets from the sender boards and hosts a web server to display the latest received readings.
2. Insert your network credentials on the following lines so that the ESP32 can connect to your local network.
    ```
    const char* ssid = "REPLACE_WITH_YOUR_SSID";
    const char* password = "REPLACE_WITH_YOUR_PASSWORD";
    ```
3. Upload the code file "Receiver" to your receiver board – the code is prepared to receive readings from the PCB board that is assembled with TinyPICO board and ADXL345 sensors. The ESP32 IP address should be printed on the Serial Monitor as well as the Wi-Fi channel.
    
    <img width="672" alt="Screen Shot 2022-04-26 at 8 56 17 PM" src="https://user-images.githubusercontent.com/46912813/165416959-7784e304-8819-4cf6-a447-15f4637254b6.png">
4. Please refer to the tutorial [esp32-esp-now-wi-fi-web-server](https://randomnerdtutorials.com/esp32-esp-now-wi-fi-web-server/) here if you are not sure about the details in this code snippet. 


### Assemble your PCB board
* Make sure register names on ADXL345 line correctly with those on the PCB board.
* When you plug in the batteries, remember that the black wire goes into Ground.
* You should have setup similar to the image below when you finish.
* <img  alt="Screen Shot 2022-04-26 at 8 26 15 PM" src="https://user-images.githubusercontent.com/46912813/165414280-6dcf3874-aabe-411f-be3a-a2e49bc9b484.png" width="400" height="350">

### ESP32 Sender Code (ESP-NOW)
1. The ESP32 sender board reads data from the ADXL345 (xVal, yVal, zVal) and wrap the readings into structure that then be sent to the ESP32 receiver board through ESP-NOW protocol.
2. Insert the Mac Address of your receiver board that you've saved into this part.
   ```
   uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    ```
3. Insert your network credentials on the following lines so that the ESP32 can connect to your local network.
     ```
    constexpr char WIFI_SSID[] = "REPLACE_WITH_YOUR_SSID";
    ```
4. Upload the code file "Sender" to your sender board. You should notice that the boards change their Wi-Fi channel to the channel of the receiver board.In our case, the boards changed their Wi-Fi channel number to 6.
    
    <img width="668" alt="Screen Shot 2022-04-26 at 8 59 04 PM" src="https://user-images.githubusercontent.com/46912813/165417219-cf1ef502-d3bb-442a-b05d-58bbabe9972c.png">

    
5. Please refer to the tutorial [esp32-esp-now-wi-fi-web-server](https://randomnerdtutorials.com/esp32-esp-now-wi-fi-web-server/) here if you are not sure about the details in this code snippet.

## Demonstration
* After uploading the code to all the boards and if everything is going as expected, the ESP32 receiver board should start receiving sensor readings from the PCB board.
* Open a browser on your local network and type the ESP32 IP address. You Should see something like this.
    
    <img  alt="Screen Shot 2022-04-26 at 9 15 03 PM" src="https://user-images.githubusercontent.com/46912813/165418680-959272cd-b6e5-48e6-a8ad-1e1d61bab154.png" width="760" height="450" >

    
## Acknowledgments

Inspiration:
* [esp32-esp-now-wi-fi-web-server](https://randomnerdtutorials.com/esp32-esp-now-wi-fi-web-server/)
