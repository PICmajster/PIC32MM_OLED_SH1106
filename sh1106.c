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

void initDisplay(void)
{
  DisplayCS = 1 ;  
  delayMs(25) ;     // minimum 10uS, oryginalnie Felek dal 20ms
  sendCmd(0xAE) ;   // turn off OLED panel                                 [#11]
  sendCmd(0xC8) ;   // set COM scan direction 0-n [POR = C0, C8]           [#13]
  sendCmd(0xA1) ;   // set SEG/Column left/right re-map [POR = A0, A1]     [#6]
//  sendCmd(0xA4) ; // Entire Display Force On [POR = A4, A5 force on]     [#7]
  clsDisplay() ;    // clear RAM data and zero cursor
  setContrast(128) ;// POR value = 0x80
  sendCmd(0xAF) ;   // turn on OLED panel                                  [#11]
  delayMs(100) ;    // at least 100mS after 0xAF command
}

void sendCmd(uint8_t cmdOut)
{ // Write a command to Display
   DisplayD_C = 0 ;   // set A0 low, select Command mode DisplayD_C = 0 for this byte
   DisplayCS = 0 ;
   writeSD(cmdOut) ;  // Clock out command bits
   }

void sendData(uint8_t dataOut)
{ // Write data to Display, DisplayD_C = 1 set after sendCmd()
  DisplayD_C = 1 ;     // set A0 high, select Data mode DisplayD_C = 1
  DisplayCS = 0 ;
  writeSD(dataOut) ;   // Clock out data bits
}

void writeSD(uint8_t byteOut)
{ // Clocks out data to the SSD1106 controller
  uint8_t bitcnt ;
  DisplayCS = 0 ;
  DisplayCLK = 0 ;           
  delayUs(1);
  for (bitcnt = 8 ; bitcnt > 0 ; bitcnt--)
  {
    DisplaySDI = (byteOut&0x80) ? 1 : 0 ;  // transfer current bit 7 of byteOut to DisplaySDI pin
    delayUs(1);
    DisplayCLK = 1 ; 
    delayUs(1);
    DisplayCLK = 0 ;
    delayUs(1);
    byteOut = byteOut << 1 ;   // logical shift left: get next bit to b7
  }
  DisplayCLK = 0 ;
  delayUs(1);
  DisplayCS = 1 ;
  
}

void setContrast(unsigned char contr)
{ 
  sendCmd(0x81) ;    // set contrast control register                       [#5]
  sendCmd(contr) ;   //
}

void onDisplay(void)
{
  sendCmd(0xAF) ;                     // turn on OLED panel
}

void offDisplay(void)
{
  sendCmd(0xAE) ;                     // turn off OLED panel
}

void clLnDisplay(uint8_t yline)
{ // Clear single line 0-7 on display, cursor to line start
  uint8_t ram ;
  cursorXY(0, yline)    ;            // Cursor Home
  for (ram=sWidth ; ram>0 ; ram--)   // sWidth DDRAM addresses in each line
  { sendData(0); }                   // clear RAM
  cursorXY(0, yline) ;               // Cursor Home
}

void clsDisplay(void)
{ // Erase the entire display DDRAM and set cursor home (0,0)
  uint8_t line;
  offDisplay() ;                      // turn off OLED panel
  for (line = 8 ; line > 0 ; line--)  // 8 lines of Display DDRAM
  { clLnDisplay(8 - line) ; }         // erase each in turn, home cursor to line
  onDisplay() ;                       // turn on OLED panel
}

void cursorY(unsigned char add)
{ //Set page address for Page Addressing Mode
  sendCmd(0xB0 | add) ;
}

void cursorX(unsigned char add)
{ //Set column adress for Page Addressing Mode
  add = add+sXOffset ;
  sendCmd(0x10 | (add>>4)) ; //  shift high 4
  sendCmd(0x0F & add) ;      // low 4
}

void cursorXY(uint8_t x, uint8_t y)
{ // Position cursor to column, line
  cursorY(y) ; // Y axis = line 0-7
  cursorX(x) ; // X axis = column 0 - (sWidth - 1)
}

void strFont5(uint8_t s[])
{ // Write Variable or Contatant string to Font5
  while(*s) {charFont5(*s++);} // Points to character, or terminator
}

void strFont5XY(uint8_t s[], uint8_t x, uint8_t y)
{ // Write (V or C) string to Font5 @ XY
  cursorXY(x,y);
  strFont5(s);
}
void sendGlyphs(const uint8_t *pRow, uint8_t size)
{ //Send any font row to print character
  uint8_t count ;
  for (count = 0 ; count < size ; count++)  // Send row to print the character
  {
  sendData((pRow[count])) ;
  }
  sendData(0x00);  // 1 pixel horizontal space after character
}

// Font5 functions (5x8)
void charFont5(uint8_t character) // MODIFY IF FONT RANGE CHANGED
{ // Prepare to write byte to Font5, via sendGlyphs
  if (character == ' ')  // munge ' ' to empty ';' character
   { character = ';' ; }
  if ((character<'.')||(character>'Z')) // reduced font5 table, CAPS only
   { return ; }    // Exit function if character out of ASCII range
  //Index to row of the character in font5 table, then send glyphs
  sendGlyphs(font5[character - '.'], sizeof font5[0]);
}

