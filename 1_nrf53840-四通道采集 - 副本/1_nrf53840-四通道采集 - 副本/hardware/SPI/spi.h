#ifndef __SPI_H
#define __SPI_H

#include "nrf_drv_spi.h"
#include "ADS1299.h"

void SPI_Init(void);
u8 SPI_Receive_Send(u8 CommData);
		 
#endif

