/*
    File Name:        :  mcc.h

    Device            :  PIC32MM0256GPM048
    Compiler          :  XC32 2.05
    MPLAB             :  MPLAB X 4.15
    Created by        :  http://strefapic.blogspot.com
*/

#ifndef MCC_H
#define	MCC_H
#define WDT_CLR_KEY 0x5743
#include "pin_manager.h"
#include "interrupt_manager.h"
#include "exceptions.h"
#include <xc.h>
#include <string.h> /*for memset() etc*/
#include <stdint.h> /*uint8_t etc.*/
#include <stdbool.h>
#include <sys/attribs.h> /*for Interrupt*/
#include <stdio.h> /*sprintf() etc*/

#define _XTAL_FREQ  24000000UL

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Unlocks the write protected register to enable any write operation
 *                  MCC GUI
 * @Example
    SYSTEM_RegUnlock();
 */
inline static void SYSTEM_RegUnlock(void)
{
    SYSKEY = 0x0; //write invalid key to force lock
    SYSKEY = 0xAA996655; //write Key1 to SYSKEY
    SYSKEY = 0x556699AA; //write Key2 to SYSKEY
}

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Locks the write protected register to disable any write operation
 *                  MCC GUI
 * @Example
    SYSTEM_RegLock();
 */
inline static void SYSTEM_RegLock(void)
{
   SYSKEY = 0x00000000; 
}

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Initializes the device to the default states configured in the
 *                  MCC GUI
 * @Example
    SYSTEM_Initialize(void);
 */
void SYSTEM_Initialize(void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Initializes the oscillator to the default states configured in the
 *                  MCC GUI
 * @Example
    OSCILLATOR_Initialize(void);
 */
void OSCILLATOR_Initialize(void);

/* Enables Watch Dog Timer (WDT) using the software bit.
 * @example
 * <code>
 * WDT_WatchdogtimerSoftwareEnable();
 * </code>
 */
inline static void WDT_WatchdogtimerSoftwareEnable(void)
{
    WDTCONbits.ON = 1;
}
/* Disables Watch Dog Timer (WDT) using the software bit.
 * @example
 * <code>
 * WDT_WatchdogtimerSoftwareDisable();
 * </code>
 */
inline static void WDT_WatchdogtimerSoftwareDisable(void)
{
    WDTCONbits.ON = 0;
}
/* Clears the Watch Dog Timer (WDT).
 * @example
 * <code>
 * WDT_WatchdogTimerClear();
 * </code>
 */
inline static void WDT_WatchdogTimerClear(void)
{
    WDTCONbits.WDTCLRKEY = WDT_CLR_KEY;
}
#endif	/* MCC_H */
