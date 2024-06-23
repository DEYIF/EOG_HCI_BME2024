#ifndef __ADS1299_H
#define __ADS1299_H

#include "Datatype.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "spi.h"
//#include "filter.h"
#include <nrf_gpiote.h>

/********* 引脚定义 *********/
#define  ADS_MISO        2
#define  ADS_MOSI        4
#define  ADS_PWDN        6
#define  ADS_RESET       8
#define  ADS_SCLK        29
#define  ADS_CS          31
#define  ADS_START       41
#define  ADS_CLKSEL      42
#define  ADS_DRDY        47
//#define  ADS_START       NRF_GPIO_PIN_MAP(1,9)
//#define  ADS_CLKSEL      NRF_GPIO_PIN_MAP(1,10)
//#define  ADS_DRDY        NRF_GPIO_PIN_MAP(1,15)


/********* 引脚电平定义 *********/
#define  ADS_PWDN_SET            nrf_gpio_pin_set(ADS_PWDN)
#define  ADS_PWDN_CLEAR          nrf_gpio_pin_clear(ADS_PWDN)

#define  ADS_RESET_SET            nrf_gpio_pin_set(ADS_RESET)
#define  ADS_RESET_CLEAR          nrf_gpio_pin_clear(ADS_RESET)

#define  ADS_CS_SET              nrf_gpio_pin_set(ADS_CS)
#define  ADS_CS_CLEAR            nrf_gpio_pin_clear(ADS_CS)

#define  ADS_START_SET           nrf_gpio_pin_set(ADS_START)
#define  ADS_START_CLEAR         nrf_gpio_pin_clear(ADS_START)

#define  ADS_CLKSEL_SET          nrf_gpio_pin_set(ADS_CLKSEL)
#define  ADS_CLKSEL_CLEAR        nrf_gpio_pin_clear(ADS_CLKSEL)

#define  ADS_DRDY_READ           nrf_gpio_pin_read(ADS_DRDY)

///****************************************************************/
//ADS1299命令码表
/****************************************************************/
// 系统命令
#define WAKEUP	0x02		//退出待机模式
#define STANDBY	0x04		//进入待机模式
#define RESET		0x06		//芯片复位
#define START		0x08		//启动/重新启动同步转换
#define STOP		0x0A		//停止转换
 
// 读取数据命令
#define RDATAC	0x10		//使能连续读取数据模式
 								//这个模式是上电默认模式
#define SDATAC	0x11		//停止连续读取数据模式
#define RDATA		0x12		//按命令读取数据; supports multiple read back.
 
// 寄存器读取命令
#define RREG		0x20		//Read n nnnn registers starting at address r rrrr
 								//first byte 001r rrrr (2xh)(2) - second byte 000n nnnn(2)
#define WREG		0x40		//Write n nnnn registers starting at address r rrrr
								//first byte 010r rrrr (2xh)(2) - second byte 000n nnnn(2)
 
 
/****************************************************************/
// ADS1299寄存器列表
/****************************************************************/
//都是该寄存器的地址  往寄存器写值时 先写地址在写值
#define ADS1299_ID           0x00   //芯片ID
#define ADS1299_CONFIG1      0x01   //菊花链、时钟、数据速率
#define ADS1299_CONFIG2      0x02   //配置测试信号
#define ADS1299_CONFIG3      0x03   //偏置BIAS
#define ADS1299_LOFF         0x04   //导联脱落检测寄存器
#define ADS1299_CH1SET       0x05   //配置输入通道
#define ADS1299_CH2SET       0x06   //配置输入通道
#define ADS1299_CH3SET       0x07   //配置输入通道
#define ADS1299_CH4SET       0x08   //配置输入通道
#define ADS1299_CH5SET       0x09   //配置输入通道
#define ADS1299_CH6SET       0x0A   //配置输入通道
#define ADS1299_CH7SET       0x0B   //配置输入通道
#define ADS1299_CH8SET       0x0C   //配置输入通道
#define ADS1299_BIAS_SENSP   0x0D   //BIAS正信号导出寄存器
#define ADS1299_BIAS_SENSN   0x0E   //BIAS负信号导出寄存器
#define ADS1299_LOFF_SENSP   0x0F   //正信号导联脱落检测寄存器
#define ADS1299_LOFF_SENSN   0x10   //负信号导联脱落检测寄存器
#define ADS1299_LOFF_FLIP    0x11   //导联脱落电流的流向
#define ADS1299_LOFF_STATP   0x12   //存储每个正电极导联状态
#define ADS1299_LOFF_STATN   0x13   //存储每个负电级导联状态
#define ADS1299_GPIO         0x14
#define ADS1299_MISC1        0x15
#define ADS1299_MISC2        0x16
#define ADS1299_CONFIG4      0x17   //配置转换模式和使能导联脱落比较器

void ADS1299_Init(void);
//u32 Data_validation(u32 data);
u8 ADS1299_Read_REG(u8 Address);
//void ADS1299_ReadDates(void);
void ADS1299_Write_REG(u8 Address,u8 data);
void ADS1299_ReadDates(void);
//int32_t ReadData(void);
#endif
