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
  DisplayRST = 0 ;  // reset display (RST is active LOW)
  delayMs(20) ;     // minimum 10uS, oryginalnie Felek dal 25ms
  DisplayRST = 1 ;  // release reset
  sendCmd(0xAE) ;   // turn off OLED panel                                 [#11]
  sendCmd(0xC8) ;   // set COM scan direction 0-n [POR = C0, C8]           [#13]
  sendCmd(0xA1) ;   // set SEG/Column left/right re-map [POR = A0, A1]     [#6]
  sendCmd(0xA4) ;   // Entire Display Force On [POR = A4, A5 force on]     [#7]
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

void sendGlyphs(const uint8_t *pRow, uint8_t size)
{ //Send any font row to print character
  uint8_t count ;
  for (count = 0 ; count < size ; count++)  // Send row to print the character
  {
  sendData((pRow[count])) ;
  }
  sendData(0x00);  // 1 pixel horizontal space after character
}

void rptPix(uint8_t pix, uint8_t blank, uint8_t percent)
{ // repeat pixel across Display for percent - show bar graph
  uint8_t count ;
  int width ;
  width = (sWidth*percent)/100 ; // screen width x fill level
  for (count=0; count<sWidth; count++)  // loop for width
  {
   if ((count <= width)||(count > (sWidth - 2)))
   {  sendData(pix) ; }  // send 'pix' glyph to display
   else
   {  sendData(blank) ;} // send 'blank' glyph to display
  }
}

void barDisplay(uint8_t barG)
{// water bargraph on line 7  
  cursorXY(0, 7) ;
  rptPix(0b11111111, 0b10000001, barG);  // Draw line, then blanks
}

// Display FONTS:
// 5x8 (1-line, full 7-bit ASCII from space)
// 34x48 (6 lines, numerals only, ASCII 48 - 57) Calibri Bold 54

// Display FONTS:
// 5x8 (1-line, full 7-bit ASCII from space)
// 34x48 (6 lines, numerals only, ASCII 48 - 57) Calibri Bold 54

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

void strFont5(uint8_t s[])
{ // Write Variable or Contatant string to Font5
  while(*s) {charFont5(*s++);} // Points to character, or terminator
}

void strFont5XY(uint8_t s[], uint8_t x, uint8_t y)
{ // Write (V or C) string to Font5 @ XY
  cursorXY(x,y);
  strFont5(s);
}

void byteFont5XY(uint8_t num, uint8_t x, uint8_t y)
{ // display byte value at XY
  char txt4[4];
  //ByteToStr(num, txt4);
  sprintf(txt4,"%i",num);
  strFont5XY(txt4, x, y);
}

void intFont5XY(uint16_t num, uint8_t x, uint8_t y)
{ // display int value at XY
  char txt7[7];
  //IntToStr(num, txt7);
  sprintf(txt7,"%i",num);
  strFont5XY(txt7, x, y);
}

void byteFont5(uint8_t num)
{ // display byte value at cursor
  char txt4[4];
  //ByteToStr(num, txt4);
  strFont5(txt4);
}

void digitFont5(uint8_t digit) // send single digit to Display
{
  charFont5(digit + '0');
}

void digitFont5XY(uint8_t digit, uint8_t x, uint8_t y) // send single digit to Display @ x, y
{
  cursorXY(x,y);
  digitFont5(digit) ;
}

// End of Font5 functions

// Font6R functions (34x48) numerals only

void charfont6Ra(uint8_t character)
{ // Prepare top 22 bytes for Display, via sendGlyphs (numerals, space and '.' only)
  //if (character==' ') {character='/';} // Munge ' ' into '/' for blank glyph
  if ((character<'0')||(character>'9'))
  { return ; }    // Exit function if character out of numeric range
  //Index to row of the character in font6R table, then send glyphs
  sendGlyphs(font6Ra[character - '0'], sizeof font6Ra[0]);
}

void charfont6Rb(uint8_t character)
{ // Prepare middle1 22 bytes for Display, via sendGlyphs
  // if (character==' ') {character='/';} // Munge ' ' into '/' for blank glyph
  if ((character<'0')||(character>'9'))
  { return ; }    // Exit function if character out of numeric range
  //Index to row of the character in font6R table, then send glyphs
  sendGlyphs(font6Rb[character - '0'], sizeof font6Rb[0]);
}

void charfont6Rc(uint8_t character)
{ // Prepare middle2 22 bytes for Display, via sendGlyphs
  // if (character==' ') {character='/';} // Munge ' ' into '/' for blank glyph
  if ((character<'0')||(character>'9'))
  { return ; }     // Exit function if character out of numeric range
  //Index to row of the character in font6R table, then send glyphs
  sendGlyphs(font6Rc[character - '0'], sizeof font6Rc[0]);
}

void charfont6Rd(uint8_t character)
{ // Prepare lower 22 bytes for Display, via sendGlyphs
  // if (character==' ') {character='/';} // Munge ' ' into '/' for blank glyph
  if ((character<'0')||(character>'9'))
  { return ; }   // Exit function if character out of numeric range
  //Index to row of the character in font6R table, then send glyphs
  sendGlyphs(font6Rd[character - '0'], sizeof font6Rd[0]);
}

void charfont6Re(uint8_t character)
{ // Prepare lower 22 bytes for Display, via sendGlyphs
  // if (character==' ') {character='/';} // Munge ' ' into '/' for blank glyph
  if ((character<'0')||(character>'9'))
  { return ; }   // Exit function if character out of numeric range
  //Index to row of the character in font6R table, then send glyphs
  sendGlyphs(font6Re[character - '0'], sizeof font6Re[0]);
}

void charfont6Rf(uint8_t character)
{ // Prepare lower 22 bytes for Display, via sendGlyphs
  // if (character==' ') {character='/';} // Munge ' ' into '/' for blank glyph
  if ((character<'0')||(character>'9'))
  { return ; }   // Exit function if character out of numeric range
  //Index to row of the character in font6R table, then send glyphs
  sendGlyphs(font6Rf[character - '0'], sizeof font6Rf[0]);
}

void strfont6Ra(uint8_t s[])
{ // Write (V or C) string to font6Ra
  while(*s) {charfont6Ra(*s++);} // Points to character, or terminator
}

void strfont6Rb(uint8_t s[])
{ // Write (V or C) string to font6Rb
  while(*s) {charfont6Rb(*s++);} // Points to character, or terminator
}

void strfont6Rc(uint8_t s[])
{ // Write (V or C) string to font6Rc
  while(*s) {charfont6Rc(*s++);} // Points to character, or terminator
}

void strfont6Rd(uint8_t s[])
{ // Write (V or C) string to font6Rd
  while(*s) {charfont6Rd(*s++);} // Points to character, or terminator
}

void strfont6Re(uint8_t s[])
{ // Write (V or C) string to font6Rd
  while(*s) {charfont6Re(*s++);} // Points to character, or terminator
}

void strfont6Rf(uint8_t s[])
{ // Write (V or C) string to font6Rd
  while(*s) {charfont6Rf(*s++);} // Points to character, or terminator
}

void strFont6RXY(uint8_t s[], uint8_t x, uint8_t y)
{ // Write (V or C) string to font6R @ XY
  cursorXY(x, y) ;
    strfont6Ra(s) ;
  cursorXY(x, y+1) ;
    strfont6Rb(s) ;
  cursorXY(x, y+2) ;
    strfont6Rc(s) ;
  cursorXY(x, y+3) ;
    strfont6Rd(s) ;
  cursorXY(x, y+4) ;
    strfont6Re(s) ;
  cursorXY(x, y+5) ;
    strfont6Rf(s) ;
}
// End of font6R functions