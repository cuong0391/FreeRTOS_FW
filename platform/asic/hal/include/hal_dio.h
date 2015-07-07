/**
  ******************************************************************************
  * @file    hal_dio.h
  * @author  Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    June-04-2015
  * @brief   This file expandes for #dio driver
  ******************************************************************************
  */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _HAL_DIO_H_
#define _HAL_DIO_H_

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "std_type.h"

/* Exported define ----------------------------------------------------------*/


/** for development software */
#define HAL_DIO_DEBUG               STD_OFF

/* Exported typedef ----------------------------------------------------------*/

/**
 * @brief hal_dio_return_t
 *
 * #dio return type
 */
typedef enum _hal_dio_return{
    DIO_OK,
    DIO_NULL_PTR,
    DIO_INVALID_PORT,
    DIO_UNKNOWN_ERROR,
}hal_dio_return_t;

/**
 * @brief hal_dio_chanel_t
 *
 * Support dio channels
 */
typedef enum _hal_dio_channel {
    HAL_DIO_CH1 = 0,
    HAL_DIO_CH2,
    HAL_DIO_CH3,
    HAL_DIO_CH4,
    HAL_DIO_CH5,
    HAL_DIO_CH6,
    HAL_DIO_CH7,
    HAL_DIO_CH8,
    HAL_DIO_CH9,
    HAL_DIO_CH10,
    HAL_DIO_CH11,
    HAL_DIO_CH12,
    HAL_DIO_CH13,
    HAL_DIO_CH14,
    HAL_DIO_CH15,
    HAL_DIO_CH16,
    HAL_DIO_CH17,
    HAL_DIO_CH18,
    HAL_DIO_CH19,
    HAL_DIO_CH20,
    HAL_DIO_CH21,
    HAL_DIO_CH22,
    HAL_DIO_CH23,
    HAL_DIO_CH24,
    HAL_DIO_CH25,
    HAL_DIO_CH26,
    HAL_DIO_CH27,
    HAL_DIO_CH28,
    HAL_DIO_CH29,
    HAL_DIO_CH30,              
    HAL_DIO_MAX
}hal_dio_chanel_t;

/**
 * @brief hal_dio_dir_t
 *
 * Support #dio direction
 */
typedef enum _hal_gio_dir {
    HAL_DIO_IN = 0,
    HAL_DIO_OUT,
}hal_dio_dir_t;

/**
 * @brief hal_dio_level_t
 *
 * Support #dio logic level
 */
typedef enum _hal_level {
    HAL_DIO_LOW = 0,
    HAL_DIO_HIGH,
}hal_dio_level_t;

/**
 * @brief hal_dio_t
 *
 * #dio driver structure
 */
typedef struct _hal_dio {
    hal_dio_chanel_t           chid;               /** hal dio channel id */
    hal_dio_dir_t              dir;                /** hal dio direction */
    hal_dio_level_t            state;              /** hal dio status */
}hal_dio_t;


/* Exported functions ------------------------------------------------------- */

/**
 * @brief hal_dio_init
 * The function shall be initialize independence dio channel
 * @param #dio Point to hal_dio_t structure
 * @return reference to #hal_dio_return_t
 */
int hal_dio_init( hal_dio_t * dio );

/**
 * @brief hal_dio_set_low
 * The function sets low logic level on #dio channel
 * @param #dio Point to hal_dio_t structure
 * @return reference to #hal_dio_return_t
 */
int hal_dio_set_low( hal_dio_t * dio );

/**
 * @brief hal_dio_set_high
 * The function sets high logic level on #dio channel
 * @param #dio Point to hal_dio_t structure
 * @return reference to #hal_dio_return_t
 */
int hal_dio_set_high( hal_dio_t * dio );

/**
 * @brief hal_dio_toggle
 * The function toggles logic level #dio channel
 * @param #dio Point to hal_dio_t structure
 * @return reference to #hal_dio_return_t
 */
int hal_dio_toggle( hal_dio_t * dio );

/**
 * @brief hal_dio_read
 * The function reads logic level on #dio channel
 * @param #dio specific #dio structure
 * @return reference to #hal_dio_level_t
 */
hal_dio_level_t hal_dio_read( hal_dio_t * dio );

#endif /* _HAL_DIO_H_ */