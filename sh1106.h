/*
    File Name:        :  sh1106.h

    Device            :  PIC32MM0256GPM048
    Compiler          :  XC32 2.05
    MPLAB             :  MPLAB X 4.15
    Created by        :  http://strefapic.blogspot.com
*/

#ifndef SH1106_H
#define	SH1106_H

#define DisplayD_C   PORTAbits.RA15         
#define DisplaySDI   PORTBbits.RB10        
#define DisplayCS    PORTBbits.RB11
#define DisplayCLK   PORTBbits.RB13 
#define DisplayRST   PORTCbits.RC9 

#define sWidth 128      // screen width
#define sXOffset 2      // x offset


/*Function Prototypes*/

/*Display Internal functions*/
void initDisplay(void) ;     // initialise the display
void sendCmd(uint8_t) ;         // write a command to display
void sendData(uint8_t) ;        // write data to display
void writeSD(uint8_t);          // clock out serial data to the display controller

/*Display Control*/
void onDisplay(void) ;      // turn on OLED panel
void offDisplay(void) ;     // turn off OLED panel
void clsDisplay(void) ;     // erase the display DDRAM and set cursor home (0,0)
void clLnDisplay(uint8_t) ;    // clear line on display, cursor to line start
void cursorXY(uint8_t, uint8_t) ; // position cursor to column, line
void setContrast(uint8_t) ;    // set contrast

/*Display*/
void sendGlyphs(const uint8_t *pRow, uint8_t size); //Send any font row to print character
void rptPix(uint8_t, uint8_t, uint8_t) ; // repeat pixel across Display - show bar graph
void barDisplay(uint8_t) ;         // display bar graph
void charFont5(uint8_t);           // Write byte in font 5, via sendGlyphs
void strFont5(uint8_t []);         // Write Variable or Contstant string in font 5
void strFont5XY(uint8_t [], uint8_t, uint8_t);  // Write (V or C) string in font 5 @XY
void strFont6RXY(uint8_t [], uint8_t, uint8_t); // Write (V or C) string in font 22 @XY
void byteFont5(uint8_t) ;                 // Write 3 characters of byte in font 5
void intFont5XY(uint16_t, uint8_t, uint8_t) ;     // Write 5 characters of int in font 5
void byteFont5XY(uint8_t, uint8_t, uint8_t);    // Write 3 characters of byte in font 5 @XY
void digitFont5(uint8_t) ;                // Send single digit to Display
void digitFont5XY(uint8_t, uint8_t, uint8_t) ;  // Send single digit to Display @XY
void charfont6Ra(uint8_t character);
void charfont6Rb(uint8_t character);
void charfont6Rc(uint8_t character);
void charfont6Rd(uint8_t character);
void charfont6Re(uint8_t character);
void charfont6Rf(uint8_t character);
void strfont6Ra(uint8_t s[]);
void strfont6Rb(uint8_t s[]);
void strfont6Rc(uint8_t s[]);
void strfont6Rd(uint8_t s[]);
void strfont6Re(uint8_t s[]);
void strfont6Rf(uint8_t s[]);



// private functions for reference
// void sendGlyphs(const byte *, byte);  //Send any font row to print character

#endif	/* SH1106_H */

