/*
Copyright (c) 2017 Ubidots.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Original Maker: Mateo Velez - Metavix for Ubidots Inc
Modified and Maintened by: María Carlina Hernández ---- Developer at Ubidots Inc
                           https://github.com/mariacarlinahernandez
                           Jose Garcia ---- Developer at Ubidots Inc
                           https://github.com/jotathebest
*/

#include "UbidotsEthernet.h"

/**
 * Constructor.
 * Default deviceName is "arduino-ethernet"
 */
Ubidots::Ubidots(const char * token, const char * server) {
  _token = token;
  _server = server;
  maxValues = 5;
  currentValue = 0;
  _deviceLabel = DEFAULT_DEVICE_LABEL;
  val = (Value *)malloc(maxValues*sizeof(Value));
}

/***************************************************************************
FUNCTIONS TO RETRIEVE DATA
***************************************************************************/
/**
 * This function is to get last value from the Ubidots API with the device label
 * and variable label
 * @arg device_label is the label of the device
 * @arg variable_label is the label of the variable
 * @return num the data that you get from the Ubidots API, if any error occurs
    the function returns ERRORs messages
 */
float Ubidots::getValue(char* device_label, char* variable_label) {
  /* Assigns the constans as global on the function */
  bool flag = false;
  char* res = (char *) malloc(sizeof(char) * 250);
  char* value = (char *) malloc(sizeof(char) * 20);
  char* parsed = (char *) malloc(sizeof(char) * 20);
  float num;
  uint8_t index = 0;
  uint8_t l = 0;
  uint8_t timeout = 0;
  uint8_t max_retries = 0;

  if (_debug) {
    Serial.print(F("Connecting to the server..."));
  }

  /* Initial connection */
  _client.connect(SERVER, PORT);

  /* Reconnect the client when is disconnected */
  while (!_client.connected()) {
    if (_debug) {
      Serial.println(F("Attemping to connect"));
    }

    if (_client.connect(SERVER, PORT)) {
      break;
    }

    max_retries++;
    if (max_retries > 5) {
      if (_debug) {
        Serial.println(F("Could not connect to server"));
      }
      return ERROR_VALUE;
    }
    delay(5000);
  }

  if (_debug) {
    Serial.println(F("Connected!"));
    Serial.println(F("Sending the GET Request..."));
  }
  /* Make the HTTP request to the server*/
  _client.print(F("GET /api/v1.6/devices/"));
  _client.print(device_label);
  _client.print(F("/"));
  _client.print(variable_label);
  _client.println(F("/lv HTTP/1.1"));
  _client.println(F("Host: things.ubidots.com"));
  _client.print(F("X-Auth-Token: "));
  _client.println(_token);
  _client.println(F("Connection: close"));
  _client.println();

  /* Reach timeout when the server is unavailable */
  while (!_client.available() && timeout < 2000) {
    timeout++;
    delay(1);
    if (timeout >= 2000) {
      if (_debug) {
        Serial.println(F("Error, max timeout reached"));
      }
      _client.stop();
      return ERROR_VALUE;
    }
  }

  /* Reads the response from the server */
  while (_client.connected()) {
    while (_client.available()) {
      char c = _client.read();
      Serial.write(c);
      if (c == -1) {
        if (_debug) {
          Serial.println(F("Error reading data from server"));
        }
        _client.stop();
        return ERROR_VALUE;
      }
      res[l++] = c;
    }
  }

  /* Extracts the string with the value */
  /* Expected result string : {length_of_request}\r\n{value}\r\n{length_of_answer} */

  int len = dataLen(res); // Length of the answer char array from the server

  for (int i = 0; i < len - 4; i++) {
    if ((res[i] == '\r') && (res[i+1] == '\n') && (res[i+2] == '\r') && (res[i+3] == '\n')) {
        strncpy(parsed, res+i+3,20);  // Copies the result to the parsed
        parsed[20] = '\0';
        break;
    }
  }

  /* Extracts the the value */

  for (int k = 0; k < 20; k++){
    if ((parsed[k] == '\r') && (parsed[k+1] == '\n')) {
      while((parsed[k+2] != '\r')){
        value[index] = parsed[k+2];
        k++;
        index++;
      }
      break;
    }
  }

  /* Converts the value obtained to a float */
  num = atof(value);
  /* Free memory*/
  free(res);
  free(value);
  free(parsed);
  /* Removes any buffered incoming serial data */
  _client.flush();
  /* Disconnects the client */
  _client.stop();
  return num;
}

/***************************************************************************
FUNCTIONS TO SEND DATA
***************************************************************************/

 /**
  * Add a value of variable to save
  * @arg variable_label variable label to save in a struct
  * @arg value variable value to save in a struct
  * @arg ctext [optional] is the context that you will save, default
  * @arg timestamp_val [optional] is the timestamp for the actual value
  * is NULL
  */
void Ubidots::add(const char * variable_label, double value) {
  return add(variable_label, value, '\0', '\0');
}

void Ubidots::add(const char * variable_label, double value, char* ctext) {
  return add(variable_label, value, ctext, '\0');
}

void Ubidots::add(const char * variable_label, float value, unsigned long timestamp_val) {
  return add(variable_label, value, '\0', timestamp_val);
}

void Ubidots::add(const char * variable_label, double value, char* ctext, unsigned long timestamp_val ) {
  (val+currentValue)->varLabel = variable_label;
  (val+currentValue)->varValue = value;
  (val+currentValue)->context = ctext;
  (val+currentValue)->timestamp_val = timestamp_val;
  if (currentValue>maxValues) {
    Serial.println(F("You are sending more than 5 consecutives variables, you just could send 5 variables. Then other variables will be deleted!"));
    currentValue = maxValues;
  }
  currentValue++;
}

/**
 * Assigns a new device label
 * @arg new_device_name new label that you want to assign to your device
 */
void Ubidots::setDeviceLabel(const char * new_device_label) {
    _deviceLabel = new_device_label;
}

/**
 * Send all data of all variables that you saved
 * @reutrn true upon success, false upon error.
 */
bool Ubidots::sendAll() {
  /* Assigns the constans as global on the function */
  uint8_t max_retries = 0;
  uint8_t timeout = 0;
  uint8_t i = 0;
  String str;

  /* Verify the variables invoked */
  if (currentValue == 0) {
    Serial.println(F("Invoke a variable to be send using the method \"add\""));
    return false;
  }

  /* Builds the JSON to be send */
  char* body = (char *) malloc(sizeof(char) * 150);
  sprintf(body, "{");
  for (i = 0; i < currentValue;) {
    /* Saves variable value in str */
    str = String(((val+i)->varValue),3); // variable value
    sprintf(body, "%s\"%s\":", body, (val + i)->varLabel);
    if ((val + i)->context != '\0') {
        sprintf(body, "%s{\"value\":%s, \"context\":{%s}}", body, str.c_str(), (val + i)->context);
    } else if ((val + i)-> timestamp_val != '\0') {
      sprintf(body, "%s{\"value\":%s, \"timestamp\":%lu}", body, str.c_str(), (val + i)->timestamp_val);
    } else {
      sprintf(body, "%s%s", body, str.c_str());
    }
    i++;
    if (i < currentValue) {
      sprintf(body, "%s, ", body);
    }
  }
  sprintf(body, "%s}", body);

  if (_debug) {
    Serial.print("Connecting to the server...");
  }

  /* Initial connection */
  _client.connect(SERVER, PORT);

  /* Reconnect the client when is disconnected */
  while (!_client.connected()) {
    if (_debug) {
      Serial.println(F("Attemping to connect"));
    }

    if (_client.connect(SERVER, PORT)) {
      break;
    }

    max_retries++;
    if (max_retries > 5) {
      if (_debug) {
        Serial.println(F("Could not connect to server"));
      }
      return ERROR_VALUE;
    }
    delay(5000);
  }

  if (_debug) {
    Serial.println(F("Connected!"));
    Serial.println(F("Sending the POST Request..."));
  }
  /* Make the HTTP request to the server*/
  _client.print(F("POST /api/v1.6/devices/"));
  _client.print(_deviceLabel);
  _client.println(F(" HTTP/1.1"));
  _client.println(F("Host: things.ubidots.com"));
  _client.print(F("User-Agent: "));
  _client.print(USER_AGENT);
  _client.print(F("/"));
  _client.println(VERSION);
  _client.print(F("X-Auth-Token: "));
  _client.println(_token);
  _client.println(F("Connection: close"));
  _client.println(F("Content-Type: application/json"));
  _client.print(F("Content-Length: "));
  _client.println(dataLen(body));
  _client.println();
  _client.print(body);
  _client.println();

  /* Reach timeout when the server is unavailable */
  while (!_client.available() && timeout < 5000) {
    timeout++;
    delay(1);
    if (timeout >= 5000) {
      if (_debug) {
        Serial.println(F("Error, max timeout reached"));
      }
      _client.stop();
      return ERROR_VALUE;
    }
  }

  /* Reads the response from the server */
  while (_client.connected()) {
    while (_client.available()) {
      char c = _client.read();
      Serial.write(c);
      if (c == -1) {
        if (_debug) {
          Serial.println(F("Error reading data from server"));
        }
        _client.stop();
        return ERROR_VALUE;
      }
    }
  }

  currentValue = 0;
  /* free memory */
  free(body);
  /* Removes any buffered incoming serial data */
  _client.flush();
  /* Disconnects the client */
  _client.stop();
  return true;
}

/***************************************************************************
AUXILIAR FUNCTIONS
***************************************************************************/

/**
* Turns on or off debug messages
* @debug is a bool flag to activate or deactivate messages
*/
void Ubidots::setDebug(bool debug) {
     _debug = debug;
}

/**
 * Gets the length of a variable
 * @arg variable a variable of type char
 * @return dataLen the length of the variable
 */
int Ubidots::dataLen(char* variable) {
  uint8_t dataLen = 0;
  for (int i = 0; i <= 250; i++) {
    if (variable[i] != '\0') {
      dataLen++;
    } else {
      break;
    }
  }
  return dataLen;
}

/**
* Verify if the client is connected
*/
bool Ubidots::connected() {
  return _client.connected();
}

/**
* Connect the client
*/
bool Ubidots::connect(const char * server, int port) {
  _server = server;
  _port = port;
  return _client.connect(server, port);
}
