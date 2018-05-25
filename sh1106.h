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
#define DisplayRST   PORTBbits.RB11          
#define DisplayCLK   PORTBbits.RB13          

#define sWidth 128      // screen width
#define sXOffset 2      // x offset


/*Function Prototypes*/

/*Display Internal functions*/
void initDisplay(void) ;     // initialise the display
void sendCmd(byte) ;         // write a command to display
void sendData(byte) ;        // write data to display
void writeSD(byte);          // clock out serial data to the display controller

/*Display Control*/
void onDisplay(void) ;      // turn on OLED panel
void offDisplay(void) ;     // turn off OLED panel
void clsDisplay(void) ;     // erase the display DDRAM and set cursor home (0,0)
void clLnDisplay(byte) ;    // clear line on display, cursor to line start
void cursorXY(byte, byte) ; // position cursor to column, line
void setContrast(byte) ;    // set contrast

/*Display*/
void rptPix(byte, byte, byte) ; // repeat pixel across Display - show bar graph
void barDisplay(byte) ;         // display bar graph
void charFont5(byte);           // Write byte in font 5, via sendGlyphs
void strFont5(byte []);         // Write Variable or Contstant string in font 5
void strFont5XY(byte [], byte, byte);  // Write (V or C) string in font 5 @XY
void strFont6RXY(byte [], byte, byte); // Write (V or C) string in font 22 @XY
void byteFont5(byte) ;                 // Write 3 characters of byte in font 5
void intFont5XY(int, byte, byte) ;     // Write 5 characters of int in font 5
void byteFont5XY(byte, byte, byte);    // Write 3 characters of byte in font 5 @XY
void digitFont5(byte) ;                // Send single digit to Display
void digitFont5XY(byte, byte, byte) ;  // Send single digit to Display @XY

// private functions for reference
// void sendGlyphs(const byte *, byte);  //Send any font row to print character

#endif	/* SH1106_H */

