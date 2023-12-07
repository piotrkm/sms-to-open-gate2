#include <Arduino.h>
#include "temperature.h"

float temperature::get_t()
   {
     unsigned int temp {0};
     temp = analogRead(TEMPERATURE_SENSOR);
     return (temp*500.0/1023.0);
   } 