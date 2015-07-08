/**
  ******************************************************************************
  * @file    common.h
  * @author  Embedded Team CopyRight@NTC
  * @version V1.1.0
  * @date    05-Mar-2015
  * @brief   This file contains inclusion files
  ******************************************************************************
  */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _OS_H_
#define _OS_H_

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/** Standard library. */
#include <stdint.h>

/** OS */
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "task.h"

/** error, type definition */
#include "std_type.h"

/** error, type definition */
#include "std_type.h"

/** driver inclusion */
#include "log.h"        
/** application inclusion */
#include "__task_watchdog_kick.h" 
  

/* Exported define -----------------------------------------------------------*/

#define    __TASK_NUM               1

#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0) 

  
/* Exported typedef ----------------------------------------------------------*/

/**
 * @brief __task_priority_t
 *
 * #com portname define
 */
typedef enum __task_priority {
    __TASK_PRIO_1  = tskIDLE_PRIORITY + 1,
    __TASK_PRIO_2  = tskIDLE_PRIORITY + 2,
    __TASK_PRIO_3  = tskIDLE_PRIORITY + 3,
    __TASK_PRIO_4  = tskIDLE_PRIORITY + 4,
    __TASK_PRIO_5  = tskIDLE_PRIORITY + 5,
    __TASK_PRIO_6  = tskIDLE_PRIORITY + 6,
    __TASK_PRIO_7  = tskIDLE_PRIORITY + 7,
    __TASK_PRIO_8  = tskIDLE_PRIORITY + 8,
    __TASK_PRIO_9  = tskIDLE_PRIORITY + 9,
    __TASK_PRIO_10 = tskIDLE_PRIORITY + 10,
    __TASK_PRIO_11 = tskIDLE_PRIORITY + 11,
    __TASK_PRIO_12 = tskIDLE_PRIORITY + 12,
    __TASK_PRIO_13 = tskIDLE_PRIORITY + 13,
    __TASK_PRIO_14 = tskIDLE_PRIORITY + 14,
    __TASK_PRIO_15 = tskIDLE_PRIORITY + 15,
    __TASK_PRIO_16 = tskIDLE_PRIORITY + 16,
    __TASK_PRIO_17 = tskIDLE_PRIORITY + 17,
    __TASK_PRIO_18 = tskIDLE_PRIORITY + 18,
    __TASK_PRIO_19 = tskIDLE_PRIORITY + 19,
    __TASK_PRIO_20 = tskIDLE_PRIORITY + 20,
    __TASK_PRIO_21 = tskIDLE_PRIORITY + 21,
    __TASK_PRIO_22 = tskIDLE_PRIORITY + 22,
    __TASK_PRIO_23 = tskIDLE_PRIORITY + 23,
    __TASK_PRIO_24 = tskIDLE_PRIORITY + 24,
    __TASK_PRIO_25 = tskIDLE_PRIORITY + 25,
    __TASK_PRIO_26 = tskIDLE_PRIORITY + 26,
    __TASK_PRIO_27 = tskIDLE_PRIORITY + 27,
    __TASK_PRIO_28 = tskIDLE_PRIORITY + 28,
    __TASK_PRIO_29 = tskIDLE_PRIORITY + 29,
    __TASK_PRIO_30 = tskIDLE_PRIORITY + 30,
    __TASK_PRIO_HIGHEST = tskIDLE_PRIORITY + 31
}__task_priority_t;

/**
 * @brief __task_stacksize_t
 *
 * #com portname define
 */
typedef enum __task_stacksize {
    __TASK_STACK_SIZE_1  = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_2  = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_3  = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_4  = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_5  = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_6  = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_7  = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_8  = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_9  = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_10 = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_11 = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_12 = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_14 = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_15 = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_16 = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_17 = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_18 = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_19 = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_20 = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_21 = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_22 = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_24 = configMINIMAL_STACK_SIZE + 0,
    __TASK_STACK_SIZE_25 = configMINIMAL_STACK_SIZE + 128,
    __TASK_STACK_SIZE_26 = configMINIMAL_STACK_SIZE + 2048,
    __TASK_STACK_SIZE_27 = configMINIMAL_STACK_SIZE + 2048,
    __TASK_STACK_SIZE_28 = configMINIMAL_STACK_SIZE + 2048,
    __TASK_STACK_SIZE_29 = configMINIMAL_STACK_SIZE + 2048,
    __TASK_STACK_SIZE_30 = configMINIMAL_STACK_SIZE + 2048,
    __TASK_STACK_SIZE_31 = configMINIMAL_STACK_SIZE + 0
}__task_stacksize_t;

/**
 * @brief __task_configure_t
 *
 */
typedef struct __task_configure {
    void                      (*task)(void*) ; /** pointer to task */
    const char * const        name;            /** task name */
    __task_stacksize_t        stacksize;       /** task stack size */
    __task_priority_t         prio;            /** task priority */
}__task_configure_t;

/**
 * @brief __task_state
 *
 */
typedef enum __task_state {
    __TASK_INTIALIZE = 0,
    __TASK_READY     = 1
}__task_state_t;

/**
 * @brief __task_handle_t
 *
 */
typedef struct __task_handle {
    __task_state_t             state;                /** Indicate that task init done or not */
    uint8_t                    idx;                  /** Index of task in task list */
    xTaskHandle                handle;               /** Task handler */
}__task_handle_t;

/* Exported variables --------------------------------------------------------*/

/* task handler */
extern __task_handle_t  __task_handler[__TASK_NUM];

/* task list configure */
extern const __task_configure_t __task_list[__TASK_NUM];

/* Exported functions --------------------------------------------------------*/

/**
 * @brief init_platform
 * The function starts all driver
 * @param None
 * @return None
 */
void init_platform( void );

/**
 * @brief __task_query_tid
 * The function will query task id from task name
 * @param task_name - name of task to get task id
 * @return None
 */
int __task_query_tid( const char * const task_name );



#endif /* _OS_H_ */
