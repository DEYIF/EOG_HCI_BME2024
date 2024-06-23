#ifndef _VARIABLE_H
#define _VARIABLE_H
#include "DataType.h"
#include "WitleafProtocol.h"
//#include "keywords.h"
extern UINT32 TimingDelay;
extern UINT8 count;

//for usart_send_packet.c
extern WITLEAF_PACKET gtWitleafPacket;
extern UINT8 gu8ConnectionFlag;
extern UINT16 gpu8ConnectionOf1sTimer;
extern UINT8 gpu8WitleafCommOutBuff[20];
extern UINT32 gu32WitleafDataNo;
extern UINT8 gu8CommInHead;
extern UINT8 gu8CommInTail;
// 串口0的接收缓冲区和头指针、尾指针
extern UINT8 gpu8CommInBuff[COMM_IN_BUFF_LEN];
// 保存要发送数据的缓冲区
extern UINT8 gpu8CommStoreBuff[COMM_OUT_BUFF_LEN];
// 保存要发送数据的缓冲区的头指针、尾指针
// 接收到的一个命令包的内容
extern UINT16 gu16CommStoreHead;
extern UINT16 gu16CommStoreTail;

extern UINT8  get8data[27];
 
#endif

