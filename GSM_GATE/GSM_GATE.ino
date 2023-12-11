// #########################################################################################
//   __  ___  ___  _  _     __   __  ____  ___    ___  _  _    ___  __  __  ___ 
//  /  \(  ,\(  _)( \( )   / _) (  )(_  _)(  _)  (  ,)( \/ )  / __)(  \/  )/ __)
// ( () )) _/ ) _) )  (   ( (/\ /__\  )(   ) _)   ) ,\ \  /   \__ \ )    ( \__ \
//  \__/(_)  (___)(_)\_)   \__/(_)(_)(__) (___)  (___/(__/    (___/(_/\/\_)(___/
//
// #########################################################################################


#include "gsm.h"
#include "message.h"
#include "gate.h"
#include "temperature.h"

#define LED 13          // The pin 13 associated to the onboard LED
#define VERBOSE true    // Set the verbose mode;
                        // Initialise the verbose mode to (true = ON), (false = OFF)


gsmClass gsm {VERBOSE};        // Establish the gsm object
                               
gate GateObj {}; 

temperature TempObj {};
                        
String number;
                               
                               
// ################################################################
// 
//  Setup section 
//
// ################################################################
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);                                               // Start the hardware serial
  Serial.println();
  delay(1000);                                                      // Wait a second
  pinMode(LED, OUTPUT);                                             // Initialise the onboard LED
  
  if(gsm.get_verbose())
    Serial.println("\n### [       Program started !         ###]");   
    gsm.flush_to_serial();
    delay(500);
    gsm.initialize();  
} // END OF _SETUP_




// ################################################################
// 
//  Main loop section 
//
// ################################################################
void loop() {
    String incoming_message {};
    
    while(gsm.serial_available())
    {
      incoming_message = gsm.readFromGsm();
    } 
    
    if(incoming_message.substring(0, 5) == "+CLIP" )
      Serial.println("[###   SOMEONE IS CALLING !!!!    ###]");
    
     
    // =====================
    // INCOMING TEXT MESSAGE
    // =====================
    if(incoming_message.substring(0, 5) == "+CMT:" )
    {
      Serial.println("[###   SOMEONE SENT A TEXT !!!    ###]");
      
      message mes(incoming_message);
      String second_line {};
      while(gsm.serial_available())
        {   
          second_line = gsm.readFromGsm();
          mes.set_text(second_line);
        }
      
      if(VERBOSE)
        {
          Serial.print("SMS was sent from number: ");
          Serial.flush();
          Serial.println(mes.get_number());
          Serial.flush();
          Serial.print("Text of the message: ");
          Serial.flush();
          Serial.println(mes.get_text());
          Serial.flush();
        }
      
      
      // =====================
      // OPEN COMMAND
      // =====================
      if(mes.text_contains("OPEN"))
        {
          
          GateObj.open_gate();  
          number = mes.get_number();

          if(VERBOSE)
            {
              Serial.println("Gate is going to open");
              Serial.println(number);
            }
        }  // _END OF OPEN COMMAND_

      // =====================
      // CLOSE COMMAND
      // =====================
      if(mes.text_contains("CLOSE"))
        {
          GateObj.close_gate();  
          number = mes.get_number();
          
          if(VERBOSE)
            {
              Serial.println("Gate is going to close");
              Serial.println(number);
            }
        } // _END OF CLOSE COMMAND_
      

      // =====================
      // STATUS COMMAND
      // =====================
      if(mes.text_contains("STATUS"))
        {
          number = mes.get_number();
          String status = GateObj.get_status();
          
          if(VERBOSE)
            {
              Serial.println("Gate status requested");
              Serial.println(status);
            }
          
        } // _END OF STATUS REQUEST_

      // =====================
      // TEMPERATURE COMMAND
      // =====================
      if(mes.text_contains("TEMP")) 
        {
          number = mes.get_number();
          
          String temp_val {};
          temp_val = "Current temperature: " + String(TempObj.get_t()) + " C";
          gsm.send_sms(temp_val.c_str(), number.c_str());
          
        }

      // =====================
      // GPRS REQUEST
      // =====================
      if(mes.text_contains("gprs")) 
        {
          number = mes.get_number();
          String temperature {};
          temperature = String(TempObj.get_t());
          // INITIALIZE GPRS
          String gprs_initial_list [6] = {
            "AT+SAPBR=3,1,\"Contype\",\"GPRS\"",
            "AT+SAPBR=3,1,\"APN\",\"internet\"",
            "AT+SAPBR=1,1",
            "AT+SAPBR=2,1",
            "AT+HTTPINIT",
            "AT+HTTPPARA=\"CID\",1"}; 

            String gprs_request = "AT+HTTPPARA=\"URL\",\"pkmita.vxm.pl/temperature.php?temp=" + temperature + "\"";

          String gprs_terminate_list [4] = {
            "AT+HTTPACTION=0",
            "AT+SAPBR=0,1",
            "AT+HTTPREAD",
            "AT+HTTPTERM"};

          Serial.println("###  INITIALIZE GPRS ###");
          for(size_t i {0}; i<= 5; i++)
          {
            Serial.print("# No:");
            Serial.print(i);
            Serial.print("\t");
            Serial.println(gprs_initial_list[i].c_str());
            gsm.send_command(gprs_initial_list[i].c_str());
            delay(1500);
            String m {};
            m = gsm.readFromGsm();
            Serial.println(m);

          }
          Serial.println("###  INITIALIZATION COMPLETED ###");
          Serial.println("###  SENDING REQUEST ###");

          Serial.print("#GPRS REQUEST: ");
          Serial.println(gprs_request.c_str());
          gsm.send_command(gprs_request.c_str());
          delay(1500);
          while(gsm.serial_available())
          Serial.println(gsm.readFromGsm());

          
          
          //gsm.send_command("AT+HTTPPARA=\"URL\",\"pkmita.vxm.pl/temperature.php?temp=10.00\"");

          
          Serial.println("###  TERMINATION GPRS ###");

          for(size_t i {0}; i<= 3; i++)
          {
            Serial.print("# No:");
            Serial.print(i);
            Serial.print("\t");
            Serial.println(gprs_terminate_list[i].c_str());
            gsm.send_command(gprs_terminate_list[i].c_str());
            delay(1500);
            while(gsm.serial_available())
            Serial.println(gsm.readFromGsm());

          }
          Serial.println("###  TERMINATION ENDED UP ###");

        }

        
    } // _END OF_INCOMMING_MESSAGE_
    

      // =======================
      // CHECKING LIMITS SWITCHES
      // =========================
      GateObj.check_limit_switch();
      
      if (GateObj.get_message() !="")
        {
          String temp_mess = GateObj.get_message();
          Serial.println("CONFIRMATION: ");
          Serial.println(temp_mess.c_str());
          Serial.println(number.c_str());
          Serial.println("CONFIRMATION END");
          gsm.send_sms(temp_mess.c_str(), number.c_str());
          GateObj.delete_message();
          
        }
     
    
  
    
    
    
  // =========================
  //
  // GPRS CONNECTION
  //
  // =========================
  
  // String list [] = {
  //   "AT+SAPBR=3,1,\"Contype\",\"GPRS\"",
  //   "AT+SAPBR=3,1,\"APN\",\"internet\"",
  //   "AT+SAPBR=1,1",
  //   "AT+SAPBR=2,1",
  //   "AT+HTTPINIT",
  //   "AT+HTTPPARA=\"CID\",1",
  //   "AT+HTTPPARA=\"URL\",\"pkmita.vxm.pl/date.php\"",
  //   "AT+HTTPACTION=0",
  //   "AT+SAPBR=0,1",
  //   "AT+HTTPREAD",
  //   "AT+HTTPTERM"
  // };
  
  // for(size_t i{0}; i < 11; i++ )
  //   {
  //    
  //     gsm.send_command(list[i].c_str());
  //     delay(1500);
  //     while(gsm.serial_available())
  //       incoming_message = gsm.readFromGsm();
  //  
  //     Serial.println(incoming_message);
  //    
  //   }
  

  

    
    
    
    
} // END OF _LOOP_
