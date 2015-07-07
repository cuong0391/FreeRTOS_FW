/**
 ******************************************************************************
 * @file    hal_spi.c
 * @author  Embedded Team CopyRight@NTC
 * @version V1.0.0
 * @date    June-04-2015
 * @brief   This file expandes for #SPI driver
 ******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "hal_spi.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h" 
#include "stm32f4xx_dma.h"
#include "misc.h"
#include "log.h"
/* Private define------------------------------------------------------------*/

/* Private Typedef------------------------------------------------------------*/
/* pin hardware configuration */
typedef struct _spi_hw_pin_config {
    uint32_t               Clock;
    GPIO_TypeDef           *Port;
    uint16_t               PinNum;
    uint16_t               AFPin;

}spi_hw_pin;

/* hardware COM configuration */
typedef struct _spi_hw_config {
   SPI_TypeDef*         SPIBase;
   uint32_t             ClockMask;
   uint8_t              IRQ;
   uint8_t              GPIO_AFFunc;
   spi_hw_pin           PIN_MOSI;
   spi_hw_pin           PIN_MISO;
   spi_hw_pin           PIN_SCK;
   spi_hw_pin           PIN_NSS;
}spi_hw_config;

/* hardware dma configuration*/
typedef struct _dma_hw_ch{
    uint32_t TX_Channel;
    DMA_Stream_TypeDef* TX_Stream;
    uint32_t RX_Channel;
    DMA_Stream_TypeDef* RX_Stream;
}dma_hw_ch_t;
typedef struct _dma_irq_config
{
    uint8_t     DMA_TX_IRQn;
    uint8_t     DMA_RX_IRQn;
    uint32_t    DMA_IT_TX_TCIFx;
    uint32_t    DMA_IT_RX_TCIFx;
}dma_irq_config_t;
typedef struct _dma_hw_config
{
    dma_hw_ch_t channel;
    dma_irq_config_t irq;
}_dma_hw_config_t;
/* direction transfer */
typedef enum _spi_transfer {
    SPI_TX,
    SPI_RX
}spi_transfer_t;

/* Operation mode handle structure */
typedef struct _spi_isr_handle
{
    hal_spi_status_t                 sm;
    spi_transfer_t                   dir;           /* transfer dir */
    volatile hal_spi_master_buffer_t *t_data;       /* handler of data from upper layer */
    hal_spi_operation_mode_t         omode;
    void                             (*func)(hal_spi_status_t status);    /**< call back function to upper layer */
}spi_isr_handle_t;
/* Private variable- ---------------------------------------------------------*/
/* Private function- ---------------------------------------------------------*/
/*
 * hal_spi_send_block
 * SPI transfer data
 */
static int hal_spi_send_block(hal_spi_t *spi, hal_spi_master_buffer_t * buf );
/*
 * hal_spi_read_block
 * SPI read data
 */
static int hal_spi_read_block(hal_spi_t *spi, hal_spi_master_buffer_t * buf );
/*
 * hal_spi_irq_handler
 * SPI interrupt handler
 */

static void hal_spi_irq_handler(hal_spi_channel_t chid);
/*
 * hal_spi_dma_tx
 * SPI dma transfer
 */
static int hal_spi_dma_tx(hal_spi_t *spi, hal_spi_master_buffer_t *buff);
/*
 * hal_spi_dma_rx
 * SPI dma receive data
 */
static int hal_spi_dma_rx(hal_spi_t *spi, hal_spi_master_buffer_t *buff);
/*
 * hal_spi_dma_tx_irq_en
 * SPI dma enable tx irq hw
 */
static void hal_spi_dma_tx_irq_en(hal_spi_t *spi);
/*
 * hal_spi_dma_rx_irq_en
 * SPI dma enable rx irq hw
 */
static void hal_spi_dma_rx_irq_en(hal_spi_t *spi);
/*
 * hal_spi_dma_init
 * initializing dma
 */
static void hal_spi_dma_init(hal_spi_channel_t chid);
/*
 * hal_spi_irq_enable
 * spi enable irq hw
 */
static int hal_spi_irq_enable(hal_spi_t *spi);
/* Private variables ---------------------------------------------------------*/
DMA_InitTypeDef DMA_InitStruct;
/* spi irq handle */
static spi_isr_handle_t irq_hdl[HAL_SPI_MAX_IDX];
/* spi hardware configure */
 static const spi_hw_config  hw_config[] =
{
    /* SPI1 */
    {
        .SPIBase      =  SPI1,
        .ClockMask    =  RCC_APB2Periph_SPI1,
        .IRQ          =  SPI1_IRQn,
        .GPIO_AFFunc  = GPIO_AF_SPI1,
        /* MOSI pin */
        {
            .Clock = RCC_AHB1Periph_GPIOA,
            .Port  = GPIOA,
            .PinNum= GPIO_Pin_7,
            .AFPin = GPIO_PinSource7,
        },
        /* MISO pin */
        {
            .Clock = RCC_AHB1Periph_GPIOA,
            .Port  = GPIOA,
            .PinNum= GPIO_Pin_6,
            .AFPin = GPIO_PinSource6,
        },
        /* SCK pin */
        {
            .Clock = RCC_AHB1Periph_GPIOA,
            .Port  = GPIOA,
            .PinNum= GPIO_Pin_5,
            .AFPin = GPIO_PinSource5,
        },
        /*NSS Pin*/
        {
            .Clock = RCC_AHB1Periph_GPIOA,
            .Port  = GPIOA,
            .PinNum= GPIO_Pin_4,
            .AFPin = GPIO_PinSource4,
        }
    },
    /* SPI2 */
    {
        .SPIBase      =  SPI2,
        .ClockMask    =  RCC_APB1Periph_SPI2,
        .IRQ          =  SPI2_IRQn,
        .GPIO_AFFunc  =  GPIO_AF_SPI2,
        /* MOSI pin */
        {
            .Clock = RCC_AHB1Periph_GPIOB,
            .Port  = GPIOB,
            .PinNum= GPIO_Pin_15,
            .AFPin = GPIO_PinSource15,
        },
        /* MISO pin */
        {
            .Clock = RCC_AHB1Periph_GPIOB,
            .Port  = GPIOB,
            .PinNum= GPIO_Pin_14,
            .AFPin = GPIO_PinSource14,
        },
        /* SCK pin */
        {
            .Clock = RCC_AHB1Periph_GPIOB,
            .Port  = GPIOB,
            .PinNum= GPIO_Pin_13,
            .AFPin = GPIO_PinSource13,
        },
        /*NSS Pin*/
        {
            .Clock = RCC_AHB1Periph_GPIOB,
            .Port  = GPIOB,
            .PinNum= GPIO_Pin_12,
            .AFPin = GPIO_PinSource12,
        }
    },
    /* SPI3 */
    {
        .SPIBase      =  SPI3,
        .ClockMask    =  RCC_APB1Periph_SPI3,
        .IRQ          =  SPI3_IRQn,
        .GPIO_AFFunc  =  GPIO_AF_SPI3,
        /* MOSI pin */
        {
            .Clock = RCC_AHB1Periph_GPIOC,
            .Port  = GPIOC,
            .PinNum= GPIO_Pin_12,
            .AFPin = GPIO_PinSource12,
        },
        /* MISO pin */
        {
            .Clock = RCC_AHB1Periph_GPIOC,
            .Port  = GPIOC,
            .PinNum= GPIO_Pin_11,
            .AFPin = GPIO_PinSource11,
        },
        /* SCK pin */
        {
            .Clock = RCC_AHB1Periph_GPIOC,
            .Port  = GPIOC,
            .PinNum= GPIO_Pin_10,
            .AFPin = GPIO_PinSource10,
        },
        /*NSS Pin*/
        {
            .Clock = RCC_AHB1Periph_GPIOA,
            .Port  = GPIOA,
            .PinNum= GPIO_Pin_4,
            .AFPin = GPIO_PinSource4,
        }
    },
    /* SPI4 */
    {
        .SPIBase      =  SPI4,
        .ClockMask    =  RCC_APB2Periph_SPI4,
        .IRQ          =  SPI4_IRQn,
        .GPIO_AFFunc  =  GPIO_AF_SPI4,
        /* MOSI pin */
        {
            .Clock = RCC_AHB1Periph_GPIOE,
            .Port  = GPIOE,
            .PinNum= GPIO_Pin_6,
            .AFPin = GPIO_PinSource6,
        },
        /* MISO pin */
        {
            .Clock = RCC_AHB1Periph_GPIOE,
            .Port  = GPIOE,
            .PinNum= GPIO_Pin_5,
            .AFPin = GPIO_PinSource5,
        },
        /* SCK pin */
        {
            .Clock = RCC_AHB1Periph_GPIOE,
            .Port  = GPIOE,
            .PinNum= GPIO_Pin_2,
            .AFPin = GPIO_PinSource2,
        },
        /*NSS Pin*/
        {
            .Clock = RCC_AHB1Periph_GPIOE,
            .Port  = GPIOE,
            .PinNum= GPIO_Pin_4,
            .AFPin = GPIO_PinSource4,
        }
    },
    /* SPI5 */
    {
        .SPIBase      =  SPI5,
        .ClockMask    =  RCC_APB2Periph_SPI5,
        .IRQ          =  SPI5_IRQn,
        .GPIO_AFFunc  =  GPIO_AF_SPI5,
        /* MOSI pin */
        {
            .Clock = RCC_AHB1Periph_GPIOF,
            .Port  = GPIOF,
            .PinNum= GPIO_Pin_9,
            .AFPin = GPIO_PinSource9,
        },
        /* MISO pin */
        {
            .Clock = RCC_AHB1Periph_GPIOF,
            .Port  = GPIOF,
            .PinNum= GPIO_Pin_8,
            .AFPin = GPIO_PinSource8,
        },
        /* SCK pin */
        {
            .Clock = RCC_AHB1Periph_GPIOH,
            .Port  = GPIOH,
            .PinNum= GPIO_Pin_6,
            .AFPin = GPIO_PinSource6,
        },
        /*NSS Pin*/
        {
            .Clock = RCC_AHB1Periph_GPIOH,
            .Port  = GPIOH,
            .PinNum= GPIO_Pin_5,
            .AFPin = GPIO_PinSource5,
        }
    },
    /* SPI6 */
    {
        .SPIBase      =  SPI6,
        .ClockMask    =  RCC_APB2Periph_SPI6,
        .IRQ          =  SPI6_IRQn,
        .GPIO_AFFunc  =  GPIO_AF_SPI6,
        /* MOSI pin */
        {
            .Clock = RCC_AHB1Periph_GPIOG,
            .Port  = GPIOG,
            .PinNum= GPIO_Pin_14,
            .AFPin = GPIO_PinSource14,
        },
        /* MISO pin */
        {
            .Clock = RCC_AHB1Periph_GPIOG,
            .Port  = GPIOG,
            .PinNum= GPIO_Pin_12,
            .AFPin = GPIO_PinSource12,
        },
        /* SCK pin */
        {
            .Clock = RCC_AHB1Periph_GPIOG,
            .Port  = GPIOG,
            .PinNum= GPIO_Pin_13,
            .AFPin = GPIO_PinSource13,
        },
        /*NSS Pin*/
        {
            .Clock = RCC_AHB1Periph_GPIOG,
            .Port  = GPIOG,
            .PinNum= GPIO_Pin_8,
            .AFPin = GPIO_PinSource8,
        }
    }
};
static const _dma_hw_config_t spi_dma_hw[] =
{
        /* DMA_SPI1_CH1*/
        {
                .channel={
                        .TX_Channel = DMA_Channel_3,
                        .TX_Stream  = DMA2_Stream3,
                        .RX_Channel = DMA_Channel_3,
                        .RX_Stream  = DMA2_Stream2
                },
                .irq = {
                        .DMA_TX_IRQn = DMA2_Stream3_IRQn,
                        .DMA_RX_IRQn = DMA2_Stream2_IRQn,
                        .DMA_IT_TX_TCIFx= DMA_IT_TCIF3,
                        .DMA_IT_RX_TCIFx= DMA_IT_TCIF2,
                }
        },
        /*DMA_SPI2_CH2*/
        {
                .channel={
                        .TX_Channel = DMA_Channel_0,
                        .TX_Stream  = DMA1_Stream4,
                        .RX_Channel = DMA_Channel_0,
                        .RX_Stream  = DMA1_Stream3
                },
                .irq ={
                        .DMA_TX_IRQn = DMA1_Stream4_IRQn,
                        .DMA_RX_IRQn = DMA1_Stream3_IRQn,
                        .DMA_IT_TX_TCIFx= DMA_IT_TCIF4,
                        .DMA_IT_RX_TCIFx= DMA_IT_TCIF3,
                }

        },
        /*DMA_SPI3_CH3*/
        {
                .channel={
                        .TX_Channel = DMA_Channel_0,
                        .TX_Stream  = DMA1_Stream5,
                        .RX_Channel = DMA_Channel_0,
                        .RX_Stream  = DMA1_Stream0
                },
                .irq = {
                        .DMA_TX_IRQn = DMA1_Stream5_IRQn,
                        .DMA_RX_IRQn = DMA1_Stream0_IRQn,
                        .DMA_IT_TX_TCIFx= DMA_IT_TCIF5,
                        .DMA_IT_RX_TCIFx= DMA_IT_TCIF0,
                }
        },
        /*DMA_SPI4_CH4*/
        {
                .channel={
                        .TX_Channel = DMA_Channel_4,
                        .TX_Stream  = DMA2_Stream1,
                        .RX_Channel = DMA_Channel_4,
                        .RX_Stream  = DMA2_Stream0
                },
                .irq = {
                        .DMA_TX_IRQn = DMA2_Stream1_IRQn,
                        .DMA_RX_IRQn = DMA2_Stream0_IRQn,
                        .DMA_IT_TX_TCIFx= DMA_IT_TCIF1,
                        .DMA_IT_RX_TCIFx= DMA_IT_TCIF0,
                }
        },
        /*DMA_SPI5_CH5*/
        {
                .channel={
                        .TX_Channel = DMA_Channel_7,
                        .TX_Stream  = DMA2_Stream6,
                        .RX_Channel = DMA_Channel_7,
                        .RX_Stream  = DMA2_Stream5
                },
                .irq = {
                        .DMA_TX_IRQn = DMA2_Stream6_IRQn,
                        .DMA_RX_IRQn = DMA2_Stream5_IRQn,
                        .DMA_IT_TX_TCIFx= DMA_IT_TCIF6,
                        .DMA_IT_RX_TCIFx= DMA_IT_TCIF5,
                }
        },
        /*DMA_SP6_CH6*/
        {
                .channel={
                        .TX_Channel = DMA_Channel_1,
                        .TX_Stream  = DMA2_Stream5,
                        .RX_Channel = DMA_Channel_1,
                        .RX_Stream  = DMA2_Stream6
                },
                .irq ={
                        .DMA_TX_IRQn = DMA2_Stream5_IRQn,
                        .DMA_RX_IRQn = DMA2_Stream6_IRQn,
                        .DMA_IT_TX_TCIFx= DMA_IT_TCIF5,
                        .DMA_IT_RX_TCIFx= DMA_IT_TCIF6,
                }
        }
};
/* Exported functions --------------------------------------------------------*/

/*
 * hal_spi_init
 * Initializes SPIx peripheral
 */
int hal_spi_init(hal_spi_t *spi)
{
    int ret;
    uint8_t chid;
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;    
#if (HAL_SPI_DEBUG == STD_ON)
    if((void *)0 == spi)
    {
        ret = (int)SPI_NULL_PTR;
    }
    else if(HAL_SPI_MAX_IDX <= spi->chid || HAL_SPI_CH1 > spi->chid )
    {
        ret = (int)SPI_INVALID_CHANNEL;
    }
    else
#endif /* HAL_SPI_DEBUG */
    {
        chid = spi->chid;
        RCC_AHB1PeriphClockCmd(hw_config[chid].PIN_MOSI.Clock,ENABLE);
        RCC_AHB1PeriphClockCmd(hw_config[chid].PIN_MISO.Clock,ENABLE);
        RCC_AHB1PeriphClockCmd(hw_config[chid].PIN_SCK.Clock,ENABLE);
        /* if we configure spi to slave mode,we'll use nss hard*/
        uint8_t af_func = hw_config[chid].GPIO_AFFunc & 0xff;
        if(SPI_SLAVE == spi->omode)
        {
            RCC_AHB1PeriphClockCmd(hw_config[chid].PIN_NSS.Clock,ENABLE);
            GPIO_PinAFConfig(
                             hw_config[chid].PIN_NSS.Port,
                             hw_config[chid].PIN_NSS.AFPin,
                             af_func);
        }
        /* AF Config*/
        GPIO_PinAFConfig(
                      hw_config[chid].PIN_MOSI.Port,
                      hw_config[chid].PIN_MOSI.AFPin,
                      af_func);
        GPIO_PinAFConfig(
                      hw_config[chid].PIN_MISO.Port,
                      hw_config[chid].PIN_MISO.AFPin,
                      af_func);
        GPIO_PinAFConfig(
                      hw_config[chid].PIN_SCK.Port,
                      hw_config[chid].PIN_SCK.AFPin,
                      af_func);
        /* GPIO configure */
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

        /* MOSI pin */
        GPIO_InitStructure.GPIO_Pin = hw_config[chid].PIN_MOSI.PinNum;
        GPIO_Init(hw_config[chid].PIN_MOSI.Port,&GPIO_InitStructure);
        /* MISO pin */
        GPIO_InitStructure.GPIO_Pin = hw_config[chid].PIN_MISO.PinNum;
        GPIO_Init(hw_config[chid].PIN_MISO.Port,&GPIO_InitStructure);
        /* SCK */
        GPIO_InitStructure.GPIO_Pin = hw_config[chid].PIN_SCK.PinNum;
        GPIO_Init(hw_config[chid].PIN_SCK.Port,&GPIO_InitStructure);
        /*NSS*/
        if(SPI_SLAVE == spi->omode)
        {
            GPIO_InitStructure.GPIO_Pin = hw_config[chid].PIN_NSS.PinNum;
            GPIO_Init(hw_config[chid].PIN_NSS.Port,&GPIO_InitStructure);
        }
        /* enable clock for SPI peripheral */
        if(hw_config[chid].ClockMask != RCC_APB1Periph_SPI2 || hw_config[chid].ClockMask != RCC_APB1Periph_SPI3)
        {
            RCC_APB2PeriphClockCmd(hw_config[chid].ClockMask,ENABLE);
        }
        else
        {
            RCC_APB1PeriphClockCmd(hw_config[chid].ClockMask,ENABLE);
        }
        if( SPI_MASTER == spi->omode)
        {
            /* SPI Configure */
            SPI_InitStructure.SPI_BaudRatePrescaler = (uint16_t)spi->clock_speed;
            SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
            SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
            SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
            SPI_InitStructure.SPI_CRCPolynomial     = 7;
            if(spi->datasize == DATASIZE_16b)
            {
              SPI_InitStructure.SPI_DataSize        = SPI_DataSize_16b;
            }
            else
            {
                SPI_InitStructure.SPI_DataSize       = SPI_DataSize_8b;
            }
            if((spi->mode==SPI_MODE_0_MSB) || (spi->mode==SPI_MODE_0_LSB))
            {
              SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
              SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
            }
            else if((spi->mode==SPI_MODE_1_MSB) || (spi->mode==SPI_MODE_1_LSB))
            {
              SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
              SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
            }
            else if((spi->mode==SPI_MODE_2_MSB) || (spi->mode==SPI_MODE_2_LSB))
            {
              SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
              SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
            }
            else
            {
              SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
              SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
            }
            if((spi->mode==SPI_MODE_0_MSB) || (spi->mode==SPI_MODE_1_MSB) ||
               (spi->mode==SPI_MODE_2_MSB) || (spi->mode==SPI_MODE_3_MSB))
            {
              SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
            }
            else
            {
              SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
            }
        } 
        else
        {
            /* SPI Configure */
            SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
            SPI_InitStructure.SPI_Mode              = SPI_Mode_Slave;
            SPI_InitStructure.SPI_NSS               = SPI_NSS_Hard;
            SPI_InitStructure.SPI_CRCPolynomial     = 7; 
            if(spi->datasize == DATASIZE_16b)
            {
              SPI_InitStructure.SPI_DataSize        = SPI_DataSize_16b;
            }
            else
            {
              SPI_InitStructure.SPI_DataSize        = SPI_DataSize_8b;
            }
            if((spi->mode==SPI_MODE_0_MSB) || (spi->mode==SPI_MODE_0_LSB))
            {
              SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
              SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
            }
            else if((spi->mode==SPI_MODE_1_MSB) || (spi->mode==SPI_MODE_1_LSB))
            {
              SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
              SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
            }
            else if((spi->mode==SPI_MODE_2_MSB) || (spi->mode==SPI_MODE_2_LSB))
            {
              SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
              SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
            }
            else
            {
              SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
              SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
            }
            if((spi->mode==SPI_MODE_0_MSB) || (spi->mode==SPI_MODE_1_MSB) ||
               (spi->mode==SPI_MODE_2_MSB) || (spi->mode==SPI_MODE_3_MSB))
            {
              SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
            }
            else
            {
              SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
            }
            SPI_SSOutputCmd(hw_config[chid].SPIBase,DISABLE);
        }
        SPI_Init(hw_config[chid].SPIBase,&SPI_InitStructure);
        /* configure dma if it's enable*/
        if(spi->dma_status == ENABLE_DMA)
        {
             hal_spi_dma_init(chid);
            /*configure tx irq dma*/
             hal_spi_dma_tx_irq_en(spi);
            /*configure rx irq dma*/
             hal_spi_dma_rx_irq_en(spi);
        }else
        {
            /* configure irq for spi*/
            hal_spi_irq_enable(spi);
        }
        SPI_Cmd(hw_config[chid].SPIBase,ENABLE);
        irq_hdl[chid].sm = SPI_READY;
        irq_hdl[chid].func = spi->func_callback;
        ret = (int)SPI_OK;
    }
    return ret;
}
int hal_spi_xfer(hal_spi_t *spi, hal_spi_master_buffer_t *buff)
{
    uint16_t ret;
    if(spi->dma_status == ENABLE_DMA)
    {
        ret=hal_spi_dma_tx(spi,buff);
    }else
    {
        ret=hal_spi_send_block(spi,buff);
    }
    return ret;
}
/*
 * hal_spi_xfer
 * The function does receive data
 */
int hal_spi_xreceive(hal_spi_t *spi,hal_spi_master_buffer_t *buff)
{
    uint16_t ret;
    if(spi->dma_status == ENABLE_DMA)
    {
        ret=hal_spi_dma_rx(spi,buff);
    }else
    {
        ret=hal_spi_read_block(spi,buff);
    }
    return ret;
}
/*
 * hal_spi_deinit
 * The function does free spi resource
 */
int hal_spi_deinit(hal_spi_t *spi)
{
    int ret;
#if (HAL_SPI_DEBUG == STD_ON)
    if((void *)0 == spi)
    {
        ret = (int)SPI_NULL_PTR;
    }
    else if(HAL_SPI_MAX_IDX <= spi->chid || HAL_SPI_CH1 > spi->chid )
    {
        ret = (int)SPI_INVALID_CHANNEL;
    }
    else
#endif /* HAL_SPI_DEBUG */
    {
        /* enable clock for SPI peripheral */
        RCC_AHB2PeriphClockCmd(hw_config[spi->chid].ClockMask,DISABLE);
        SPI_DeInit(hw_config[spi->chid].SPIBase);
        ret = (int)SPI_OK;
    }
    return ret;
}

/*
 * hal_spi_send_byte
 * The function transfer byte on SPI channel, included Transmit and Receive 
 * make surce that #CS pin of slave device is presented before call this function  
 */
uint8_t hal_spi_transfer(hal_spi_channel_t chid, uint8_t byte )
{
    uint32_t TimeOut = HAL_SPI_TIMEOUT;
    /* Tx */
    while (TimeOut-- && !(hw_config[chid].SPIBase->SR & SPI_SR_TXE));
    hw_config[chid].SPIBase->DR = byte;

    /* Rx */
    TimeOut = HAL_SPI_TIMEOUT;
    while (TimeOut-- && !(hw_config[chid].SPIBase->SR & SPI_SR_RXNE));
    return hw_config[chid].SPIBase->DR;    
}

/*
 * hal_spi_send_block
 * The function sends block data to slave device, make surce that #CS pin of slave 
 * device is presented before call this function
 */
static int hal_spi_send_block(hal_spi_t *spi, hal_spi_master_buffer_t * buf )
{
    int ret;
    uint8_t chid;
    chid = spi->chid;
#if (HAL_SPI_DEBUG == STD_ON)
    if((void*) 0 == buf )
    {
        ret = (int)SPI_NULL_PTR;
    }
    else if (HAL_SPI_MAX_IDX <= chid)
    {
        ret =(int)SPI_INVALID_CHANNEL;
    }
    else if(SPI_READY!=irq_hdl[chid].sm)
    {
        ret=(int)SPI_CHANNEL_BUSY;
    }
    else if(buf->txbuf.len <1)
    {
        ret= (int)SPI_INVALID_LENGTH;
    }
#endif
    if(SPI_I2S_GetFlagStatus(hw_config[chid].SPIBase,SPI_I2S_FLAG_BSY))
    {
        ret = (int)SPI_CHANNEL_BUSY;
    }
    else
    {
        irq_hdl[chid].t_data            = buf;
        irq_hdl[chid].sm                = SPI_TRANSFERING;
        irq_hdl[chid].dir               = SPI_TX;
        (*irq_hdl[chid].func)((uint16_t)SPI_TRANSFERING);
        if(spi->omode == SPI_MASTER)
        {
            irq_hdl[chid].omode  = SPI_MASTER;
        }
        else
        {
            irq_hdl[chid].omode  = SPI_SLAVE;
        }
        /*enable interrupt hw*/
        SPI_I2S_ITConfig(hw_config[chid].SPIBase,SPI_I2S_IT_TXE,ENABLE);
        ret = (int) SPI_OK;
    }
    return ret;
}

/*
 * hal_spi_read_block
 * The function reads block data from slave device, make surce that #CS pin of slave 
 * device is presented before call this function
 */
static int hal_spi_read_block(hal_spi_t *spi, hal_spi_master_buffer_t * buf )
{
    int ret;
    uint8_t chid;
    chid = spi->chid;
#if (HAL_SPI_DEBUG == STD_ON)
    if((void*) 0 == buf )
    {
        ret = (int)SPI_NULL_PTR;
    }
    else if (HAL_SPI_MAX_IDX <= chid)
    {
        ret =(int)SPI_INVALID_CHANNEL;
    }
    else if(SPI_READY!=irq_hdl[chid].sm)
    {
        ret=(int)SPI_CHANNEL_BUSY;
    }
    else if(buf->txbuf.len <1)
    {
        ret= (int)SPI_INVALID_LENGTH;
    }
#endif
    if(SPI_I2S_GetFlagStatus(hw_config[chid].SPIBase,SPI_I2S_FLAG_BSY))
    {
        ret = (int)SPI_CHANNEL_BUSY;
    }
    else
    {
        irq_hdl[chid].t_data            = buf;
        irq_hdl[chid].sm                = SPI_TRANSFERING;
        irq_hdl[chid].dir               = SPI_RX;
        (*irq_hdl[chid].func)((uint16_t)SPI_TRANSFERING);
        if(spi->omode == SPI_MASTER)
        {
            irq_hdl[chid].omode  = SPI_MASTER;
        }
        else
        {
            irq_hdl[chid].omode  = SPI_SLAVE;
        }
        /*clear any pending Rx */
        SPI_I2S_ReceiveData(hw_config[chid].SPIBase);
        SPI_I2S_ITConfig(hw_config[chid].SPIBase,SPI_I2S_IT_RXNE,ENABLE);
        ret = (int) SPI_OK;
    }
    return ret;
}

/*
 * hal_spi_irq_enable
 * The function enable SPI interrupt
 */
static int hal_spi_irq_enable(hal_spi_t *spi)
{
    int ret;
#if (HAL_SPI_DEBUG ==STD_ON)
    if((void*)0 == spi)
    {
        ret =(int) SPI_NULL_PTR;
    }
    else if(HAL_SPI_MAX_IDX <=spi->chid)
    {
        ret = (int)SPI_INVALID_CHANNEL;
    }
    else if(HAL_SPI_P_MAX <=spi->priority)
    {
        ret =(int)SPI_INVALID_PRIORITY;
    }
#endif
    NVIC_InitTypeDef  NVIC_InitStructure;
    /* Configure the Priority Group to 4 bit */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    /* Configure the SPI interrupt priority */
    NVIC_InitStructure.NVIC_IRQChannel =hw_config[spi->chid].IRQ ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = spi->priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    ret=(int)SPI_OK;
    return ret;
}
static void hal_spi_dma_init(hal_spi_channel_t chid)
{
    /* Enable DMA TX Clock*/
    if(spi_dma_hw[chid].channel.TX_Stream > DMA2_Stream0)
    {
        /*Enable DMA2 Cock*/
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
    }
    else
    {
        /*Enable DMA1 Clock*/
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
    }
    /* Enable DMA RX Clock*/
    if(spi_dma_hw[chid].channel.RX_Stream >= DMA2_Stream0)
    {
        /*Enable DMA2 Cock*/
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
    }
    else
    {
        /*Enable DMA1 Clock*/
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
    }
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
}

static int hal_spi_dma_tx(hal_spi_t *spi, hal_spi_master_buffer_t *buff)
{
    uint8_t chid;

    chid = spi->chid;
    if(spi_dma_hw[chid].channel.TX_Stream->NDTR|| buff->txbuf.buf ==0 ||buff->txbuf.len==0)
    {
        return SPI_INVALID_LENGTH;
    }
    if(SPI_READY !=hal_spi_get_status(chid))
    {
        return SPI_CHANNEL_BUSY;
    }else
    {
        uint8_t dummy =0x00;
        DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &hw_config[chid].SPIBase->DR;
        DMA_InitStruct.DMA_BufferSize = buff->txbuf.len;

        DMA_InitStruct.DMA_Channel = spi_dma_hw[chid].channel.TX_Channel;
        DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
        if(buff->txbuf.buf !=0)
        {
            DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)buff->txbuf.buf; // transfer tx buffer
            DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable; // increase pointer;
        }else
        {
            DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)&dummy; // transfer tx buffer
            DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable; // not increase pointer;
        }
        DMA_DeInit(spi_dma_hw[chid].channel.TX_Stream);
        DMA_Init(spi_dma_hw[chid].channel.TX_Stream,&DMA_InitStruct);
        /*enable interrupt dma hw */
        DMA_ITConfig(spi_dma_hw[chid].channel.TX_Stream,DMA_IT_TC,ENABLE);
        /* DMA-TX request enable*/
        SPI_I2S_DMACmd(hw_config[chid].SPIBase, SPI_I2S_DMAReq_Tx, ENABLE);
        /*enable DMA*/
        DMA_Cmd(spi_dma_hw[chid].channel.TX_Stream,ENABLE);
        if(spi->omode == SPI_MASTER)
        {
            irq_hdl[chid].omode  = SPI_MASTER;
        }
        else
        {
            irq_hdl[chid].omode  = SPI_SLAVE;
        }
        irq_hdl[chid].dir = SPI_TX;
        irq_hdl[chid].sm  = SPI_TRANSFERING;
         (*irq_hdl[chid].func)((uint16_t)SPI_TRANSFERING);
         /*enable SPI*/
        SPI_Cmd(hw_config[chid].SPIBase,ENABLE);
    }
    return SPI_OK;
}

static int hal_spi_dma_rx(hal_spi_t *spi, hal_spi_master_buffer_t *buff)
{
    uint8_t chid;
    chid = spi->chid;
    if(spi_dma_hw[chid].channel.RX_Stream->NDTR|| buff->rxbuf.buf ==0 ||buff->rxbuf.len==0)
    {
        return SPI_INVALID_LENGTH;
    }
    if(SPI_READY !=hal_spi_get_status(chid))
    {
        return SPI_CHANNEL_BUSY;
    }
    else
    {
        uint8_t dummy =0x01;
        DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) &hw_config[chid].SPIBase->DR;
        DMA_InitStruct.DMA_BufferSize = buff->rxbuf.len;

        DMA_InitStruct.DMA_Channel = spi_dma_hw[chid].channel.RX_Channel;
        DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
        if(buff->rxbuf.buf !=0)
        {
            DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)buff->rxbuf.buf;
            DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
        }else
        {
            DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)&dummy;
            DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable;
        }
        DMA_DeInit(spi_dma_hw[chid].channel.RX_Stream);
        DMA_Init(spi_dma_hw[chid].channel.RX_Stream,&DMA_InitStruct);
        if(spi->omode == SPI_MASTER)
        {
            DMA_InitStruct.DMA_Channel = spi_dma_hw[chid].channel.TX_Channel;
            DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
            DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)&dummy;
            DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable;
            DMA_DeInit(spi_dma_hw[chid].channel.TX_Stream);
            DMA_Init(spi_dma_hw[chid].channel.TX_Stream,&DMA_InitStruct);
            /* DMA-TX request enable*/
            SPI_I2S_DMACmd(hw_config[chid].SPIBase, SPI_I2S_DMAReq_Tx, ENABLE);
            /*enable DMA*/
            DMA_Cmd(spi_dma_hw[chid].channel.TX_Stream,ENABLE);
        }
        /*enable interrupt dma hw */
        DMA_ITConfig(spi_dma_hw[chid].channel.RX_Stream,DMA_IT_TC,ENABLE);
        /* DMA-RX request enable*/
        SPI_I2S_DMACmd(hw_config[chid].SPIBase, SPI_I2S_DMAReq_Rx, ENABLE);
        /*enable DMA*/
        DMA_Cmd(spi_dma_hw[chid].channel.RX_Stream,ENABLE);

        if(spi->omode == SPI_MASTER)
        {
            irq_hdl[chid].omode  = SPI_MASTER;
        }
        else
        {
            irq_hdl[chid].omode  = SPI_SLAVE;
        }
        irq_hdl[chid].dir = SPI_RX;
        irq_hdl[chid].sm  = SPI_TRANSFERING;
         (*irq_hdl[chid].func)((uint16_t)SPI_TRANSFERING);
        /*enable SPI*/
        SPI_Cmd(hw_config[chid].SPIBase,ENABLE);
        }
    return SPI_OK;
}
/*
 * hal_spi_dma_rx_irq_en
 * DMA Rx interrupt enable
 */
static void hal_spi_dma_tx_irq_en(hal_spi_t *spi)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the SPI interrupt priority */
    NVIC_InitStructure.NVIC_IRQChannel =spi_dma_hw[spi->chid].irq.DMA_TX_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = spi->priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*
 * hal_spi_dma_rx_irq_en
 * DMA Rx interrupt enable
 */
static void hal_spi_dma_rx_irq_en(hal_spi_t *spi)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the SPI interrupt priority */
    NVIC_InitStructure.NVIC_IRQChannel =spi_dma_hw[spi->chid].irq.DMA_RX_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =spi->priority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*
 * hal_dma_irq_handler
 * SPI DMA interrupt handler
 */
void hal_dma_irq_handler(hal_spi_channel_t chid)
{
    switch (irq_hdl[chid].omode)
    {
    case SPI_MASTER:
        switch(irq_hdl[chid].dir)
        {
        case SPI_TX:
            if(DMA_GetITStatus(spi_dma_hw[chid].channel.TX_Stream,spi_dma_hw[chid].irq.DMA_IT_TX_TCIFx)== SET)
            {
                /*Disable interrupt dma hw */
                DMA_ITConfig(spi_dma_hw[chid].channel.RX_Stream,DMA_IT_TC,DISABLE);
                /* DMA-TX request disable*/
                SPI_I2S_DMACmd(hw_config[chid].SPIBase, SPI_I2S_DMAReq_Tx, DISABLE);
                irq_hdl[chid].sm = SPI_READY;
                (*irq_hdl[chid].func)(SPI_TX_BLOCK_COMPLETED);
                /* clear flag pending*/
                DMA_ClearITPendingBit(spi_dma_hw[chid].channel.TX_Stream, spi_dma_hw[chid].irq.DMA_IT_TX_TCIFx);
                DMA_Cmd(spi_dma_hw[chid].channel.RX_Stream, DISABLE);
                /* clear any pending RX*/
                hw_config[chid].SPIBase->DR;
            }
            break;
        case SPI_RX:
            if(DMA_GetITStatus(spi_dma_hw[chid].channel.RX_Stream,spi_dma_hw[chid].irq.DMA_IT_RX_TCIFx)== SET)
            {
                DMA_ITConfig(spi_dma_hw[chid].channel.RX_Stream,DMA_IT_TC,DISABLE);
                /* DMA-TX request enable*/
                SPI_I2S_DMACmd(hw_config[chid].SPIBase, SPI_I2S_DMAReq_Rx, DISABLE);
                irq_hdl[chid].sm = SPI_READY;
                (*irq_hdl[chid].func)(SPI_RX_BLOCK_COMPLETED);
                /* clear flag pending*/
                DMA_ClearITPendingBit(spi_dma_hw[chid].channel.RX_Stream, spi_dma_hw[chid].irq.DMA_IT_RX_TCIFx);
                DMA_Cmd(spi_dma_hw[chid].channel.TX_Stream, DISABLE);
            }
            break;
        }
        break;
        case SPI_SLAVE:
            switch(irq_hdl[chid].dir)
            {
            case SPI_TX:
                if(DMA_GetITStatus(spi_dma_hw[chid].channel.TX_Stream,spi_dma_hw[chid].irq.DMA_IT_TX_TCIFx)== SET)
                {
                    /*Disable interrupt dma hw */
                    DMA_ITConfig(spi_dma_hw[chid].channel.RX_Stream,DMA_IT_TC,DISABLE);
                    /* DMA-TX request disable*/
                    SPI_I2S_DMACmd(hw_config[chid].SPIBase, SPI_I2S_DMAReq_Tx, DISABLE);
                    irq_hdl[chid].sm = SPI_READY;
                    (*irq_hdl[chid].func)(SPI_TX_BLOCK_COMPLETED);
                    /* clear flag pending*/
                    DMA_ClearITPendingBit(spi_dma_hw[chid].channel.TX_Stream, spi_dma_hw[chid].irq.DMA_IT_TX_TCIFx);
                    DMA_Cmd(spi_dma_hw[chid].channel.RX_Stream, DISABLE);
                    /* clear any pending RX*/
                    hw_config[chid].SPIBase->DR;
                }
                break;
            case SPI_RX:
                if(DMA_GetITStatus(spi_dma_hw[chid].channel.RX_Stream,spi_dma_hw[chid].irq.DMA_IT_RX_TCIFx)== SET)
                {
                    DMA_ITConfig(spi_dma_hw[chid].channel.RX_Stream,DMA_IT_TC,DISABLE);
                    /* DMA-TX request enable*/
                    SPI_I2S_DMACmd(hw_config[chid].SPIBase, SPI_I2S_DMAReq_Rx, DISABLE);
                    irq_hdl[chid].sm = SPI_READY;
                    (*irq_hdl[chid].func)(SPI_RX_BLOCK_COMPLETED);
                    /* clear flag pending*/
                    DMA_ClearITPendingBit(spi_dma_hw[chid].channel.RX_Stream, spi_dma_hw[chid].irq.DMA_IT_RX_TCIFx);
                    DMA_Cmd(spi_dma_hw[chid].channel.TX_Stream, DISABLE);
                }
                break;
            }
            break;
    }
}

/*
 * hal_spi_irq_handler
 * SPI interrupt handler
 */
static void hal_spi_irq_handler(hal_spi_channel_t chid)
{
    switch(irq_hdl[chid].omode)
    {
        case SPI_MASTER:
            switch (irq_hdl[chid].dir)
            {
                case SPI_TX:
                    /* checking status register if SPI_I2S_IT_RXNE flag is set*/
                    if(SPI_I2S_GetITStatus(hw_config[chid].SPIBase,SPI_I2S_IT_TXE)!=RESET)
                    {
                        if(irq_hdl[chid].t_data->txbuf.len !=0)
                        {
                            hw_config[chid].SPIBase->DR = *(irq_hdl[chid].t_data->txbuf.buf++);
                            irq_hdl[chid].t_data->txbuf.len--;
                        }
                        else
                        {
                            irq_hdl[chid].sm = SPI_READY;
                            SPI_I2S_ITConfig(hw_config[chid].SPIBase,SPI_I2S_IT_TXE,DISABLE);
                            /* notification to upper layer tx is completed */
                            (*irq_hdl[chid].func)((uint16_t)SPI_TX_BLOCK_COMPLETED);
                        }
                    }
                    break;
                case SPI_RX:
                    /* checking status register if SPI_I2S_IT_RXNE flag is set*/
                    if(SPI_I2S_GetITStatus(hw_config[chid].SPIBase,SPI_I2S_IT_RXNE)!=RESET)
                    {
                        if(irq_hdl[chid].t_data->rxbuf.len !=0)
                        {
                            *(irq_hdl[chid].t_data->rxbuf.buf++) = hw_config[chid].SPIBase->DR;
                            irq_hdl[chid].t_data->rxbuf.len--;
                        }
                        else
                        {
                            irq_hdl[chid].sm = SPI_READY;
                            SPI_I2S_ITConfig(hw_config[chid].SPIBase,SPI_I2S_IT_RXNE,DISABLE);
                            /* notification to upper layer rx is completed */
                            (*irq_hdl[chid].func)((uint16_t)SPI_RX_BLOCK_COMPLETED);
                        }
                    }
                break;
            }
        break;
        case SPI_SLAVE:
            switch (irq_hdl[chid].dir)
            {
                case SPI_TX:
                    /* checking status register if SPI_I2S_IT_RXNE flag is set*/
                    if(SPI_I2S_GetITStatus(hw_config[chid].SPIBase,SPI_I2S_IT_TXE)!=RESET)
                    {
                        if(irq_hdl[chid].t_data->txbuf.len !=0)
                        {
                            hw_config[chid].SPIBase->DR = *(irq_hdl[chid].t_data->txbuf.buf++);
                            irq_hdl[chid].t_data->txbuf.len--;
                        }
                        else
                        {
                            irq_hdl[chid].sm = SPI_READY;
                            SPI_I2S_ITConfig(hw_config[chid].SPIBase,SPI_I2S_IT_TXE,DISABLE);
                            /* notification to upper layer tx is completed */
                            (*irq_hdl[chid].func)((uint16_t)SPI_TX_BLOCK_COMPLETED);
                        }
                    }
                    break;
                case SPI_RX:
                    /* checking status register if SPI_I2S_IT_RXNE flag is set*/
                    if(SPI_I2S_GetITStatus(hw_config[chid].SPIBase,SPI_I2S_IT_RXNE)!=RESET)
                    {
                        if(irq_hdl[chid].t_data->rxbuf.len !=0)
                        {
                            *(irq_hdl[chid].t_data->rxbuf.buf++) = hw_config[chid].SPIBase->DR;
                            irq_hdl[chid].t_data->rxbuf.len--;
                        }
                        else
                        {
                            irq_hdl[chid].sm = SPI_READY;
                            SPI_I2S_ITConfig(hw_config[chid].SPIBase,SPI_I2S_IT_RXNE,DISABLE);
                            /* notification to upper layer rx is completed */
                            (*irq_hdl[chid].func)((uint16_t)SPI_RX_BLOCK_COMPLETED);
                        }
                    }
                break;
            }
        break;
    }
}

/*
 * hal_spi_get_status
 * The function gets SPI bus status
 */
int hal_spi_get_status( hal_spi_channel_t chid )
{
    int ret;
#if (HAL_SPI_DEBUG == STD_ON)
    if(HAL_SPI_MAX_IDX <= chid )
    {
        ret = (int)SPI_INVALID_CHANNEL;
    }
    else
#endif /* HAL_SPI_DEBUG */
    {
        ret = (int)irq_hdl[chid].sm;
    }
    return ret;
}

/*
 * DMA2_Stream3_IRQHandler
 * Hardware SPI DMA interrupt handler
 */
void DMA2_Stream3_IRQHandler(void)
{
    hal_dma_irq_handler(HAL_SPI_CH1);
}

/*
 * DMA2_Stream3_IRQHandler
 * Hardware SPI DMA interrupt handler
 */
void DMA2_Stream2_IRQHandler(void)
{
    hal_dma_irq_handler(HAL_SPI_CH1);
}

/*
 * DMA1_Stream4_IRQHandler
 * Hardware SPI DMA interrupt handler
 */
void DMA1_Stream4_IRQHandler(void)
{
    hal_dma_irq_handler(HAL_SPI_CH2);
}

/*
 * DMA1_Stream3_IRQHandler
 * Hardware SPI DMA interrupt handler
 */
void DMA1_Stream3_IRQHandler(void)
{
    hal_dma_irq_handler(HAL_SPI_CH2);
}

/*
 * DMA1_Stream5_IRQHandler
 * Hardware SPI DMA interrupt handler
 */
void DMA1_Stream5_IRQHandler(void)
{
    hal_dma_irq_handler(HAL_SPI_CH3);
}

/*
 * DMA1_Stream0_IRQHandler
 * Hardware SPI DMA interrupt handler
 */
void DMA1_Stream0_IRQHandler(void)
{
    hal_dma_irq_handler(HAL_SPI_CH3);
}
/*
 * DMA2_Stream1_IRQHandler
 * Hardware SPI DMA interrupt handler
 */
void DMA2_Stream1_IRQHandler(void)
{
    hal_dma_irq_handler(HAL_SPI_CH4);
}
/*
 * DMA2_Stream0_IRQHandler
 * Hardware SPI DMA interrupt handler
 */
void DMA2_Stream0_IRQHandler(void)
{
    hal_dma_irq_handler(HAL_SPI_CH4);
}
/*
 * DMA2_Stream5_IRQHandler
 * Hardware SPI DMA interrupt handler
 */
void DMA2_Stream5_IRQHandler(void)
{
    hal_dma_irq_handler(HAL_SPI_CH5);
}
/*
 * DMA2_Stream6_IRQHandler
 * Hardware SPI DMA interrupt handler
 */
void DMA2_Stream6_IRQHandler(void)
{
    hal_dma_irq_handler(HAL_SPI_CH5);
}

/*
 * SPI1_IRQHandler
 * Hardware SPI interrupt handler
 */
void SPI1_IRQHandler(void)
{
    hal_spi_irq_handler(HAL_SPI_CH1);
}

/*
 * SPI2_IRQHandler
 * Hardware SPI interrupt handler
 */
void SPI2_IRQHandler(void)
{
    hal_spi_irq_handler(HAL_SPI_CH2);
}

/*
 * SPI3_IRQHandler
 * Hardware SPI interrupt handler
 */
void SPI3_IRQHandler(void)
{
    hal_spi_irq_handler(HAL_SPI_CH3);
}

/*
 * SPI4_IRQHandler
 * Hardware SPI interrupt handler
 */
void SPI4_IRQHandler(void)
{
    hal_spi_irq_handler(HAL_SPI_CH4);
}

/*
 * SPI5_IRQHandler
 * Hardware SPI interrupt handler
 */
void SPI5_IRQHandler(void)
{
    hal_spi_irq_handler(HAL_SPI_CH5);
}

/*
 * SPI6_IRQHandler
 * Hardware SPI interrupt handler
 */
void SPI6_IRQHandler(void)
{
    hal_spi_irq_handler(HAL_SPI_CH6);
}
