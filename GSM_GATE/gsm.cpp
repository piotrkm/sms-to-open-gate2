#include <Arduino.h>
#include "gsm.h"

gsmClass::gsmClass()
{
  /*
  Defailt gsmClass constructor
  Set the default verbose mode to false (OFF).
  */
  gsmClass(false);
}

gsmClass::gsmClass(bool set_verbose) 
{ 
  /*
  Initialise THE SOFTWARE SERIAL PORT to GSM module 
  and activete it by sending empty AT command
  */
  gsmSerial.begin(9600);
  gsmSerial.print("AT\r");
  gsmSerial.flush();
  verbose = set_verbose;        // set default verbose mode to NO VERBOSE
  

}


void gsmClass::initialize() 
{ 
  /*
  Initialize basic SMS settings to read text messages as text
  */
    send_command("AT+CMGF=1");          // SMS in text mode
    send_command("AT+CNMI=2,2,0,0,0");  // SMS flush directly to serial port
    send_command("AT+CMGD=0,4");          // Deleted all SMS messages from memory

}


void gsmClass::send_sms(const char *text, const char *number) 
{
  /*
  Send a text message to a receipient
  */
  if(verbose)
    {
      Serial.println("### [sending the recipient number    ] ###");
      Serial.flush();
    }

  gsmSerial.print("AT+CMGS=\"");
  gsmSerial.print(number);
  gsmSerial.println("\"");
  gsmSerial.flush();
  
  if(verbose)
    {
      Serial.println("### [The number of SMS recipiend sent] ###");
      Serial.flush();
      flush_to_serial();
    }
  
  delay(1000);

  if(verbose)
    {
      Serial.println("### [1 second waiting                ] ###");
      Serial.flush();
      flush_to_serial();
    }
  
  gsmSerial.print(text);
  gsmSerial.println((char)26);
  gsmSerial.flush();
  delay(300);
  
  if(verbose)
    {
      flush_to_serial(); 
      Serial.println("\r<EOF>");
    }
  
  delay(1000);
} // END OF send_sms

bool gsmClass::serial_available()
{
  return gsmSerial.available();
}

String gsmClass::readFromGsm()
{
  /*
  Read a single char from THE SOFTWARE SERIAL PORT
  */
  String read = gsmSerial.readStringUntil('\n');
  //delay(300);
  return read;
}

void gsmClass::send_command(const char *text)
{
  /*
  Send a command to GSM
  */
  gsmSerial.println(text);
  delay(300);
  flush_to_serial();
}

void gsmClass::flush_to_serial()
{
  /*
  Write a contenct from the SOFTWARE SERIAL PORT associated with the GSM modlue
  to the HARDWARE SERIAL 
  */
    while(serial_available()) 
      {
        Serial.println(readFromGsm());
        Serial.flush();
      }
  
}

void gsmClass::set_verbose(bool turn) 
{
  /*  
  Set the verbose mode
  */
  verbose = turn;
}

bool gsmClass::get_verbose()
{
  /*  
  Return current verbose status
  */
  return verbose;
}

