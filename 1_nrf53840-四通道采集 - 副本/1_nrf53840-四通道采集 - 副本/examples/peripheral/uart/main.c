#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "app_uart.h"
#include "app_error.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "bsp.h"
#include "nrf_gpio.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "SEGGER_RTT.h"

#include "spi.h"
#include "ADS1299.h"
#include "wavelet.h"

#if defined (UART_PRESENT)
#include "nrf_uart.h"
#endif
#if defined (UARTE_PRESENT)
#include "nrf_uarte.h"
#endif


//#define ENABLE_LOOPBACK_TEST  /**< if defined, then this example will be a loopback test, which means that TX should be connected to RX to get data loopback. */

#define MAX_TEST_DATA_BYTES     (15U)                /**< max number of test bytes to be used for tx and rx. */
#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 256                         /**< UART RX buffer size. */


/********¼¡µç²É¼¯*********/
int i=0;
u8 EMG_Flag = 0;
extern u32 EMG_data[8];
int data_wait=0;

/**
 * @brief Function for main application entry.
 */
int main(void)
{
		SPI_Init();
	  ADS1299_Init();	
//	  UART_Init();

    while (1)
    {
			ADS1299_ReadDates();			
			if(EMG_Flag == 1)
			{						
				if (data_wait>1000)
				{
					for(i=0;i<2;i++)
					{
						SEGGER_RTT_SetTerminal(i);
						SEGGER_RTT_printf(0,"%d\r\n",EMG_data[i]);			
					}	
				}
				else
				{
					data_wait++;
				}

			}
			EMG_Flag = 0;				
    }			
}
