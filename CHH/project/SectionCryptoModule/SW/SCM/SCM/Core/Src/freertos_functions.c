/*
 * freertos_functions.c
 *
 *  Created on: Dec 18, 2021
 *      Author: Hyun-Ho Cha
 */
#include "cmsis_os.h"

void Unit_Side_Data_Task(void const * argument)
{

  for(;;)
  {
    osDelay(1);
  }

}

void SCM_Side_Data_Task(void const * argument)
{

  for(;;)
  {

    osDelay(1);
  }

}
