/******************************************************************************
 *
 * Copyright:
 *    (C) 2007 Embedded Artists AB
 *
 * Description:
 *    Expose hardware specific routines
 *
 *****************************************************************************/
#ifndef _PCA9532_H_
#define _PCA9532_H_

/******************************************************************************
 * Includes
 *****************************************************************************/
//#include <general.h> // TODO: Setup includes
#include "../pre_emptive_os/api/general.h"
#include <lpc2xxx.h>


/******************************************************************************
 * Typedefs and defines
 *****************************************************************************/
#define LCD_CS     0x00000080
#define LCD_CLK    0x00000010
#define LCD_MOSI   0x00000040


/*****************************************************************************
 * Global variables
 ****************************************************************************/
void sendToLCD(tU8 firstBit, tU8 data);
void initSpiForLcd(void);
void selectLCD(tBool select);

#endif
