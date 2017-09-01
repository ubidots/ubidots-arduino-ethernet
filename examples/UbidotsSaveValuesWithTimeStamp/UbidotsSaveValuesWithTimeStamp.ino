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
char const * VARIABLE_LABEL = "temperature"; // Assign the unique variable label to send the data

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
  while (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    /* Try to configure using IP address instead of DHCP */
    Ethernet.begin(mac, ip);
    delay(5000);
  }
  /* Give the Ethernet shield a second to initialize */
  delay(1000);
}

void loop() {
  /* If no connected, attemps to reconnect */
  if (!client.connected()) {
    /* Start the Ethernet connection */
    while (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet using DHCP");
      /* Try to configure using IP address instead of DHCP */
      Ethernet.begin(mac, ip);
      delay(5000);
    }
    /* Connect the client */
    client.connect();
  }

  /* Sensors readings */
  float value = analogRead(A0);
  unsigned long timestamp = 1464661369000;
  /* Sending values to Ubidots */
  client.add(VARIABLE_LABEL, value, "", timestamp);
  client.sendAll();
  delay(5000);
}
