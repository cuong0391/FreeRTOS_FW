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
#include <stdio.h>
#include <stdarg.h>
#include <board_config.h>
/* Exported define -----------------------------------------------------------*/

#define DEBUG_HEADER()              fprintf(stderr,"\t\033[39mDEBUG  \t: ")
#define INFO_HEADER()               fprintf(stderr,"\t\033[92mINFO   \t: ")
#define WARN_HEADER()               fprintf(stderr,"\t\033[93mWARNING\t: ")
#define ERROR_HEADER()              fprintf(stderr,"\t\033[91mERROR  \t: ")
#define COLOR_ENDL()                fprintf(stderr,"\033[39m")
/* Exported macros -------------------------------------------------------- */
/**
 * @brief log_init
 */
void log_init( void );
/**
 * @brief log_error
 * @param restriction
 * @return None
 */
#if(LOG_ERROR == STD_ON ) 
void log_error( const char* format, ... );
#else
#define log_error(...) {/* do nothing */}
#endif

/**
 * @brief log_warn
 * @param restriction
 * @return None
 */
#if(LOG_WARN == STD_ON ) 
void log_warn( const char* format, ... );
#else
#define log_warn(...) {/* do nothing */}
#endif

/**
 * @brief log_info
 * @param restriction
 * @return None
 */
#if(LOG_INFO == STD_ON ) 
void log_info( const char* format, ... );
#else
#define log_info(...) {/* do nothing */}
#endif

/**
 * @brief log_debug
 * @param restriction
 * @return None
 */
#if(LOG_DEBUG == STD_ON ) 
void log_debug( const char* format, ... );
#else
#define log_debug(...) {/* do nothing */}
#endif

/**
 * @brief log_msg
 * @param restriction
 * @return None
 */
#if(LOG_MSG == STD_ON ) 
void log_msg( const char* format, ... );
#else
#define log_msg(...) {/* do nothing */}
#endif

#endif /* _LOG_H_ */
