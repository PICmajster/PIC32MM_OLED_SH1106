/*
    File Name:        :  main.c
    Device            :  PIC32MM0256GPM048
    Compiler          :  XC32 2.05
    MPLAB             :  MPLAB X 4.15
    Created by        :  http://strefapic.blogspot.com
*/
#include "mcu_config_files/mcc.h"
#include "delay.h"
#include "sh1106.h"


int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    initDisplay(); /*Alleluja*/

    //strFont5XY("HELLO WORLD", 30, 1) ;
    //strfont6RXY("1", 10,1);
    cursorXY(10, 10) ;
    strfont6Ra("1") ;
    //strfont6Rb("1") ;
    //strfont6Rc("1") ;
    //strfont6Rd("1") ;
    //strfont6Re("1") ;
    //strfont6Rf("1") ;
    
    while (1)
    {
       
    }
}



// When using interrupts, you need to set the Global Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalDisable();