/******************************************************************************
 *
 * Copyright:
 *    (C) 2000 - 2005 Embedded Artists AB
 *
 * Description:
 *
 * ESIC:
 *    pre_emptive_os
 *
 * Version:
 *    1.4.0
 *
 * Generate date:
 *    2005-03-15 at 20:27:15
 *
 * NOTE:
 *    DO NOT EDIT THIS FILE. IT IS AUTO GENERATED.
 *    CHANGES TO THIS FILE WILL BE LOST IF THE FILE IS RE-GENERATED
 *
 * Signature:
 *   7072655F656D70746976655F6F73,312E342E302E30,020235
 *   ,35,10104021013134373435363030,07323838,3732,01013
 *   830,0101013138303030303030,3135,33,3135,0232323530
 *   ,01020130,0231343734353539,3238313831,020101100302
 *   103030310010133,0163130,3230,3330,3430,3530,3630,3
 *   730,3830,3930,313030,313130,313230,313330,313430,3
 *   13530,313630,,35,35,35,35,35,35,35,35,35,35,35,35,
 *   35,35,35,35,,,,1001001100011000000000]484152445741
 *   5245,4C5043323130365F32,545538,756E7369676E6564206
 *   3686172,414C49474E4D454E54,34,54424F4F4C,756E73696
 *   76E65642063686172,54553332,756E7369676E656420696E7
 *   4,544D505F46494C4553,2A2E656C663B2A2E6C73743B2A2E6
 *   D61703B2A2E6F3B2A2E6F626A3B2A2E64,454E4449414E,4C4
 *   954544C45,54533332,7369676E656420696E74,545338,736
 *   9676E65642063686172,54553136,756E7369676E656420736
 *   86F7274,54533136,7369676E65642073686F7274,44455343
 *   52495054494F4E,,44454255475F4C4556454C,30,434F4445
 *   5F524F4F54,,47454E5F52554C4553,,4C494E455F5445524D
 *   ,43524C46,4252414345,,43524541544F52,416E646572732
 *   0526F7376616C6C,4352454154494F4E5F44415445,3230303
 *   52D30332D31352032303A31373A3432,524F4F54,433A2F446
 *   F63756D656E747320616E642053657474696E67732F416E646
 *   5727320526F7376616C6C2F4D696E6120646F6B756D656E742
 *   F456D62656464656420417274697374732F50726F647563747
 *   32F4C50433231303620525332333220517569636B537461727
 *   420426F6172642F72746F732F]505245464958,,4445425547
 *   5F4C4556454C,30,555345525F434F4D4D454E54,]64656661
 *   756C74,
 *
 * Checksum:
 *    224270
 *
 *****************************************************************************/


/******************************************************************************
 * Includes
 *****************************************************************************/

#include "stack_usage.h"
#include "kernel.h"

/******************************************************************************
 * Local functions
 *****************************************************************************/

static tU8 readStackUsage(tU8* pStackArea,
                          tU16 size);

/******************************************************************************
 * Implementation of public functions
 *****************************************************************************/


/*****************************************************************************
 *
 * Description:
 *    This function returns the stack usage. The stack usage is based on the 
 *    maximum size used so far, i.e. from the application start to the point 
 *    where this function is called. 
 *
 * Params:
 *    [in] pid - The pid of the process to check. 
 *
 * Returns:
 *    The used fraction of the stack area specified in percent. 
 *
 ****************************************************************************/
tU8
osStackUsage(tU8 pid)
{
  return readStackUsage(processControlBlocks[pid].pStkOrg, processControlBlocks[pid].stackSize);
}


/*****************************************************************************
 *
 * Description:
 *    This function creates the stack pattern. The stack pattern is used to 
 *    see how much of the stack that has not been used. 
 *
 * Params:
 *    [in] pStackArea - A pointer to a stack area. 
 *    [in] size       - The size of the stack area in bytes. 
 *
 ****************************************************************************/
void
createStackPattern(tU8* pStackArea,
                   tU16 size)
{
  tU16 i;
  for(i=0; i<size; i++)
  {
    pStackArea[i] = 0xAA;
  }
}


/*****************************************************************************
 *
 * Description:
 *    This function checks if the stack has grown past a specified limit. 
 *
 * Params:
 *    [in] pStackArea - A pointer to a stack area. 
 *    [in] size       - The size of the stack area in bytes. 
 *    [in] limit      - The limit is specified in percent of the total size of 
 *                      the stack area. 
 *
 * Returns:
 *    TRUE if the stack has grown past the specified limit, else FALSE. 
 *
 ****************************************************************************/
tBool
checkStackLimit(tU8* pStackArea,
                tU16 size,
                tU8  limit)
{
  tU8 i;
  tU8 * pTmp;
  pTmp = pStackArea + (size*(100-limit))/100;

  for(i=0; i<20; pTmp--,i++)
  {
    if(*pTmp != 0xAA)
      break;
  }

  return i!=20;
}

/******************************************************************************
 * Implementation of local functions
 *****************************************************************************/


/*****************************************************************************
 *
 * Description:
 *    This function scans the stack to see how much of the stack that has been 
 *    used so far. 
 *
 * Params:
 *    [in] pStackArea - A pointer to a stack area. 
 *    [in] size       - The size of the stack area in bytes. 
 *
 * Returns:
 *    The used fraction of the stack specified in percent. 
 *
 ****************************************************************************/
static tU8
readStackUsage(tU8* pStackArea,
               tU16 size)
{
  tU16 i;
  tU32 tmp;
  tU8 * pBottom;
  tU8 * pTop;
  tU8 * pTmp;
  pTop = pStackArea;           
  pBottom = pStackArea + size; 

  for(pTmp=pTop,i=0; pTmp<pBottom; pTmp++)
  {
    if(*pTmp != 0xAA)
      break;
    i++;
  }

  tmp = (tU32)(size-i)*100/size;  
  return (tU8)tmp;
}