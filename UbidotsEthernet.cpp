#include "UbidotsEthernet.h"
/**
 * Constructor.
 */
Ubidots::Ubidots(char* token){
    _token = token;
    maxValues = 5;
    currentValue = 0;
    val = (Value *)malloc(maxValues*sizeof(Value));
}
/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::parseValue(String body){
  String rawvalue;
  float num;
  char reply[15];
  uint8_t bodyPosinit;
  uint8_t bodyPosend;
  bodyPosinit = 2 + body.indexOf("\n\n");
  rawvalue = body.substring(bodyPosinit);
  bodyPosinit = 9 + rawvalue.indexOf("\"value\": ");
  bodyPosend = rawvalue.indexOf(", \"timestamp\"");
  rawvalue.substring(bodyPosinit,bodyPosend).toCharArray(reply, 10);
  num = atof(reply); 
  return num;
}
/** 
 * This function is to get value from the Ubidots API
 * @arg id the id where you will get the data
 * @return num the data that you get from the Ubidots API
 */
float Ubidots::getValue(char* id){
  float num;
  String raw;
  _client.connect(SERVER, PORT);
  if (_client.connected()){
        Serial.println(F("Geting your variable"));
        // Make a HTTP request:
        _client.print(F("GET /api/v1.6/variables/"));
        _client.print(id);
        _client.println(F("/values?page_size=1 HTTP/1.1"));
        _client.println(F("Host: things.ubidots.com"));
        _client.println(F("User-Agent: Arduino-Ethernet/1.0")); 
        _client.print(F("X-Auth-Token: "));
        _client.println(_token);
        _client.println(F("Connection: close"));
        _client.println();
    } else {
        Serial.println(F("Connection failed"));  
        currentValue = 0;  
        return NULL;
    }
    while (!_client.available());
    while (_client.available()){
        char c = _client.read();
        raw += c;
        //Serial.write(c);
    }
    _client.stop();
    Serial.println(raw);
    num = parseValue(raw);     
    return num;
}
/**
 * Add a value of variable to save
 * @arg variable_id variable id to save in a struct
 * @arg value variable value to save in a struct
 */
void Ubidots::add(char *variable_id, double value){
  (val+currentValue)->id = variable_id;
  (val+currentValue)->value_id = value;
  currentValue++;
  if(currentValue>maxValues){
    Serial.println(F("You are sending more than 5 consecutives variables, you just could send 5 variables. Then other variables will be deleted!"));
    currentValue = maxValues;
  }
}
/**
 * Send all data of all variables that you saved
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll(){
    int i;
    String all;
    String str;
    char b[3];
    all = "[";
    for(i=0; i<currentValue;){
        str = String(((val+i)->value_id),5);
        all += "{\"variable\": \"";
        all += String((val + i)->id);
        all += "\", \"value\":";
        all += str;
        all += "}";
        i++;
        if(i<currentValue){
            all += ", "; 
        }
    }
    all += "]";
    i = all.length();
    
    if (_client.connect(SERVER, PORT)){
          Serial.println(F("Posting your variables"));
          _client.println(F("POST /api/v1.6/collections/values/?force=true HTTP/1.1"));
          _client.println(F("Host: things.ubidots.com"));
          _client.println(F("User-Agent: Arduino-Ethernet/1.0"));
          _client.print(F("X-Auth-Token: "));
          _client.println(_token);
          _client.println(F("Connection: close"));
          _client.println(F("Content-Type: application/json"));
          _client.print(F("Content-Length: "));
          _client.println(String(i));
          _client.println();
          _client.println(all);
          _client.println();          
    }
    while (!_client.available());
    while (_client.available()){
        char c = _client.read();
        Serial.write(c);
    }
    currentValue = 0;
    _client.stop();
    return true;    
}
