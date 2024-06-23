#include "key.h"


void KEY_Init(void)
{
  nrf_gpio_cfg_input(11,NRF_GPIO_PIN_PULLUP);
  nrf_gpio_cfg_input(12,NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(24,NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(25,NRF_GPIO_PIN_PULLUP);
	nrf_gpio_cfg_input(26,NRF_GPIO_PIN_PULLUP);
	}
	
 void Delay(uint32_t temp)
{
  for(; temp!= 0; temp--);
} 


uint8_t KEY1_Down(void)
{			
		  /*检测是否有按键按下 */
   	if( nrf_gpio_pin_read(KEY_1)== 0 ) 
	  {	   
	 	   /*延时消抖*/
	  	 Delay(10000);		
	   		if(nrf_gpio_pin_read(KEY_1)== 0 )  
					{	 
						/*等待按键释放 */
						while(nrf_gpio_pin_read(KEY_1)== 0 );   
			 			return 	0	; 
					}
			else
					return 1;
		}
	else
		return 1;
}


uint8_t KEY2_Down(void)
{			
		  /*检测是否有按键按下 */
   	if( nrf_gpio_pin_read(KEY_2)== 0 ) 
	  {	   
	 	   /*延时消抖*/
	  	 Delay(10000);		
	   		if(nrf_gpio_pin_read(KEY_2)== 0 )  
					{	 
						/*等待按键释放 */
						while(nrf_gpio_pin_read(KEY_2)== 0 );   
			 			return 	0	; 
					}
			else
					return 1;
		}
	else
		return 1;
}


uint8_t KEY3_Down(void)
{			
		  /*检测是否有按键按下 */
   	if( nrf_gpio_pin_read(KEY_3)== 0 ) 
	  {	   
	 	   /*延时消抖*/
	  	 Delay(10000);		
	   		if(nrf_gpio_pin_read(KEY_3)== 0 )  
					{	 
						/*等待按键释放 */
						while(nrf_gpio_pin_read(KEY_3)== 0 );   
			 			return 	0	; 
					}
			else
					return 1;
		}
	else
		return 1;
}



uint8_t KEY4_Down(void)
{			
		  /*检测是否有按键按下 */
   	if( nrf_gpio_pin_read(KEY_4)== 0 ) 
	  {	   
	 	   /*延时消抖*/
	  	 Delay(10000);		
	   		if(nrf_gpio_pin_read(KEY_4)== 0 )  
					{	 
						/*等待按键释放 */
						while(nrf_gpio_pin_read(KEY_4)== 0 );   
			 			return 	0	; 
					}
			else
					return 1;
		}
	else
		return 1;
}

//uint8_t TKY_Down(void)
//{			
//		  /*检测是否有按键按下 */
//   	if( nrf_gpio_pin_read(TKY)== 0 ) 
//	  {	   
//	 	   /*延时消抖*/
//	  	 Delay(10000);		
//	   		if(nrf_gpio_pin_read(TKY)== 0 )  
//					{	 
//						/*等待按键释放 */
//						while(nrf_gpio_pin_read(TKY)== 0 );   
//			 			return 	0	; 
//					}
//			else
//					return 1;
//		}
//	else
//		return 1;
//}


