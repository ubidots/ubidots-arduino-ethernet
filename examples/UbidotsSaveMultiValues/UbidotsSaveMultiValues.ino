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
IPAddress ip(192, 168, 0, 177);

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
      /* Try to congifure using IP address instead of DHCP */
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
