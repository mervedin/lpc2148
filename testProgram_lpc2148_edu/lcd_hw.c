/******************************************************************************
 *
 * Copyright:
 *    (C) 2006 Embedded Artists AB
 *
 * Description:
 *    Implements hardware specific routines
 *
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "../pre_emptive_os/api/osapi.h"
#include "../pre_emptive_os/api/general.h"
#include <lpc2xxx.h>
#include "lcd_hw.h"

/******************************************************************************
 * Typedefs and defines
 *****************************************************************************/


/*****************************************************************************
 * Global variables
 ****************************************************************************/

/*****************************************************************************
 * Local variables
 ****************************************************************************/

/*****************************************************************************
 * Local prototypes
 ****************************************************************************/


/*****************************************************************************
 *
 * Description:
 *    Send 9-bit data to LCD controller
 *    przesylanie informacji na ekran
 *
 ****************************************************************************/
void
sendToLCD(tU8 firstBit, tU8 data)
{
  //disable SPI
 // wylaczenie SPI w celu ustawienia lub zresetowania MOSI
  IOCLR = LCD_CLK;
  PINSEL0 &= 0xffffc0ff;
  
  if (1 == firstBit)
    IOSET = LCD_MOSI;   //set MOSI -> jesli przesylamy dane do wyswietlania na ekran
  else
    IOCLR = LCD_MOSI;   //reset MOSI -> jesli przesylamy dane dotyczace konfiguracji ekranu LCD

  
  //Set clock high (ustawienie sygnalu wysokiego na linii sygnalu zerowego)
  IOSET = LCD_CLK;
  
  //Set clock low (ustawienie sygnalu niskiego na linii sygnalu zerowego)
  IOCLR = LCD_CLK;
  
  /*
   * Enable SPI again
   */
  //initialize SPI interface
  SPI_SPCCR = 0x08;    
  SPI_SPCR  = 0x20;

  //connect SPI bus to IO-pins
  //podlaczenie SPI do pinow
  PINSEL0 |= 0x00001500;
  
  //send byte
  SPI_SPDR = data;
  while((SPI_SPSR & 0x80) == 0)
    ;
}


/*****************************************************************************
 *
 * Description:
 *    Initialize the SPI interface for the LCD controller
 *    Ustawianie interfejsu SPI dla LCD
 * 
 ****************************************************************************/
void
initSpiForLcd(void)
{
  //make SPI slave chip select an output and set signal high
	//ustawienie dla wyjscia interfejsu SPI stanu wysokiego -> zgodnie z dokumentacja
  IODIR |= (LCD_CS | LCD_CLK | LCD_MOSI);
  
  //deselect controller
  selectLCD(FALSE);

  //connect SPI bus to IO-pins
  //podlaczenie SPI do pinow
  PINSEL0 |= 0x00001500;
  
  //initialize SPI interface
  //inicjalizacja interfejsu SPI
  SPI_SPCCR = 0x08;    
  SPI_SPCR  = 0x20;
}

/*****************************************************************************
 *
 * Description:
 *    Select/deselect LCD controller (by controlling chip select signal)
 *    Przed okresleniem ustawien dla LCD (czyli przed wyslaniem do niego odpowiednich informacji
 *    nalezy go wybrac po to by potem w funkcji sendToLCD w zaleznosci czy chcemy cos wsywietlic 
 *    czy zmienic konfiguracje ekranu mozna bylo ustawic lub zresetowac MOSI
 ****************************************************************************/
void
selectLCD(tBool select)
{
  if (TRUE == select)
    IOCLR = LCD_CS; //wybieranie ekranu LCD
  else
    IOSET = LCD_CS; //zwalnianie ekranu LCD
}
