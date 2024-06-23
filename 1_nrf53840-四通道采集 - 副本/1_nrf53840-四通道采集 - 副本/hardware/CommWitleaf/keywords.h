#ifndef _KEYWORDS_H
#define _KEYWORDS_H
#include "DataType.h"
#include "WitleafProtocol.h"
//volatile UINT32 TimingDelay;
UINT8 count;

//for usart_send_packet.c
WITLEAF_PACKET gtWitleafPacket;

UINT8 gu8ConnectionFlag;


UINT16 gpu8ConnectionOf1sTimer;
UINT8 gpu8WitleafCommOutBuff[20];
UINT32 gu32WitleafDataNo;
UINT8 gu8CommInHead;
UINT8 gu8CommInTail;
// 串口0的接收缓冲区和头指针、尾指针
UINT8 gpu8CommInBuff[COMM_IN_BUFF_LEN];
// 保存要发送数据的缓冲区
UINT8 gpu8CommStoreBuff[COMM_OUT_BUFF_LEN];
// 保存要发送数据的缓冲区的头指针、尾指针
// 接收到的一个命令包的内容
UINT16 gu16CommStoreHead;
UINT16 gu16CommStoreTail;

UINT8  get8data[27];

#endif

