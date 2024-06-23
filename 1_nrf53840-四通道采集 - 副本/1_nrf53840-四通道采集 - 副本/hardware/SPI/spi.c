#include "spi.h"

#define SPI_INSTANCE0  0 /**< SPI instance index. */

static volatile bool spi_xfer_done0;  //SPI数据传输完成标志
static const nrf_drv_spi_t spi_0 = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE0);  /**< SPI instance. */
//SPI_InitTypeDef  SPI_InitStructure;
/**
 * @brief SPI user event handler.
 * @param event
 */
void spi_event_handler0(nrf_drv_spi_evt_t const * p_event)
{
  spi_xfer_done0 = true;
}

void SPI_Init(void)
{
	nrf_gpio_cfg_output(ADS_PWDN);
	nrf_gpio_cfg_output(ADS_RESET);
	nrf_gpio_cfg_output(ADS_CS);
	nrf_gpio_cfg_output(ADS_START);
	nrf_gpio_cfg_output(ADS_CLKSEL);
	nrf_gpio_cfg_input(ADS_DRDY,NRF_GPIO_PIN_PULLUP);
	
	nrf_gpio_pin_set(ADS_PWDN);
	nrf_gpio_pin_set(ADS_RESET);
	nrf_gpio_pin_set(ADS_CS);
	nrf_gpio_pin_set(ADS_START); 
	nrf_gpio_pin_set(ADS_CLKSEL);

	nrf_drv_spi_config_t spi0_config;
	
	spi0_config.mode = NRF_DRV_SPI_MODE_1;       //SPI模式选择 相位、极性选择
	spi0_config.bit_order = NRF_DRV_SPI_BIT_ORDER_MSB_FIRST;    //高位先行
	spi0_config.frequency = NRF_DRV_SPI_FREQ_8M;     //SPI通信频率
	spi0_config.irq_priority = 0;         //设置中断优先级
	spi0_config.orc = 0xFF;
		
  spi0_config.ss_pin = NRF_DRV_SPI_PIN_NOT_USED;    //使用软件片选功能
	spi0_config.miso_pin = ADS_MISO;
	spi0_config.mosi_pin = ADS_MOSI;
	spi0_config.sck_pin = ADS_SCLK;
	
  nrf_drv_spi_init(&spi_0, &spi0_config, NULL, NULL);
}

/********* SPI读写 *********/
u8 SPI_Receive_Send(u8 CommData)
{
	uint8_t ReceData;
    
  nrf_drv_spi_transfer(&spi_0,&CommData,1,&ReceData,1);
	
  return ReceData;
}


