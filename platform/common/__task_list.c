/**
  ******************************************************************************
  * @file    __task_list.c
  * @author  Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    June-16-2015
  * @brief   This file contains task list
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
#include "os.h"
#include <string.h>

/* Exported variables---------------------------------------------------------*/

/* task handler */
__task_handle_t  __task_handler[__TASK_NUM];

/* task list configure */
const __task_configure_t __task_list[__TASK_NUM] = 
{
    /* __task_watchdog_kick */
    {
        .task           =   __task_watchdog_kick,
        .name           =   "__task_watchdog_kick",
        .prio           =   __TASK_PRIO_HIGHEST,
        .stacksize      =   __TASK_STACK_SIZE_31
    },
    {
      .task             = imuTask,
      .name             = "__task_imu",
      .prio             = __TASK_PRIO_1,
      .stacksize        = __TASK_STACK_SIZE_26
    }
    /**
     * @brief ADD MORE TASK HERE <<<<<<<<<<<<<<<<<<<
     */
};


/* Exported function ----------------------------------------------------------*/
/**
 * @brief __task_query_tid
 * The function will query task id from task name
 * @param task_name - name of task to get task id
 * @return None
 */
int __task_query_tid( const char * const task_name )
{
    int i = 0,ret = -1, EXIT = 0;
    /* loop check task name */
    while( __TASK_NUM > i && !EXIT )
    {
        if(!strcmp(task_name,__task_list[i].name))
        {
            EXIT = 1;
            ret = i;
        }
        i++;
    }
    return ret;
}
