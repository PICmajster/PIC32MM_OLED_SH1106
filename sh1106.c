/*
    File Name:        :  sh1106.c

    Device            :  PIC32MM0256GPM048
    Compiler          :  XC32 2.05
    MPLAB             :  MPLAB X 4.15
    Created by        :  http://strefapic.blogspot.com
*/
#include "mcu_config_files/mcc.h"
#include "delay.h"
#include "sh1106.h"
#include "font5.h"
#include "font6R.h"
#include "font22.h"

// SSD1307:
// StartCol = 0;
// DisplayRAMColSize = 128;
// SH1106:
// StartCol = 2;
// DisplayRAMColSize = 132;

void sendCmd(uint8_t cmdOut)
{ // Write a command to Display
   DisplayD_C = 0 ;     // set A0 low, select Command mode DisplayD_C = 0 for this byte
   writeSD(cmdOut) ;    // Clock out command bits
   DisplayD_C = 1 ;     // set A0 high, select Data mode DisplayD_C = 1 for next ...
}

void sendData(uint8_t dataOut)
{ // Write data to Display, DisplayD_C = 1 set after sendCmd()
  //  DisplayD_C = 1 ;     // set A0 high, select Data mode DisplayD_C = 1
  writeSD(dataOut) ;   // Clock out data bits
}

void writeSD(uint8_t byteOut)
{ // Clocks out data to the SSD1106 controller
  uint8_t bitcnt ;
  for (bitcnt = 8 ; bitcnt > 0 ; bitcnt--)
  {
    DisplayCLK = 0 ;           // Set clock low
    DisplaySDI = (byteOut & 80 == 0x80) ? 1 : 0 ;  // transfer current bit 7 of byteOut to DisplaySDI pin
    DisplayCLK = 1 ;           // clock data bit on the rising edge of DisplayCLK
    byteOut = byteOut << 1 ;   // logical shift left: get next bit to b7
  }
}