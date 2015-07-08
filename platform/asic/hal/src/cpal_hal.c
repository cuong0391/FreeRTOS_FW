/**
  ******************************************************************************
  * @file    STM32_CPAL_Driver/src/cpal_hal.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    21-December-2012
  * @brief   This file contains all the functions for the CPAL_HAL common
  *          firmware layer.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
/* If STM32F4XX family is used */   
#include "stm32f4xx.h"
#include "misc.h"
#include "cpal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Configure NVIC Priority Group.
  * @param  None.
  * @retval None. 
  */
void CPAL_HAL_NVICInit(void)
{
 /* Set NVIC Group Priority */
  NVIC_PriorityGroupConfig (CPAL_NVIC_PRIOGROUP);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
