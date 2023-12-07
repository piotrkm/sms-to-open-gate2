#ifndef _MESSAGE_H_
#define _MESSAGE_H_


class message {

public: 
  message(const String &content); 
  ~message() {};
  char* get_number();
  char* get_text();
  void set_text(const String &content);
  bool text_contains(const String &content);



private: 
    String number {};
    String text {};

};

#endif