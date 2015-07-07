#include "hal_spi.h"
#include "log.h"


uint8_t txbuffer[32];
uint8_t rxbuffer[32];
uint8_t _status=0;
static void func (hal_spi_status_t status);
hal_spi_t SPI = {
		.chid = HAL_SPI_CH1,
		.clock_speed = SPI_CLK_2Mhz,
		.omode = SPI_SLAVE,
		.priority = HAL_SPI_P0,
		.func_callback = func
};
static void spi_xfer(void);

int main (void)
{

	log_init();
	if(hal_spi_init(&SPI)==SPI_OK)
		log_msg("initialized\r\n");
	if(hal_spi_irq_enable(&SPI)==SPI_OK)
		log_msg("enable irq success\r\n");

	log_msg("%s: Start to run spi test\n\r",__FUNCTION__);
	spi_xfer();
	while(1){}
	return 0;
}

static void func (hal_spi_status_t status)
{

	if(SPI_TX_BLOCK_COMPLETED == status || SPI_RX_BLOCK_COMPLETED == status )
		_status =1;
	else
		_status =0;

	log_msg("%s \r\n", status==SPI_TX_BLOCK_COMPLETED ? "Transfered complete !" : status==SPI_RX_BLOCK_COMPLETED ? "Received complete":"Waiting...");
	log_msg("status= %d\r\n",_status);
}
static void spi_xfer(void)
{
	hal_spi_master_buffer_t xferbuf ;
	xferbuf.rxbuf.buf = rxbuffer;
	xferbuf.rxbuf.len =4;

	while(hal_spi_get_status(HAL_SPI_CH1)!= SPI_READY){}

	hal_spi_read_block(HAL_SPI_CH1,&xferbuf);
	while(!_status){}
	log_msg("Data:");
	for(int i=0;i<xferbuf.rxbuf.len;i++)
	{
		log_msg("%d",rxbuffer[i]);
	}
	log_msg("\n\r");
	log_msg("==============================\n\r");
}
