/**
  ******************************************************************************
  * @file    hal_com.h
  * @author  Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    June-03-2015
  * @brief   This file contains definitions for #Serial driver
  ******************************************************************************
  */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _HAL_COM_H_
#define _HAL_COM_H_

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "std_type.h"
  
/* Exported define -------------------------------------------------_---------*/

/** for development software debuging */
#define HAL_COM_DEBUG               STD_ON

/** Send data timeout setting */
#define HAL_COM_TIMEOUT             10000000
/* Exported typedef ----------------------------------------------------------*/

/**
 * @brief hal_com_return_t
 *
 * com return type
 */
typedef enum _hal_com_return {
    COM_OK,
    COM_NULL_PTR,
    COM_INVALID_PORT,
    COM_NULL_IRQ,
    COM_UNKNOWN_ERROR,
}hal_com_return_t;

/**
 * @brief hal_com_name
 *
 * #com portname define
 */
typedef enum _hal_com_name {
    COM1,
    COM2,
    COM3,
    COM4,
    COM5,
    COM_MAX_IDX
}hal_com_name_t;

/**
 * @brief hal_com_baudrate_t
 *
 * #com baudrate define
 */
typedef enum _hal_com_baudrate {
    COM_BAUD_9600    = 9600,
    COM_BAUD_19200   = 19200,
    COM_BAUD_38400   = 38400,
    COM_BAUD_57600   = 57600,
    COM_BAUD_115200  = 115200
}hal_com_baud_t;

/**
 * @brief hal_com_prirority_t
 *
 * #com priority define
 */
typedef enum _hal_com_prirority {
    COM_PRIO_1 = 1,
    COM_PRIO_2 = 2,
    COM_PRIO_3 = 3,
    COM_PRIO_4 = 4,
    COM_PRIO_5 = 5,
}hal_com_prio_t;

/**
 * @brief hal_com_t
 *
 * #COM driver structure
 */
typedef struct _hal_com {
    const hal_com_name_t        port_name;                 /** hal com port name */
    const hal_com_baud_t        baudrate;                  /** hal com baudrate */
    const hal_com_prio_t        priority;                  /** hal com priority */
    void                        (*irq_handler)(uint8_t c); /** point to function be handled in irq when received a character */
    uint8_t                     *data;
}hal_com_t;


/* Exported functions ------------------------------------------------------- */

/**
 * @brief hal_com_init
 * The function shall initialize independence com channel
 * @param com Point to hal_com_t structure
 * @return reference to #hal_com_return_t
 */
int hal_com_init( hal_com_t * com );

/**
 * @brief hal_com_deinit
 * The function stops operation on independence com channel
 * @param com Point to hal_com_t structure
 * @return reference to #hal_com_return_t
 */
int hal_com_deinit( hal_com_t * com );

/**
 * @brief hal_com_sendbyte
 * The function sends one byte to hardware layer
 * @param com Point to hal_pwm_t structure
 * @return reference to #hal_com_return_t
 */
int hal_com_sendbyte( hal_com_t * com );

/**
 * @brief hal_com_readbyte
 * The function stops independence com channel
 * @param com Point to hal_com_t structure
 * @return reference to #hal_com_return_t
 */
int hal_com_readbyte( hal_com_t * com );

/**
 * @brief hal_com_enable_irq
 * The function reads one byte from hardware layer
 * @param com Point to hal_com_t structure
 * @return reference to #hal_com_return_t
 */
int hal_com_enable_irq( hal_com_t * com );

/**
 * @brief hal_com_disable_irq
 * The function disables hardware interrupt
 * @param com Point to hal_com_t structure
 * @return reference to #hal_com_return_t
 */
int hal_com_disable_irq( hal_com_t * com );

#endif /* _HAL_COM_H_ */
