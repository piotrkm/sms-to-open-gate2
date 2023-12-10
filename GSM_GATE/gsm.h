#ifndef _GSM_H_
#define _GSM_H_

#define RXpin 2
#define TXpin 3

#include "SoftwareSerial.h"


//SoftwareSerial mySerial = SoftwareSerial(rxPin, txPin);

class gsmClass {
public:
  gsmClass();
  gsmClass(bool);
  void send_sms(const char *text, const char *number);
  void initialize();
  void send_command(const char *text);
  bool serial_available();
  String readFromGsm();   
  void flush_to_serial();
  void set_verbose(bool);
  bool get_verbose();

private:
  bool verbose {false};


  SoftwareSerial gsmSerial = SoftwareSerial(RXpin, TXpin);
  
};



#endif