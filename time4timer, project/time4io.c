#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h" 

int getbtns( void ){
  int bt1 = (PORTF >> 1) & 0X1;
  int bt234 = (PORTD >> 4) & 0x0000000e;
  PORTD = 0x0;
  PORTF = 0x0;
  return bt1 | bt234;
}