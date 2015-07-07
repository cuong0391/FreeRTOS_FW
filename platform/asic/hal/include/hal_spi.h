/**
 ******************************************************************************
 * @file    hal_spi.h
 * @author  Embedded Team CopyRight@NTC
 * @version V1.0.0
 * @date    June-04-2015
 * @brief   This file expandes for #SPI driver
 ******************************************************************************
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _HAL_SPI_H_
#define _HAL_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported Define------------------------------------------------------------*/

#ifndef STD_ON
#define STD_ON  1
#endif

#ifndef STD_OFF
#define STD_OFF 0
#endif

/** Configure SPI max buffer size for block data transfer */
#define HAL_SPI_MAX_BUFFER_SIZE          1024
/** For development debug code */
#define HAL_SPI_DEBUG                    STD_OFF
/** Timeout make surce program is not stuck */
#define HAL_SPI_TIMEOUT                  1000000
/* Exported typedef ----------------------------------------------------------*/

/**
 * @brief hal_spi_return_t
 * 
 * #SPI return type
 */
typedef enum _hal_spi_return {
    SPI_OK,
    SPI_NULL_PTR,
    SPI_INVALID_CHANNEL,
    SPI_CHANNEL_BUSY,
    SPI_INVALID_LENGTH,
    SPI_UNKNOWN_ERROR,
    SPI_INVALID_PRIORITY
}hal_spi_return_t;

/**
 * @brief hal_spi_status_t
 * 
 * #SPI return type
 */
typedef enum _hal_spi_status {
    SPI_READY,
    SPI_TRANSFERING,
    SPI_TX_BLOCK_COMPLETED,
    SPI_RX_BLOCK_COMPLETED
}hal_spi_status_t;

/**
 * @brief hal_spi_clock_speed_t
 * 
 * #SPI clock speed
 */
typedef enum _hal_spi_clock_speed {
    SPI_CLK_21MHz  = 0x0000,
    SPI_CLK_10MHz  = 0x0008,
    SPI_CLK_5MHz   = 0x0010,
    SPI_CLK_2MHz   = 0x0018,
    SPI_CLK_1MHz   = 0x0020,
    SPI_CLK_600KHz = 0x0028,
    SPI_CLK_300KHz = 0x0030,
    SPI_CLK_150KHz = 0x0038
}hal_spi_clock_speed_t;
/**
 * @brief _hal_spi_mode
 *
 * #SPI data mode
 */
typedef enum _hal_spi_mode{
  SPI_MODE_0_MSB = 0,  /* CPOL=0, CPHA=0 (MSB-First)*/
  SPI_MODE_1_MSB,      /* CPOL=0, CPHA=1 (MSB-First)*/
  SPI_MODE_2_MSB,      /* CPOL=1, CPHA=0 (MSB-First)*/
  SPI_MODE_3_MSB,      /* CPOL=1, CPHA=1 (MSB-First)*/
  SPI_MODE_0_LSB,      /* CPOL=0, CPHA=0 (LSB-First)*/
  SPI_MODE_1_LSB,      /* CPOL=0, CPHA=1 (LSB-First)*/
  SPI_MODE_2_LSB,      /* CPOL=1, CPHA=0 (LSB-First)*/
  SPI_MODE_3_LSB       /* CPOL=1, CPHA=1 (LSB-First)*/
}hal_spi_mode_t;

/**
 * @brief hal_spi_operation_mode_t
 *
 * SPI operation modes selection
 */
typedef enum _hal_spi_operation_mode {
    SPI_SLAVE,
    SPI_MASTER
}hal_spi_operation_mode_t;

/**
* @brief  hal_spi_channel_t
* #SPI channel
*/
typedef enum _hal_spi_channel {
    HAL_SPI_CH1,
    HAL_SPI_CH2,
    HAL_SPI_CH3,
    HAL_SPI_CH4,
    HAL_SPI_CH5,
    HAL_SPI_CH6,    
    HAL_SPI_MAX_IDX
}hal_spi_channel_t;
/**
 * @brief hal_spi_irq_priority_t
 *
 * #spi irq priority define
 */
typedef enum _hal_spi_irq_prio {
    HAL_SPI_P0 = 0,
    HAL_SPI_P1,
    HAL_SPI_P2,
    HAL_SPI_P3,
    HAL_SPI_P4,
    HAL_SPI_P_MAX
}hal_spi_irq_priority_t;
/**
 * @brief hal_spi_buffer_t
 *
 * #spi buffer define
 */
typedef struct _hal_spi_buffer {
    uint16_t     len;         /** tx Data Length */
    uint8_t*     buf;         /** tx Data buffer */
}hal_spi_buffer_t;

/**
 * @brief hal_spi_buffer_t
 *
 * #spi buffer define
 */
typedef struct _hal_spi_master_buffer {
    hal_spi_buffer_t   rxbuf;                       /** rx buffer */
    hal_spi_buffer_t   txbuf;                       /** tx buffer */
}hal_spi_master_buffer_t;
/**
 * @brief  hal_spi_t
 * #SPI struct configure
 */
typedef enum spi_dma_t
{
    ENABLE_DMA,
    DISABLE_DMA
}spi_dma_t;
/**
 * @brief  spi_data_size
 * #SPI struct configure
 */
typedef enum spi_data_size
{
    DATASIZE_16b,
    DATASIZE_8b
}spi_data_size_t;
/**
 * @brief  hal_spi_t
 * #SPI struct configure
 */
typedef struct _hal_spi {
    hal_spi_channel_t         chid;
    hal_spi_clock_speed_t     clock_speed;          /** #SPI clock speed */
    hal_spi_operation_mode_t  omode;                /** #SPI operation mode */
    hal_spi_irq_priority_t    priority;             /** hal i2c irq priority */
    hal_spi_mode_t            mode;                 /** mode to spi transfer data*/
    spi_data_size_t           datasize;             /** data size to transfer/receive data*/
    spi_dma_t                 dma_status;           /** enable or disable dma */
    void               (*func_callback)(hal_spi_status_t status);    /** call back function to upper layer */
}hal_spi_t;

/* Exported functions ----------------------------------------------------------*/

/**
 * @brief  Initializes SPIx peripheral
 * @param  *spi: Pointer to SPI peripheral you will use 
 * @retval reference to #hal_spi_return_t
 */
int hal_spi_init(hal_spi_t *spi);

/**
 * @brief  hal_spi_deinit
 * The function does free spi resource
 * @param  *spi: Pointer to SPI peripheral you will use
 * @retval reference to hal_spi_return_t
 */
int hal_spi_deinit(hal_spi_t *spi);

/** 
 * @brief  hal_spi_get_status
 * The function gets status on SPI bus
 * @param  chid - #SPI channel ID
 * @retval reference to #hal_spi_status_t
 */
int hal_spi_get_status( hal_spi_channel_t chid );
/**
 * @brief  hal_spi_xfer
 * The function sends block data from slave device, make surce that #CS pin of slave
 * device is presented before call this function
 * @param  *spi: Pointer to SPI peripheral you will use
 * @param  buf - point to hal_spi_master_buffer_t structure
 * @retval reference to #hal_spi_return_t
 */
int hal_spi_xfer(hal_spi_t *spi, hal_spi_master_buffer_t * buff);
/**
 * @brief  hal_spi_xfer
 * The function reads block data from slave device, make surce that #CS pin of slave
 * device is presented before call this function
 * @param  *spi: Pointer to SPI peripheral you will use
 * @param  buf - point to hal_spi_master_buffer_t structure
 * @retval reference to #hal_spi_return_t
 */
int hal_spi_xreceive(hal_spi_t *spi,hal_spi_master_buffer_t * buff);
#endif /* _HAL_SPI_H_ */
