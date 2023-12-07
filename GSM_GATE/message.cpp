#include <Arduino.h>
#include "message.h"


/*
templete text message 
+CMT: "+447843780000","","23/11/05,14:41:11+00"
SMS content
*/


message::message(const String &content)
{
      String temp = content;
      size_t i {7};
      for(; temp.charAt(i) != '"'; i++ )
        number += temp.charAt(i);
};

void message::set_text(const String &content)
  {
    text = content;
  }

char* message::get_number()
  {
    return number.c_str();
  }

char* message::get_text()
  {
    return text.c_str();
  }
  
bool message::text_contains(const String &content)
  {
    String temp = text;
    content.toLowerCase();
    temp.toLowerCase();
    if(-1 != temp.indexOf(content))
      {
        return true;
      } else {
        return false;
      }

  }