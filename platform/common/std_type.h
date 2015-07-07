/**
  ******************************************************************************
  * @file    types.h
  * @author  Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    18-Mar-2015
  * @brief   Types definition
  ******************************************************************************
  */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TYPES_H
#define __TYPES_H

#ifdef __cplusplus
 extern "C" {
#endif
                                              
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported define------------------------------------------------------------*/
#ifndef STD_ON
#define STD_ON  1
#endif

#ifndef STD_OFF
#define STD_OFF 0
#endif

#define E_OK                    0
#define E_NOT_OK                1

/* Exported typedef  -------------------------------------------------------- */
/**
 * @brief flags_t
 *
 */
typedef enum __flags {
    FLAG_TX_COMPLETED   = 1<<1,
    FLAG_RX_COMPLETED   = 1<<2,
    FLAG_READ_COMPLETED = 1<<3,
    FLAG_WRITE_COMPLETED = 1<<4,
}flags_t;

#endif /* __TYPES_H */

/*********** Portions COPYRIGHT 2015 Light. Co., Ltd.*****END OF FILE****/
