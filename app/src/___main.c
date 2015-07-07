/**
  ******************************************************************************
  * @file    main.c
  * @author  Embedded Team CopyRight@NTC
  * @version V1.1.0
  * @date    20-June-2015
  * @brief   This file contains main function
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
#include "os.h"
/* Exported functions ------------------------------------------------------- */
int main (void)
{
    uint8_t Index = 0;
    /* init platform */
    init_platform();   
    /* introduction */
    log_msg("Light P1\r\n");
    /* add task to os kernel */ 
    while( __TASK_NUM > Index  )
    {
        log_msg("%s: Create %s\r\n",__FUNCTION__,__task_list[Index].name);
        __task_handler[Index].idx    = Index;
        __task_handler[Index].state  = __TASK_INTIALIZE; 
        xTaskCreate(
                        (pdTASK_CODE)              __task_list[Index].task,          /* task function */
                        (const signed char * const)__task_list[Index].name,          /* task name */
                        (unsigned short)           __task_list[Index].stacksize,     /* stack size */
                                                   &__task_handler[Index],           /* passing parameters */
                        (unsigned portBASE_TYPE)   __task_list[Index].prio,          /* task priority */
                        (xTaskHandle)              &__task_handler[Index].handle     /* handle */
                    );
        Index++;
    }
    /* Start the scheduler so our tasks start executing. */
    vTaskStartScheduler();  
    /* add tasks to os kernel */
    return 0;
}
