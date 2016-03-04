#ifndef _UbidotsEthernet_H_
#define _UbidotsEthernet_H_
#include <SPI.h>
#include <Ethernet.h>


#define SERVER "things.ubidots.com"
#define PORT 80

typedef struct Value {
  char  *id;
  float value_id;
} Value;

class Ubidots {
   public:
      Ubidots(char* token);
      bool sendAll();
      float getValue(char* id);
      void add(char *variable_id, double value);
   
   private:
      char* _token;
      uint8_t maxValues;
      uint8_t currentValue;
      Value * val;
      float parseValue(String body);
      EthernetClient _client; 
};

#endif