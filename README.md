# Ubidots-arduino-ethernet

Ubidots-Ethernet is an Arduino library for interacting with Ubidots through its API with The Arduino Ethernet Shield.

**IMPORTANT NOTE**: This library was tested using a shield with W5100 chip Ethernet controller.

## Requiremets

* [Arduino UNO](https://www.arduino.cc/en/Main/ArduinoBoardUno)
* [Arduino Ethernet Shield](https://www.arduino.cc/en/Main/ArduinoEthernetShield)
* [Arduino IDE 1.6.5 or higher](https://www.arduino.cc/en/Main/Software)
* [UbidotsArduino Ethernet library](https://github.com/ubidots/ubidots-arduino-ethernet/archive/master.zip)

## Setup

1. Download the UbidotsArduino library [here](https://github.com/ubidots/ubidots-arduino-ethernet/archive/master.zip)
2. Install the library. Go to the Arduino IDE, click on **Sketch -> Include Library -> Add .ZIP Library**
3. Select the .ZIP file of UbidotsEthernet and then "Accept" or "Choose"
4. Close the Arduino IDE and open it again.

## Test your Ethernet Shield

Before start with your Ethernet Shield it is important verify if the hardware is working properly. For this, we highly recommend you make the sample test provided by Arduino.

 Go to **Sketch -> Examples -> Ethernet** and select the **"WebClient"** example.

If everything is working properly, your device is ready to work with Ubidots.

**IMPORTANT NOTE**: We can't guarantee that it will work with different Ethernet modules.


## Change the default name

The library will create a new Ubidots device named **"Arduino-Ethernet"**, also assigns the same name for the device label. if you desire to assign a differente device label, please add to your `setup()` function the line below:

> client.setDeviceLabel("my-new-device")

## Send values to Ubidots

To send values to Ubidots, go to **Sketch -> Examples -> ubidots-arduino-ethernet library** and select the **"UbidotsSaveValues"** example.

Assign your Ubidots [TOKEN](http://help.ubidots.com/user-guides/find-your-token-from-your-ubidots-account), and the variables labels where is indicated, also the MAC address for your controller.

Once the parameters are assigned, upload the code. Then, open the Serial monitor to check the results.

**NOTE**: If no response is seen, try unplugging your Arduino and then plugging it again. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.

```c++
/********************************
 * Libraries included
 *******************************/
#include <Ethernet.h>
#include <SPI.h>
#include <UbidotsEthernet.h>

/********************************
 * Constants and objects
 *******************************/
/* Assigns the Ubidots parameters */
char const * TOKEN = "Assign_your_Ubidots_TOKEN_here"; // Assign your Ubidots TOKEN
char const * VARIABLE_LABEL_1 = "temperature"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_2 = "humidity"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_3 = "pressure"; // Assign the unique variable label to send the data

/* Enter a MAC address for your controller below */
/* Newer Ethernet shields have a MAC address printed on a sticker on the shield */
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

/* Set the static IP address to use if the DHCP fails to assign */
IPAddress ip(8, 8, 8, 8);

/* initialize the instance */
Ubidots client(TOKEN);

/********************************
 * Main Functions
 *******************************/
void setup() {
    Serial.begin(9600);
    client.setDebug(true);
    //client.setDeviceLabel("my-new-device");// uncomment this line to change the defaul label

    /* start the Ethernet connection */
    if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
      /* Try to configure using IP address instead of DHCP */
      Ethernet.begin(mac, ip);
    }
    /* Give the Ethernet shield a second to initialize */
    delay(1000);
}

void loop() {
  /* Sensors readings */
  float value_1 = analogRead(A0);
  float value_2 = analogRead(A1);
  float value_3 = analogRead(A2);
  /* Sending values to Ubidots */
  client.add(VARIABLE_LABEL_1, value_1);
  client.add(VARIABLE_LABEL_2, value_2);
  client.add(VARIABLE_LABEL_3, value_3);
  client.sendAll();
  delay(5000);
}
```
The library allow you send just **5 variables** per request. But don't worry for that, if you desire send more than 5 variables, go to **Sketch -> Examples -> ubidots-arduino-ethernet library** and select the **"UbidotsSaveMultiValues"** example. Don't forget assign your Ubidots [TOKEN](http://help.ubidots.com/user-guides/find-your-token-from-your-ubidots-account), and the variables labels where is indicated, also the MAC address for your controller.

```c++
/********************************
 * Libraries included
 *******************************/
#include <Ethernet.h>
#include <SPI.h>
#include <UbidotsEthernet.h>

/********************************
 * Constants and objects
 *******************************/
/* Assigns the Ubidots parameters */
char const * TOKEN = "Assign_your_Ubidots_TOKEN_here"; // Assign your Ubidots TOKEN
char const * VARIABLE_LABEL_1 = "temperature"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_2 = "humidity"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_3 = "pressure"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_4 = "speed"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_5 = "current"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_6 = "voltage"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_7 = "light"; // Assign the unique variable label to send the data

/* Enter a MAC address for your controller below */
/* Newer Ethernet shields have a MAC address printed on a sticker on the shield */
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

/* Set the static IP address to use if the DHCP fails to assign */
IPAddress ip(8, 8, 8, 8);

/* initialize the instance */
Ubidots client(TOKEN);

/********************************
 * Main Functions
 *******************************/
void setup() {
    Serial.begin(9600);
    client.setDebug(true);
    //client.setDeviceLabel("my-new-device");// uncomment this line to change the defaul label

    /* start the Ethernet connection */
    if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
      /* Try to configure using IP address instead of DHCP */
      Ethernet.begin(mac, ip);
    }
    /* Give the Ethernet shield a second to initialize */
    delay(1000);
}

void loop() {
  /* Sensors readings */
  float value = analogRead(A0);
  /* Sending values to Ubidots */
  client.add(VARIABLE_LABEL_1, value);
  client.add(VARIABLE_LABEL_2, value);
  client.add(VARIABLE_LABEL_3, value);
  client.add(VARIABLE_LABEL_4, value);
  client.add(VARIABLE_LABEL_5, value);
  client.sendAll();
  /* Sending more than 5 variables */
  client.add(VARIABLE_LABEL_6, value);
  client.add(VARIABLE_LABEL_7, value);
  client.sendAll();
  delay(5000);
}
```


## Get the last value from Ubidots

To get the last value of a variable from Ubidots, go to **Sketch -> Examples -> UbidotsArduino library** and select the **"UbidotsGetValue"** example.

Assign your Ubidots [TOKEN](http://help.ubidots.com/user-guides/find-your-token-from-your-ubidots-account), the device and variable label from where you desire obtain the value, also the MAC address for your controller.

Upload the code, open the Serial monitor to check the results.

**NOTE**: If no response is seen, try unplugging your Arduino and then plugging it again. Make sure the baud rate of the Serial monitor is set to the same one specified in your code.

```c++
/********************************
 * Libraries included
 *******************************/
#include <Ethernet.h>
#include <SPI.h>
#include <UbidotsEthernet.h>

/********************************
 * Constants and objects
 *******************************/
/* Assigns the Ubidots parameters */
char const * TOKEN = "Assign_your_Ubidots_TOKEN_here"; // Assign your Ubidots TOKEN
char const * DEVICE_LABEL = "Assign_device_label_here"; // Assign the unique device label
char const * VARIABLE_LABEL = "Assign_variable_label_here"; // Assign the unique variable label to get the last value

/* Enter a MAC address for your controller below */
/* Newer Ethernet shields have a MAC address printed on a sticker on the shield */
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

/* Set the static IP address to use if the DHCP fails to assign */
IPAddress ip(8, 8, 8, 8);

/* initialize the instance */
Ubidots client(TOKEN);

/********************************
 * Main Functions
 *******************************/
void setup() {
    Serial.begin(9600);
    //client.setDebug(true);// uncomment this line to visualize the debug message
    /* start the Ethernet connection */
    if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
      /* Try to configure using IP address instead of DHCP */
      Ethernet.begin(mac, ip);
    }
    /* Give the Ethernet shield a second to initialize */
    delay(1000);
}

void loop() {
    /* Getting the last value from a variable */
    float value = client.getValue(DEVICE_LABEL, VARIABLE_LABEL);
    /* Print the value obtained */
    Serial.print("the value received is:  ");
    Serial.println(value);
    delay(5000);
}
```
