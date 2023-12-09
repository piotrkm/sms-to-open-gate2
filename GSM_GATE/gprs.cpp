#include "Arduino.h"
#include "gprs.h"


  String gprs_init_list [] = {
    "AT+SAPBR=3,1,\"Contype\",\"GPRS\"",
    "AT+SAPBR=3,1,\"APN\",\"internet\"",
    "AT+SAPBR=1,1",
    "AT+SAPBR=2,1",
    "AT+HTTPINIT",
    "AT+HTTPPARA=\"CID\",1",
    "AT+HTTPPARA=\"URL\",\"pkmita.vxm.pl/date.php\"",
    "AT+HTTPACTION=0",
    "AT+SAPBR=0,1",
    "AT+HTTPREAD",
    "AT+HTTPTERM"
  };



gprs::gprs()
{
  String incoming_message;
  set_verbose(true);
  for(size_t i{0}; i < 11; i++ )
   {
       send_command(gprs_init_list[i].c_str());
       delay(1500);
       while(serial_available())
         incoming_message = readFromGsm();
    
       Serial.print(i);
       Serial.println(" ");
       Serial.println(incoming_message);
       
   }

}