/**
  ******************************************************************************
  * @file    os_assert.c
  * @author  Embedded Team CopyRight@NTC
  * @version V1.1.0
  * @date    June-17-2015
  * @brief   This file contains inclusion files
  ******************************************************************************
  */ 
#include "os.h"

/*-----------------------------------------------------------*/
void vApplicationMallocFailedHook(void)
{
    /* This function will only be called if an API call to create a task, queue
    or semaphore fails because there is too little heap RAM remaining - and
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. */
    while(1);
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName)
{
    (void)pxTask;
    (void)pcTaskName;
    /* This function will only be called if a task overflows its stack.  Note
    that stack overflow checking does slow down the context switch
    implementation and will only be performed if configCHECK_FOR_STACK_OVERFLOW
    is set to either 1 or 2 in FreeRTOSConfig.h. */
    while(1);
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook(void)
{
    /* This example does not use the idle hook to perform any processing.  The
    idle hook will only be called if configUSE_IDLE_HOOK is set to 1 in 
    FreeRTOSConfig.h. */
}
/*-----------------------------------------------------------*/

void vApplicationTickHook(void)
{
    /* This example does not use the tick hook to perform any processing.   The
    tick hook will only be called if configUSE_TICK_HOOK is set to 1 in
    FreeRTOSConfig.h. */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* func, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\n\r", file, line) */
  //log_printf("\e[31m[%s(%d)] Wrong parameters value\e[39m\n\r", func, (int)line);
  /* Infinite loop */
  while (1)
  {
  }
}
#endif