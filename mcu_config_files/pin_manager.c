/**
  System Interrupts Generated Driver File 

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.c

 
  @Description:
    This source file provides implementations for MPLAB(c) Code Configurator interrupts.
    Generation Information : 
        Product Revision  :  MPLAB(c) Code Configurator - 4.45
        Device            :  PIC32MM00256GPM048
    The generated drivers are tested against the following:
        Compiler          :  XC32 v2.05
        MPLAB             :  MPLAB X v4.15

    */


#include <xc.h>
#include "pin_manager.h"
#include "mcc.h"

/**
    void PIN_MANAGER_Initialize(void)
*/
void PIN_MANAGER_Initialize(void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000;
    LATB = 0x0000;
    LATC = 0x0000;
    LATD = 0x0000;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    /*set Input for PGD1 and PGC1*/
    TRISBbits.TRISB0 = 1 ;
    TRISBbits.TRISB1 = 1 ;
    /*set Output for OLED*/
    TRISAbits.TRISA15 = 0 ;
    TRISBbits.TRISB10 = 0 ;
    TRISBbits.TRISB11 = 0 ;
    TRISBbits.TRISB13 = 0 ;
    TRISCbits.TRISC9 = 0 ;
    
    
    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPDA = 0x0000;
    CNPDB = 0x0000;
    CNPDC = 0x0000;
    CNPDD = 0x0000;
    CNPUA = 0x0000;
    CNPUB = 0x0000;
    CNPUC = 0x0000;
    CNPUD = 0x0000;

    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x0000;
    ODCC = 0x0000;
    ODCD = 0x0000;

    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    /*set pins for PGD1 and PGC1*/
    ANSELBbits.ANSB0 = 0 ; /*set digital for PGD1*/
    ANSELBbits.ANSB1 = 0 ; /*set digital for PGC1*/
  

}
/* Piny analogowe na starcie, ustawienie : 0 - cyfrowe / 1 - analogowe
ANSELA
RA0 --> ANSA0
RA1 --> ANSA1
RA2 --> ANSA2
RA3 --> ANSA3
ANSELB
RB0 --> ANSB0
RB1 --> ANSB1
RB2 --> ANSB2
RB3 --> ANSB3
RB4 --> ANSB4
ANSELC
RC0 --> ANSC0
RC1 --> ANSC1
RC5 --> ANSC5
RC8 --> ANSC8
*/