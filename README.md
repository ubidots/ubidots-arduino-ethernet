# Ubidots-arduino-ethernet

Ubidots-Ethernet is an Arduino library for interacting with Ubidots through its API with arduino ethernet shield. The library also contains the code to connect to your Ethernet network.

We can't guarantee that it will work with different Ethernet modules.

## Requiremets

* [Arduino UNO](https://www.arduino.cc/en/Main/ArduinoBoardUno)
* [Arduino Ethernet Shield](https://www.arduino.cc/en/Main/ArduinoEthernetShield)
* [Arduino IDE 1.6.5 or higher](https://www.arduino.cc/en/Main/Software)
* [UbidotsArduino Ethernet library](https://github.com/ubidots/ubidots-arduino-ethernet/archive/master.zip)


## Setup


1. Download the UbidotsArduino library [here](https://github.com/ubidots/ubidots-arduino-ethernet/archive/master.zip)
2. Go to the Arduino IDE, click on **Sketch -> Include Library -> Add .ZIP Library**
3. Select the .ZIP file of UbidotsEthernet and then "Accept" or "Choose"
4. Do the same to add the AdafruitEthernet library.
5. Close the Arduino IDE and open it again.

    
## Send one value to Ubidots

To send a value to Ubidots, go to **Sketch -> Examples -> UbidotsArduino library** and select the "saveValue" example. 
Put your Ubidots token and variable ID where indicated, as well as the WiFi settings.
Upload the code, open the Serial monitor to check the results. If no response is seen, try unplugging your Arduino and then plugging it again. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.

```c++
#include <Ethernet.h>
#include <SPI.h>
#include <UbidotsEthernet.h>
#define ID  "Your_variable_ID_here"  // Put here your Ubidots variable ID
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

Ubidots client(TOKEN);

void setup(){
    Serial.begin(9600);
    // start the Ethernet connection:
    if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
      // try to congifure using IP address instead of DHCP:
      Ethernet.begin(mac, ip);
    }
    // give the Ethernet shield a second to initialize:
    delay(1000);
}
void loop(){
    float value = analogRead(A0);
    client.add(ID, value);
    client.sendAll();
}
```


## Get one value from Ubidots

To get the last value of a variable from Ubidots, go to **Sketch -> Examples -> UbidotsArduino library** and select the "getValue" example. 
Put your Ubidots token and variable ID where indicated, as well as the WiFi settings.
Upload the code, open the Serial monitor to check the results. If no response is seen, try unplugging your Arduino and then plugging it again. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.

```c++

#include <Ethernet.h>
#include <SPI.h>
#include <UbidotsEthernet.h>
#define ID  "Your_variable_ID_here"  // Put here your Ubidots variable ID
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN


// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

Ubidots client(TOKEN);

void setup(){
    Serial.begin(9600);
    // start the Ethernet connection:
    if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
      // try to congifure using IP address instead of DHCP:
      Ethernet.begin(mac, ip);
    }
    // give the Ethernet shield a second to initialize:
    delay(1000);
    
}
void loop(){
    float value = client.getValue(ID);
}
```

## Send multiple values to Ubidots 

To send multiple values to Ubidots, go to **Sketch -> Examples -> UbidotsArduino library** and select the "saveValues" example. 
Put your Ubidots token and variable ID's where indicated,  as well as the APN settings
Upload the code, open the Serial monitor to check the results. If no response is seen, try unplugging your Arduino and then plugging it again. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.

```c++
#include <Ethernet.h>
#include <SPI.h>
#include <UbidotsEthernet.h>
#define ID  "Your_variable_ID_here"  // Put here your Ubidots variable ID
#define ID2 "Your_variable_ID2_here"
#define ID3 "Your_variable_ID3_here"
#define ID4 "Your_variable_ID4_here"
#define TOKEN  "Your_token_here"  // Put here your Ubidots TOKEN

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

Ubidots client(TOKEN);

void setup(){
    Serial.begin(9600);
    // start the Ethernet connection:
    if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
      // try to congifure using IP address instead of DHCP:
      Ethernet.begin(mac, ip);
    }
    // give the Ethernet shield a second to initialize:
    delay(1000);
}
void loop(){
    float value = analogRead(A0);
    float value2 = analogRead(A1);
    float value3 = analogRead(A2);
    float value4 = analogRead(A3);
    client.add(ID, value);
    client.add(ID2, value2);
    client.add(ID3, value3);
    client.add(ID4, value4);
    client.sendAll();
}

```