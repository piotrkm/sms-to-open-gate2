#include <Arduino.h>
#import "gate.h"

gate::gate()
  {
    pinMode(PIN_A, OUTPUT);
    pinMode(PIN_B, OUTPUT);
    digitalWrite(PIN_A, HIGH);
    digitalWrite(PIN_B, HIGH);
    
    pinMode(LIMIT_SW_OPEN, INPUT_PULLUP);
    pinMode(LIMIT_SW_CLOSE, INPUT_PULLUP);
    
    set_status();
    
  }

String gate::get_status()
  {
    String mess {};
    switch (status)
    {
      case gate_open:
        mess = GATE_STATUS_OPEN;
        break;
      case gate_close:
        mess = GATE_STATUS_CLOSED;
        break;
      case gate_in_operation:
        mess = GATE_STATUS_IN_OPERATION;
        break;
      case gate_error:
        mess = GATE_STATUS_ERROR;
        break;        
      default:
        mess = GATE_STATUS_UNKNOWN;
    }
    message = mess;
    return mess;
  }
  
void gate::set_status()
  {
    if(digitalRead(LIMIT_SW_OPEN) == LOW && digitalRead(LIMIT_SW_CLOSE) == HIGH)
      status = gate_open;

    if(digitalRead(LIMIT_SW_OPEN) == HIGH && digitalRead(LIMIT_SW_CLOSE) == LOW)
      status = gate_close;

    if(digitalRead(LIMIT_SW_OPEN) == digitalRead(LIMIT_SW_CLOSE))
      status = gate_unknown;
      
  }
  
void gate::open_gate()
  {
    status = gate_in_operation_opening;
    digitalWrite(PIN_A, HIGH);
    digitalWrite(PIN_B, LOW);
    
  }
  
void gate::close_gate()
  {
    status = gate_in_operation_closing;
    digitalWrite(PIN_A, LOW);
    digitalWrite(PIN_B, HIGH);
    
  }
  
void gate::check_limit_switch()
  {
    // =============
    // IF OPENING IS IN PROGRESS AND REACHES THE LIMIT STOP SWITCH
    // =============
    if (status == gate_in_operation_opening && digitalRead(LIMIT_SW_OPEN) == LOW)
      {
        stop_gate();
        status = gate_open;
        message = "GATE SUCCESSFULLY OPEN";
        
      }

    // =============
    // IF CLOSING IS IN PROGRESS AND REACHES THE LIMIT STOP SWITCH
    // =============
    
    if(status == gate_in_operation_closing && digitalRead(LIMIT_SW_CLOSE) == LOW)
      {
        stop_gate();
        status = gate_close;
        message = "GATE SUCCESSFULLY CLOSED";
      }

    // =============
    // IN OTHER CASE DO NOTHING
    // =============
    
  }

void gate::stop_gate()
  {
     digitalWrite(PIN_A, HIGH);
     digitalWrite(PIN_B, HIGH);     
    }
    
String gate::get_message()
  {
    return message;
  }
void gate::delete_message()
  {
    message = "";
  }

void gate::set_message(String content)
  {
    message = content;
    
  }
