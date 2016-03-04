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
