/******************************************************************************
 *
 * Copyright:
 *    (C) 2006 Embedded Artists AB
 *
 * File:
 *    key.c
 *
 * Description:
 *    Implements sampling and handling of joystick key.
 *
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "../pre_emptive_os/api/osapi.h"
#include "../pre_emptive_os/api/general.h"
#include <printf_P.h>
#include "key.h"
#include <lpc2xxx.h>
/******************************************************************************
 * Typedefs and defines
 *****************************************************************************/
#define KEYPROC_STACK_SIZE 300
#define KEYPIN_CENTER 0x00000100
#define KEYPIN_UP     0x00000400
#define KEYPIN_DOWN   0x00001000
#define KEYPIN_LEFT   0x00000200 
#define KEYPIN_RIGHT  0x00000800

#if 0
      //check if P0.8 center-key is pressed
      if ((IOPIN & 0x00000100) == 0)

      //check if P0.9 left-key is pressed
      else if ((IOPIN & 0x00000200) == 0)

      //check if P0.10 up-key is pressed
      else if ((IOPIN & 0x00000400) == 0)

      //check if P0.11 right-key is pressed
      else if ((IOPIN & 0x00000800) == 0)

      //check if P0.12 down-key is pressed
      else if ((IOPIN & 0x00001000) == 0)

#endif
/*****************************************************************************
 * Local variables
 ****************************************************************************/
static volatile tU8 activeKey = KEY_NOTHING;
static tU8 keyProcStack[KEYPROC_STACK_SIZE];
static tU8 keyProcPid;
/*****************************************************************************
 *
 * Description:
 *    Get current state of joystick switch
 *
 ****************************************************************************/
tU8
getKeys(void)
{
  tU8 readKeys = KEY_NOTHING;

  if ((IOPIN & KEYPIN_CENTER) == 0) readKeys |= KEY_CENTER;
  if ((IOPIN & KEYPIN_UP) == 0)     readKeys |= KEY_UP;
  if ((IOPIN & KEYPIN_DOWN) == 0)   readKeys |= KEY_DOWN;
  if ((IOPIN & KEYPIN_LEFT) == 0)   readKeys |= KEY_LEFT;
  if ((IOPIN & KEYPIN_RIGHT) == 0)  readKeys |= KEY_RIGHT;

  return readKeys;
}

/*****************************************************************************
 *
 * Description:
 *    Function to check if any key press has been detected
 *
 ****************************************************************************/
tU8 checkKey(void)
{
  tU8 retVal = activeKey;
  activeKey = KEY_NOTHING;
  return retVal;
}

tBool damping(tU8 readKeys)  //tlumienie drgan zestykow
{
    tU8 check =getKeys();
    tS32 i;
    for( i=0; i<10; i++)
    {
        if(readKeys != check) return TRUE;  //10 razy sprawdzamy, czy nie wcisniety 
        check =getKeys(); 
        osSleep(1);
    }
    return FALSE;
}

/*****************************************************************************
 *
 * Description:
 *    Sample key states
 * monitorowanie zmian stanu joysticka
 *  checkawdza obsluge ktorych kierunkow umozliwiono a potem checkawdza w ktora strone wychylony jest joystick
 * i odpowiednio ustawia wartosci zmiennych globalnych
 ****************************************************************************/
void
sampleKey(void)
{
  tBool nothing = TRUE;
  tU8   readKeys;
  
  //get sample
  readKeys = getKeys(); //pobranie kierunku jaki zostal wybrany na joysticku
  
  //check center key
  if (readKeys & KEY_CENTER)
  {
      nothing = damping(readKeys);
      activeKey = KEY_CENTER;
  }
    //check up key
  else if (readKeys & KEY_UP)
  {
       nothing = damping(readKeys);
      activeKey = KEY_UP;
  }
    //check down key
  else if (readKeys & KEY_DOWN)
  {
    nothing = damping(readKeys);
    activeKey = KEY_DOWN;
  }
   //check left key
  else if (readKeys & KEY_LEFT)
  {
    nothing = damping(readKeys);
    activeKey = KEY_LEFT;
  }
  //check right key
  else if (readKeys & KEY_RIGHT)
  {
    nothing = damping(readKeys);
    activeKey = KEY_RIGHT;
  }
}

/*****************************************************************************
 *
 * Description:
 *    A process entry function 
 *
 * Params:
 *    [in] arg - This parameter is not used in this application. 
 *
 ****************************************************************************/
static void
procKey(void* arg)
{
  //make all key signals as inputs
  //okreslenie wszystkich mozliwych sygnalow joysticka jako sygnaly wejsciowe
  IODIR &= ~(KEYPIN_CENTER | KEYPIN_UP | KEYPIN_DOWN | KEYPIN_LEFT | KEYPIN_RIGHT);

  //sample keys each 50 ms, i.e., 20 times per second
  while(1)
  {
    sampleKey();
    osSleep(5);
  }
}

/*****************************************************************************
 *
 * Description:
 *    Creates and starts the key sampling process. 
 *
 ****************************************************************************/
void
initKeyProc(void)
{
  tU8 error;

  osCreateProcess(procKey, keyProcStack, KEYPROC_STACK_SIZE, &keyProcPid, 3, NULL, &error);
  osStartProcess(keyProcPid, &error);
}

