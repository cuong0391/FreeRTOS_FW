/**
  ******************************************************************************
  * @file    log.h
  * @author  Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    June-16-2015
  * @brief   This file contains definitions for debuging lib
  ******************************************************************************
  */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _LOG_H_
#define _LOG_H_

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "std_type.h"
#include "board_config.h" 	
/* Exported define -------------------------------------------------_---------*/


/** for development software debuging */
#define HAL_ERR_DEBUG               STD_ON
/** Message debug */
#define HAL_MSG_DEBUG               STD_ON

/* Exported functions ------------------------------------------------------- */

/**
 * @brief log_init
 * @param None
 * @return None
 */
void log_init( void );

/**
 * @brief log_err
 * @param None
 * @return None
 */
#if (HAL_ERR_DEBUG == STD_OFF) 
inline 
#endif
void log_err( const char* format, ... );

/**
 * @brief log_msg
 * @param None
 * @return None
 */
#if (HAL_MSG_DEBUG == STD_OFF) 
inline 
#endif
void log_msg( const char* format, ... );


#endif /* _LOG_H_ */
