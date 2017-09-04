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
  char* res = (char *) malloc(sizeof(char) * 250);
  char* response;
  char* serverResponse;
  float num;
  uint8_t i = 0;
  uint8_t j = 0;
  uint8_t timeout = 0;
  uint8_t max_retries = 0;

  /* Builds the request GET - Please reference this link to know all the request's structures https://ubidots.com/docs/api/ */
  char* data = (char *) malloc(sizeof(char) * 220);
  sprintf(data, "GET /api/v1.6/devices/%s/%s/lv", device_label, variable_label);
  sprintf(data, "%s HTTP/1.1\r\n", data);
  sprintf(data, "%sHost: things.ubidots.com\r\n", data);
  sprintf(data, "%sUser-Agent: %s/%s\r\n", data, USER_AGENT, VERSION);
  sprintf(data, "%sX-Auth-Token: %s\r\n", data, _token);
  sprintf(data, "%sConnection: close\r\n\r\n", data);

  /* Initial connection */
  _client.connect(SERVER,PORT);
  /* Reconnect the client when is disconnected */
  while (!_client.connected()) {
    if (_debug) {
      Serial.println("Attemping to connect");
    }

    if (_client.connect(SERVER, PORT)) {
      break;
    }

    max_retries++;
    if (max_retries > 5) {
      if (_debug) {
        Serial.println("Could not connect to server");
      }
      free(data);
      return ERROR_VALUE;
    }
    delay(5000);
  }

  if (_debug) {
    Serial.println(F("Getting your variable with request: \r\n"));
    Serial.println(data);
  }

  /* Make the HTTP request to the server*/
  _client.print(data);
  /* Reach timeout when the server is unavailable */
  while (!_client.available() && timeout < 2000) {
    timeout++;
    delay(1);
    if (timeout >= 2000) {
      if (_debug) {
        Serial.println(F("Error, max timeout reached"));
      }
      _client.stop();
      free(data);
      return ERROR_VALUE;
    }
  }

  /* Reads the response from the server */
  while (_client.available()) {
    char c = _client.read();
    if (c == -1) {
      if (_debug) {
        Serial.println(F("Error reading data from server"));
      }
      _client.stop();
      free(data);
      return ERROR_VALUE;
    }
    res[i++] = c;
  }

  /* Parses the response to get just the last value received */
  response = strtok(res, "\r\n");
  while (response!=NULL) {
    j++;
    printf("%s", response);
    response = strtok(NULL, "\r\n");
    /* Saves the last value obtained in a new variable */
    if (j == 10) {
      if (response != NULL) {
        serverResponse = response;
      }
      j = 0;
    }
  }
  /* Converts the value obtained to a float */
  num = atof(serverResponse);
  /* Free memory*/
  free(data);
  free(res);
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

void Ubidots::add(const char * variable_label, double value, char* ctext, long unsigned timestamp_val ) {
  (val+currentValue)->varLabel = variable_label;
  (val+currentValue)->varValue = value;
  (val+currentValue)->context = ctext;
  (val+currentValue)->timestamp_val = timestamp_val;
  currentValue++;
  if (currentValue>maxValues) {
    Serial.println(F("You are sending more than 5 consecutives variables, you just could send 5 variables. Then other variables will be deleted!"));
    currentValue = maxValues;
  }
}

/**
 * Assigns a new device label
 * @arg new_device_name new label that you want to assign to your device
 */
void Ubidots::setDeviceLabel(const char * new_device_label) {
    _deviceLabel = new_device_label;
}

/**
 * Gets the length of the body
 * @arg body data to be send - JSON
 * @return dataLen the length of the JSON
 */
int Ubidots::dataLen(char* body) {
  uint8_t dataLen = 0;
  for (int i = 0; i <= 150; i++) {
    if (body[i] != '\0') {
      dataLen++;
    } else {
      break;
    }
  }
  return dataLen;
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
    Serial.println("Invoke a variable to be send using the method \"add\"");
    return false;
  }

  /* Saves variable value in str */
  str = String(((val+i)->varValue),3); // variable value
  /* Builds the JSON to be send */
  char* body = (char *) malloc(sizeof(char) * 150);
  sprintf(body, "{");
  for (i = 0; i < currentValue;) {
    sprintf(body, "%s\"%s\":", body, (val + i)->varLabel);
    if ((val + i)->context != '\0') {
        sprintf(body, "%s{\"value\":%s, \"context\":{%s}}", body, str.c_str(), (val + i)->context);
    } else if ((val + i)-> timestamp_val != '\0') {
      sprintf(body, "%s{\"value\":%s, \"timestamp\":%s}", body, str.c_str(), (val + i)->timestamp_val);
    } else {
      sprintf(body, "%s%s", body, str.c_str());
    }
    i++;
    if (i < currentValue) {
      sprintf(body, "%s, ", body);
    }
  }
  sprintf(body, "%s}", body);

  /* Builds the request POST - Please reference this link to know all the request's structures https://ubidots.com/docs/api/ */
  char* data = (char *) malloc(sizeof(char) * 300);
  sprintf(data, "POST /api/v1.6/devices/%s", _deviceLabel);
  sprintf(data, "%s HTTP/1.1\r\n", data);
  sprintf(data, "%sHost: things.ubidots.com\r\n", data);
  sprintf(data, "%sUser-Agent: %s/%s\r\n", data, USER_AGENT, VERSION);
  sprintf(data, "%sX-Auth-Token: %s\r\n", data, _token);
  sprintf(data, "%sConnection: close\r\n", data);
  sprintf(data, "%sContent-Type: application/json\r\n", data);
  sprintf(data, "%sContent-Length: %d\r\n\r\n", data, dataLen(body));
  sprintf(data, "%s%s\r\n\r\n", data, body);

  /* Initial connection */
  _client.connect(SERVER,PORT);
  /* Reconnect the client when is disconnected */
  while (!_client.connected()) {
    if (_debug) {
      Serial.println("Attemping to connect");
    }

    if (_client.connect(SERVER, PORT)) {
      break;
    }

    max_retries++;
    if (max_retries > 5) {
      if (_debug) {
        Serial.println("Could not connect to server");
      }
      free(data);
      return ERROR_VALUE;
    }
    delay(5000);
  }

  if (_debug) {
    Serial.println(F("Posting your variable with request: \r\n"));
    Serial.println(data);
  }

  /* Make the HTTP request to the server*/
  _client.println(data);
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
  while (_client.available()) {
    char c = _client.read();
    Serial.write(c);
    if (c == -1) {
      if (_debug) {
        Serial.println(F("Error reading data from server"));
      }
      _client.stop();
      free(data);
      return ERROR_VALUE;
    }
  }

  currentValue = 0;
  /* free memory */
  free(data);
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
