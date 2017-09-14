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

#ifndef _UbidotsEthernet_H_
#define _UbidotsEthernet_H_
#include <SPI.h>
#include <Ethernet.h>

namespace {
  const char * DEFAULT_DEVICE_LABEL = "arduino-ethernet";
  const char * SERVER = "things.ubidots.com";
  const char * USER_AGENT = "Arduino-Ethernet";
  const char * VERSION = "2.0";
  const int PORT = 80;
  const float ERROR_VALUE = -3.4028235E+8;
}

typedef struct Value {
  const char * varLabel;
  char *context;
  double varValue;
  unsigned long timestamp_val;
} Value;

class Ubidots {
  public:
    bool sendAll();
    explicit Ubidots(const char * token, const char * server = SERVER);
    float getValue(char* device_label, char* variable_label);
    void add(const char * variable_label, double value);
    void add(const char * variable_label, double value, char* ctext);
    void add(const char * variable_label, float value, unsigned long timestamp_val);
    void add(const char * variable_label, double value, char* ctext, unsigned long timestamp);
    void setDebug(bool debug);
    void setDeviceLabel(const char * new_device_label);
    bool connected();
    bool connect(const char * server = SERVER, int port = PORT);

  private:
    bool _debug = false;
    const char * _deviceLabel;
    const char * _token;
    const char * _server;
    int _port;
    int dataLen(char* variable);
    uint8_t maxValues;
    uint8_t currentValue;
    Value * val;
    EthernetClient _client;
};

#endif
