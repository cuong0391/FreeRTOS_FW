/**
  ******************************************************************************
  * @file    __HAL_COM_TEST.c
  * @author  Embedded Team CopyRight@NTC
  * @version V1.0.0
  * @date    June-12-2015
  * @brief   This file expandes for Test HAL_COM driver
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
#include "hal_com.h"

/* Private define ----------------------------------------------------------- */
#define COM_MAX_DATA_SIZE               8

/* Private tyepdef ---------------------------------------------------------- */
typedef struct _com_buf {
    uint16_t    idx;
    uint8_t     data[COM_MAX_DATA_SIZE];
}com_buf;

/* Private functions -------------------------------------------------------- */

/* irq handler function is implemented here */
static void com_irq_handler(uint8_t c);
static void com_putstring(hal_com_t *com,uint8_t *str);
static void com_putchar(hal_com_t *com,uint8_t byte);

/* Private variables ---------------------------------------------------------*/

static com_buf   com_data;
/* local com driver configure */
static hal_com_t com = 
{
    .port_name   = COM1,
    .baudrate    = COM_BAUD_115200,
    .priority    = COM_PRIO_1,
    .irq_handler = com_irq_handler,
    .data        = com_data.data
};

/* Exported functions ------------------------------------------------------- */


/**
  * @brief __HAL_COM_TEST
  * The function is implemented to test COM driver
  * @param None
  * @retval None
  */
void __HAL_COM_TEST (void)
{
    hal_com_init(&com);
    /* enable IRQ */
    hal_com_enable_irq(&com);
    /* putstring */
    com_putstring(&com,"Hello World\r\n");
    while(1)
    {
        /* Loopback test, data recived in interrupt */
        if(com_data.idx)
        {
            com_putstring(&com,com_data.data);
            com_data.data[com_data.idx] = 0;
            com_data.idx = 0;
        }
    }
}

/**
  * @brief main function
  * The function is implemented to test COM driver
  * @param None
  * @retval None
  */
int main (void)
{
    __HAL_COM_TEST();
    return 0;
}

/**
  * @brief com_irq_handler
  * The function is implemented to test COM driver
  * @param None
  * @retval None
  */
static void com_irq_handler(uint8_t c)
{
    if(com_data.idx < COM_MAX_DATA_SIZE)
    {
        com_data.data[com_data.idx++] = c;        
    }
    else
    {
        com_data.idx = 0;
    }

}

/**
  * @brief com_putstring
  * The function is implemented to test COM driver
  * @param None
  * @retval None
  */

void com_putstring(hal_com_t *com,uint8_t *str)
{
    while(*str)
    {
        com_data.data[0] = *str++;
        hal_com_sendbyte(com);
    }
}

/**
  * @brief com_putchar
  * The function is implemented to test COM driver
  * @param None
  * @retval None
  */

void com_putchar(hal_com_t *com, uint8_t byte)
{
    com_data.data[0] = byte;
    hal_com_sendbyte(com);
}

