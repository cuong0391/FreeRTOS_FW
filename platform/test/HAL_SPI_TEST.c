#include "hal_spi.h"
#include "log.h"
#include "hal_dio.h"
uint8_t txbuffer[32];
uint8_t rxbuffer[32];
uint8_t _status=0;

static void func (hal_spi_status_t status);
hal_spi_t SPI = {
		.chid = HAL_SPI_CH1,
		.clock_speed = SPI_CLK_21MHz,
		.omode = SPI_MASTER,
		.priority = HAL_SPI_P0,
		.func_callback = func
};
static hal_dio_t  gpio ={
        .chid  = HAL_DIO_CH2,
        .dir   = HAL_DIO_OUT,
        .state = HAL_DIO_LOW
};
static void spi_xfer(void);

int main (void)
{

	log_init();
    /* init gpio */
    hal_dio_init(&gpio);
	if(hal_spi_init(&SPI)==SPI_OK)
		log_msg("initialized\r\n");
	hal_spi_dma_init(HAL_SPI_CH1);
	hal_spi_dma_tx_irq_en(&SPI);

//	if(hal_spi_irq_enable(&SPI)==SPI_OK)
//		log_msg("enable irq success\r\n");

	log_msg("%s: Start to run spi test\n\r",__FUNCTION__);

	hal_dio_set_low(&gpio);
	spi_xfer();
	hal_dio_set_high(&gpio);

	while(1){}
	return 0;
}

static void func (hal_spi_status_t status)
{

	if(SPI_TX_BLOCK_COMPLETED == status || SPI_RX_BLOCK_COMPLETED == status )
		_status =1;
	else
		_status =0;
	log_msg("%s \r\n", status==SPI_TX_BLOCK_COMPLETED ? "Transfered complete !" : status==SPI_RX_BLOCK_COMPLETED ? "Received complete!":"Waiting...");
//	log_msg("status= %d\r\n",_status);
}
static void spi_xfer(void)
{
	hal_spi_master_buffer_t xferbuf ;
	xferbuf.txbuf.buf = txbuffer;
	xferbuf.txbuf.len =4;
	xferbuf.txbuf.buf[0] =0x01;
	xferbuf.txbuf.buf[1] =0x02;
	xferbuf.txbuf.buf[2] =0x03;
	xferbuf.txbuf.buf[3] =0x04;
	while(hal_spi_get_status(HAL_SPI_CH1)!= SPI_READY){}
//	hal_spi_send_block(HAL_SPI_CH1,&xferbuf);
	spi_dma_tx(HAL_SPI_CH1,&xferbuf);

	while(!_status){}

	log_msg("==============================\n\r");
}
