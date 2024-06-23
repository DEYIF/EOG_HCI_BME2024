#include "ADS1299.h"
#include "nrf_log.h"

u8 bug = 1;
void ADS1299_Init(void)   //ADS1299 初始化
{
	u8 i;  // 声明一个无符号8位整数型变量 i

	ADS_CLKSEL_SET;  // 设置 ADS 的时钟选择
	nrf_delay_ms(200);  // 延迟 200 毫秒
	
	ADS_CS_CLEAR;  // 清除 ADS 的芯片选择信号
	ADS_START_CLEAR;  // 清除 ADS 的启动信号

	ADS_RESET_SET;  // 设置 ADS 的复位信号
	nrf_delay_ms(200);  // 延迟 200 毫秒
	ADS_RESET_CLEAR;  // 清除 ADS 的复位信号
	nrf_delay_ms(200);  // 延迟 200 毫秒
	ADS_RESET_SET;  // 设置 ADS 的复位信号
	ADS_PWDN_SET;  // 设置 ADS 的电源控制信号
	nrf_delay_ms(3000);  // 延迟 3000 毫秒
	          
	SPI_Receive_Send(SDATAC);  // 向 SPI 发送停止连续发送数据命令

	for(i = 0; i < 50; i++);  // 等待一段时间
	
	bug = ADS1299_Read_REG(ADS1299_ID);  // 读取 ADS 寄存器中的 ID
	bug = ADS1299_Read_REG(ADS1299_CONFIG1);  // 读取 ADS 寄存器中的 CONFIG1
	bug = ADS1299_Read_REG(ADS1299_CONFIG2);  // 读取 ADS 寄存器中的 CONFIG2
	bug = ADS1299_Read_REG(ADS1299_CONFIG3);  // 读取 ADS 寄存器中的 CONFIG3
	bug = ADS1299_Read_REG(ADS1299_CONFIG4);  // 读取 ADS 寄存器中的 CONFIG4

	ADS1299_Write_REG(ADS1299_CONFIG1, 0xF6);  // 设置 ADS 寄存器中的 CONFIG1，采样频率500sps 内部时钟失能，0xF6 为 250SPS,1111 0110；1111 0100,F4,1k SPS；1111 0101
	ADS1299_Write_REG(ADS1299_CONFIG2, 0xD0);  // 设置 ADS 寄存器中的 CONFIG2，测试信号产生于内部或外部，0xD0,1101 0000
	//ADS1299_Write_REG(ADS1299_CONFIG3, 0xFE);  // 设置 ADS 寄存器中的 CONFIG3，参考和偏置，0xFE，1111 1110
	ADS1299_Write_REG(ADS1299_CONFIG3, 0xE6);  // 设置 ADS 寄存器中的 CONFIG3，参考和偏置，0xE6,1110 0110
	ADS1299_Write_REG(ADS1299_CONFIG4, 0x00);  // 设置 ADS 寄存器中的 CONFIG4，连续转换模式
	
	ADS1299_Write_REG(ADS1299_LOFF, 0x00);  // 设置 ADS 寄存器中的 LOFF
	ADS1299_Write_REG(ADS1299_LOFF_SENSP, 0x00);  // 设置 ADS 寄存器中的 LOFF_SENSP
	ADS1299_Write_REG(ADS1299_LOFF_SENSN, 0x00);  // 设置 ADS 寄存器中的 LOFF_SENSN
	ADS1299_Write_REG(ADS1299_LOFF_STATP, 0x00);  // 设置 ADS 寄存器中的 LOFF_STATP
	ADS1299_Write_REG(ADS1299_LOFF_STATN, 0x00);  // 设置 ADS 寄存器中的 LOFF_STATN
	
	ADS1299_Write_REG(ADS1299_BIAS_SENSP, 0xFF);  // 设置 ADS 寄存器中的 BIAS_SENSP    1111 0110
	ADS1299_Write_REG(ADS1299_BIAS_SENSN, 0xFF);  // 设置 ADS 寄存器中的 BIAS_SENSN    1111 0110
	
	ADS1299_Write_REG(ADS1299_MISC1, 0x20);  // 设置 ADS 寄存器中的 MISC1，0010 0000，打开
	//ADS1299_Write_REG(ADS1299_MISC1, 0x00);  // 设置 ADS 寄存器中的 MISC1，0000 0000，关闭
	ADS1299_Write_REG(ADS1299_MISC2, 0x00);  // 设置 ADS 寄存器中的 MISC2
	ADS1299_Write_REG(ADS1299_GPIO, 0x00);  // 设置 ADS 寄存器中的 GPIO
	
	
	ADS1299_Write_REG(ADS1299_CH1SET, 0x00); 	// 设置 ADS 寄存器中的 CH1SET，放大倍数24，0X68,0110 1000
	
  ADS1299_Write_REG(ADS1299_CH2SET, 0x80);  // 设置 ADS 寄存器中的 CH2SET，Input Short,1000 0001
	ADS1299_Write_REG(ADS1299_CH3SET, 0x80);  // 设置 ADS 寄存器中的 CH3SET，Input Short 0X09
	
	ADS1299_Write_REG(ADS1299_CH4SET, 0x00);  // 设置 ADS 寄存器中的 CH4SET，Input Short
	ADS1299_Write_REG(ADS1299_CH5SET, 0x00); 	// 设置 ADS 寄存器中的 CH5SET,
	
	ADS1299_Write_REG(ADS1299_CH6SET, 0x80);  // 设置 ADS 寄存器中的 CH6SET, 0000 0101
	ADS1299_Write_REG(ADS1299_CH7SET, 0x80);  // 设置 ADS 寄存器中的 CH7SET，Input Short 0X09
	
	ADS1299_Write_REG(ADS1299_CH8SET, 0x00);  // 设置 ADS 寄存器中的 CH8SET，Input Short，增益24：0110 0000
	
	bug = ADS1299_Read_REG(ADS1299_CONFIG1);  // 读取 ADS 寄存器中的 CONFIG1
	bug = ADS1299_Read_REG(ADS1299_CONFIG2);  // 读取 ADS 寄存器中的 CONFIG2
	bug = ADS1299_Read_REG(ADS1299_CONFIG3);  // 读取 ADS 寄存器中的 CONFIG3
	bug = ADS1299_Read_REG(ADS1299_CONFIG4);  // 读取 ADS 寄存器中的 CONFIG4

	ADS_START_SET;  // 设置 ADS 的启动信号
	SPI_Receive_Send(RDATAC);  // 发送连续读取数据命令
}



/********* ADS1292R读寄存器 *********/
u8 ADS1299_Read_REG(u8 Address)
{
  char temp=0;                  // 初始化变量'temp'，用于存储接收到的数据
  unsigned char i;              // 声明一个无符号字符变量'i'，用于循环迭代
  //delay_us(10);            
  for(i=0;i<10;i++);             // 空的for循环，迭代10次，可能用于延迟（但由于分号，此循环无效）
  SPI_Receive_Send(Address|0x20); // 发送指定地址与0x20进行逻辑或操作的结果，表示一个读操作  
  for(i=0;i<10;i++);             // 又一个空的for循环，可能用于延迟
  SPI_Receive_Send(0x00);        // 发送一个哑元字节以从指定地址接收数据
  for(i=0;i<10;i++);             // 再次，一个空的for循环，可能用于延迟
  temp = SPI_Receive_Send(0x00); // 通过SPI通信接收实际数据并存储在'temp'中  
  return temp;                   // 返回接收到的数据
}

/********* ADS1292R写寄存器 *********/
void ADS1299_Write_REG(u8 Address,u8 data)
{
  unsigned char i;	
	u8 cmd[3]={0x00,0x00,0x00,};
	cmd[0]=0x40|Address;
	cmd[1]=0x00;
	cmd[2]=data;
  for(i=0;i<10;i++);
	SPI_Receive_Send(cmd[0]);
  for(i=0;i<10;i++);
  SPI_Receive_Send(cmd[1]);
  for(i=0;i<10;i++);
  SPI_Receive_Send(cmd[2]);
}


u32 EMG_data[8];
//u32 EMG_data[4];
extern u8 EMG_Flag ;
int32_t signByte;
u32 ADC_data[12];

void ADS1299_ReadDates(void)
{
	static u8 buf[27]={0};
	u8 i;
	
	if(ADS_DRDY_READ == 0)
	{
		for(i=0;i<27;i++)
		{
			buf[i] = SPI_Receive_Send(0x00);
		}			
		
		ADC_data[0]=buf[3];//EMG1
		ADC_data[1]=buf[4];
		ADC_data[2]=buf[5];
		
		ADC_data[3]=buf[12];//EMG4
		ADC_data[4]=buf[13];
		ADC_data[5]=buf[14];
		
		ADC_data[6]=buf[15];//EMG5
		ADC_data[7]=buf[16];
		ADC_data[8]=buf[17];
		
		ADC_data[9]=buf[24];//EMG8
		ADC_data[10]=buf[25];
		ADC_data[11]=buf[26];
		
		for(i=0;i<4;i++){
			if(ADC_data[i*3] & 0x80u)
			{
				signByte = 0xFF000000;
			}
			else
			{
				signByte = 0x00000000;
			}
			int32_t upperByte	= ((int32_t) ADC_data[i*3] & 0xFF) << 16;
			int32_t middleByte	= ((int32_t)ADC_data[i*3+1] & 0xFF) << 8;
			int32_t lowerByte	= ((int32_t) ADC_data[i*3+2] & 0xFF) << 0;
			EMG_data[i]=signByte | upperByte | middleByte | lowerByte;
		}
	

		EMG_Flag = 1;		
	}
}
