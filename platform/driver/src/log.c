/**
  ******************************************************************************
  * @file    log.c
  * @author  Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    June-16-2015
  * @brief   This file contains expansion for debuging lib
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>  
#include "log.h"
#include "board_config.h"
#include "os.h"  
/* Privated function -------------------------------------------------------- */ 
static void com_irq_handler(uint8_t c);

/* Privated variables ------------------------------------------------------- */ 
static hal_com_t com_dbg;
static uint8_t   c;
/* local com driver configure */
static hal_com_t com_dbg = 
{
    .port_name   = CFG_LOG_DEBUG_PORT,
    .baudrate    = CFG_LOG_DEBUG_BAUDRATE,
    .priority    = CFG_LOG_DEBUG_PRIORITY,
    .irq_handler = com_irq_handler,
    .data        = &c
};
/* Exported functions ------------------------------------------------------- */
/*
 * com_irq_handler
 */
static void com_irq_handler(uint8_t _c)
{
    c = _c;
}
/*
 * log_init
 */
void log_init( void )
{
    hal_com_init(&com_dbg);
    /* enable IRQ */
    hal_com_enable_irq(&com_dbg);
}
/**
 * log_err
 */
#if(LOG_ERROR == STD_ON ) 
void log_error( const char* format, ... )
{
    __disable_irq();
    ERROR_HEADER();
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
    COLOR_ENDL();
    __enable_irq(); 
}
#endif
/*
 * log_warn
 */
#if(LOG_WARN == STD_ON )  
void log_warn( const char* format, ... )
{
    __disable_irq();
    WARN_HEADER();
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
    COLOR_ENDL();
    __enable_irq(); 
}
#endif
/**
 * log_info
 */
#if(LOG_INFO == STD_ON )  
void log_info( const char* format, ... )
{
    __disable_irq();
    INFO_HEADER();
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
    COLOR_ENDL();
    __enable_irq(); 
}
#endif
/**
 * log_debug 
 */
#if (LOG_DEBUG == STD_ON)
void log_debug( const char* format, ... )
{
    __disable_irq();
    DEBUG_HEADER();
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
    COLOR_ENDL();
    __enable_irq(); 
}
#endif
/*
 * log_msg
 */
#if (LOG_MSG == STD_ON) 
void log_msg( const char* format, ... )
{
    __disable_irq();
    va_list argptr;
    va_start(argptr, format);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
    __enable_irq();
}
#endif
/**
 * @brief  Outputs a formatted string on the DBGU stream, using a variable number of
 *         arguments.
 *
 * @param  pFormat  Format string.
 */
signed int printf(const char *pFormat, ...)
{
    va_list ap;
    signed int result;
  __disable_irq();
    /* Forward call to vprintf */
    va_start(ap, pFormat);
    result = vfprintf(stdout, pFormat, ap);
    va_end(ap);
  __enable_irq();
    return result;
}
/* retarget printf for arm gcc */

/*
 * _read
 */
int _read(int file, char *ptr, int len) 
{
  int CharCnt = 0x00;
  while(CharCnt < len) 
  {
      *ptr++ = hal_com_readbyte(&com_dbg);
      CharCnt++;
  }
  return CharCnt;  
}
/*
 * _write
 */
int _write(int file, char *ptr, int len) 
{
    int n;
    for (n = 0; n < len; n++) 
    {
        *(com_dbg.data) = *ptr++;
        hal_com_sendbyte(&com_dbg);
    }
    return n;
}

