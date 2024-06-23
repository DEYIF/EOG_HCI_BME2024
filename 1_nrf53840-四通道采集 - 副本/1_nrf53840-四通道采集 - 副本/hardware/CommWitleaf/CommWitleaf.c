#include "CommWitleaf.h"

UINT16 WitleafGetUartRxBuffLen()  //获取数据块的长度
{
	UINT8 u8Len;

	if ( gu8CommInHead >= gu8CommInTail ) 	u8Len = gu8CommInHead - gu8CommInTail;
	
	else 	u8Len = COMM_IN_BUFF_LEN - gu8CommInTail + gu8CommInHead;
	
	return u8Len;
}

UINT8 WitleafGetUartRxStr(UINT8 * pu8DestBuff, UINT8 u8Len)
{
	UINT8 u8RealLen;
	UINT8 u8Index;

	u8RealLen = 0;

	for ( u8Index = 0; u8Index < u8Len; u8Index ++ )
	{
		// 如果数据读取超界，则返回实际读取数据长度
		if ( gu8CommInTail == gu8CommInHead ) 
		{
			return u8RealLen;
		}
		pu8DestBuff[u8Index] = gpu8CommInBuff[gu8CommInTail];

		gu8CommInTail ++;
		u8RealLen ++;

		if ( COMM_IN_BUFF_LEN <= gu8CommInTail ) 
		{
			gu8CommInTail = 0;
		}
	}

	return u8Len;
}		

UINT8 WitleafUnpackUartData(UINT8 * pu8Buff, UINT8 u8BuffLen, WITLEAF_PACKET * ptPacket, UINT8 * pu8RemainLen)
{
	WITLEAF_SEARCH_PACKET_STATUS_LIST tSearchPacketStatus;
	
	UINT8 u8CurrentByte;
	UINT8 pu8No[4];
	UINT8 u8Checksum = 0;
	
	UINT8 u8Len;
	UINT8 u8HeadIndex = 0;
	UINT8 u8Index;
	
	INT8 i = 0;

	tSearchPacketStatus = WITLEAF_SEARCH_PACKET_STATUS_HEAD;

	u8Len = u8BuffLen;
	u8Index = 0;

	while ( u8Len > 0) {        
		u8CurrentByte = pu8Buff[u8Index ++];								// 读取缓冲区数据
		u8Len --;															// 缓冲区数据更新

		switch ( tSearchPacketStatus ) {
			case WITLEAF_SEARCH_PACKET_STATUS_HEAD:									// 搜索数据包头
				if ( WITLEAF_PACKET_HEAD == u8CurrentByte ) {
					tSearchPacketStatus = WITLEAF_SEARCH_PACKET_STATUS_LEN;
					u8HeadIndex = (u8Index - 1);							// 记录当前包头的位置
				}
				else if ( u8Len < WITLEAF_MIN_FRAME_LEN ) {					// 数据长度不足返回
					(* pu8RemainLen) = u8Len + 1;							// 返回缓冲区内未参与解包的数据长度
					return 0;
				}
				break;
				
			case WITLEAF_SEARCH_PACKET_STATUS_LEN:									// 搜索数据长度
				ptPacket->u8Len = u8CurrentByte;
				// 数据包过长，非法数据包，丢弃包头重新搜索
				if ( WITLEAF_MAX_FRAME_DATA_LEN < ptPacket->u8Len ) {
					u8Index = u8HeadIndex + 1;							// 丢弃第一个字节，重新解包
					u8Len = u8BuffLen - u8Index;							// 计算缓冲区剩余字节长度
					tSearchPacketStatus = WITLEAF_SEARCH_PACKET_STATUS_HEAD;                         
				}
				else if ( (ptPacket->u8Len - 2) <= u8Len ) {				// 数据长度满足当前数据包要求
					tSearchPacketStatus = WITLEAF_SEARCH_PACKET_STATUS_PARA_TYPE;
				}
				else {														// 数据长度不足
					(* pu8RemainLen) = u8Len + 2;							// 返回缓冲区内未参与解包的数据长度
					return 0;
				}                                
				break;

			case WITLEAF_SEARCH_PACKET_STATUS_PARA_TYPE:
				ptPacket->u8ParaType = u8CurrentByte;
				tSearchPacketStatus = WITLEAF_SEARCH_PACKET_STATUS_TYPE;
				break;

			case WITLEAF_SEARCH_PACKET_STATUS_TYPE:
				ptPacket->u8Type = u8CurrentByte;
				tSearchPacketStatus = WITLEAF_SEARCH_PACKET_STATUS_ID;
				break;

			case WITLEAF_SEARCH_PACKET_STATUS_ID:
				ptPacket->u8Id = u8CurrentByte;
				tSearchPacketStatus = WITLEAF_SEARCH_PACKET_STATUS_NO_0;
				break;

			case WITLEAF_SEARCH_PACKET_STATUS_NO_0:
				pu8No[0] = u8CurrentByte;
				ptPacket->u32No = u8CurrentByte;
				tSearchPacketStatus = WITLEAF_SEARCH_PACKET_STATUS_NO_1;
				break;

			case WITLEAF_SEARCH_PACKET_STATUS_NO_1:
				pu8No[1] = u8CurrentByte;
				ptPacket->u32No += u8CurrentByte << 8;
				tSearchPacketStatus = WITLEAF_SEARCH_PACKET_STATUS_NO_2;
				break;

			case WITLEAF_SEARCH_PACKET_STATUS_NO_2:
				pu8No[2] = u8CurrentByte;
				ptPacket->u32No += u8CurrentByte << 16;
				tSearchPacketStatus = WITLEAF_SEARCH_PACKET_STATUS_NO_3;
				break;

			case WITLEAF_SEARCH_PACKET_STATUS_NO_3:
				pu8No[3] = u8CurrentByte;
				ptPacket->u32No += u8CurrentByte << 24;
				tSearchPacketStatus = WITLEAF_SEARCH_PACKET_STATUS_DATA;
																			// 直接读取数据包的数据内容，因此此处省略了break;语句。
			case WITLEAF_SEARCH_PACKET_STATUS_DATA:									// 读取数据包数据 
				u8Checksum = ptPacket->u8Len;
				u8Checksum += ptPacket->u8ParaType;
				u8Checksum += ptPacket->u8Type;
				u8Checksum += ptPacket->u8Id;
				u8Checksum += pu8No[0];
				u8Checksum += pu8No[1];
				u8Checksum += pu8No[2];
				u8Checksum += pu8No[3];
				for ( i = 0; i < (ptPacket->u8Len - 10); i ++ ) {
					ptPacket->pu8Data[i] = pu8Buff[u8Index ++];
					u8Checksum += ptPacket->pu8Data[i];
					u8Len --;												// 更新数据缓冲区数据长度
				}
				tSearchPacketStatus = WITLEAF_SEARCH_PACKET_STATUS_CHECKSUM;
				break;

			case WITLEAF_SEARCH_PACKET_STATUS_CHECKSUM:										// 数据包校验和检测
				if ( u8Checksum == u8CurrentByte ) {
					ptPacket->u8Checksum = u8CurrentByte;
					(* pu8RemainLen) = u8BuffLen - u8Index;				// 返回缓冲区内未参与解包的数据长度
					return 1;
				}
				else {														// 校验和错误，丢弃第一个字节，重新解包
					tSearchPacketStatus = WITLEAF_SEARCH_PACKET_STATUS_HEAD;
					u8Index = u8HeadIndex + 1;							// 丢弃第一个字节，重新解包
					u8Len = u8BuffLen - u8Index;
				}
				break;
			
			default:
				tSearchPacketStatus = WITLEAF_SEARCH_PACKET_STATUS_HEAD;
				u8Index = 0;
				break;
		}   
	}

	return 1;   
}
											
void WitleafRestoreUartRxStr(UINT8 u8Len)
{
	if ( gu8CommInTail >= u8Len ) {
		gu8CommInTail = gu8CommInTail - u8Len;
	}
	else {
		gu8CommInTail = COMM_IN_BUFF_LEN + gu8CommInTail - u8Len;
	}
}

UINT8 pu8WitleafCache[WITLEAF_MAX_FRAME_CACHE_LEN];

// 从主机获得命令，中止命令或通讯出错返回－1；无命令或尚未传输完，返回0；得到完整命令返回命令代码
void WitleafRcvCmd(void)
{
	UINT8 u8RcvLen;

	UINT8 u8CacheLen;
	UINT8 u8RemainLen;
	UINT8 u8Index;
	
	UINT8 u8ValidPacket;

	gtWitleafPacket.u8Id = WITLEAF_CMD_NONE;

	u8RcvLen = WitleafGetUartRxBuffLen();

	if ( u8RcvLen < WITLEAF_MIN_FRAME_LEN ) {
		return;
	}
	else if ( u8RcvLen > WITLEAF_MAX_FRAME_CACHE_LEN ) {						// 增加单次最大解包数据长度限制
		u8RcvLen = WITLEAF_MAX_FRAME_CACHE_LEN;
	}

	u8CacheLen = WitleafGetUartRxStr(pu8WitleafCache, u8RcvLen);								// 读取缓冲区数据内容

	u8Index = 0;
//	u8ValidPacket = 1;
	u8ValidPacket = 0;
	
//	while ( (u8CacheLen >= MIN_WITLEAF_FRAME_LEN) && ( 1 == u8ValidPacket) ) {
	if ( u8CacheLen >= WITLEAF_MIN_FRAME_LEN ) {
		u8ValidPacket = WitleafUnpackUartData(&pu8WitleafCache[u8Index], u8CacheLen, &gtWitleafPacket, &u8RemainLen);
				
		u8Index = u8RcvLen - u8RemainLen;							// 更新缓冲区的输入下标
		u8CacheLen = u8RemainLen;				// 更新缓冲区剩余字节的长度
		
//		if ( u8ValidPacket ) {
//			break;
//		}
	}

	if ( 0 != u8CacheLen ) {
		WitleafRestoreUartRxStr(u8CacheLen);									// 恢复UART接收缓冲区内容
	}

	if ( 1 == u8ValidPacket )
	{
		if ( WITLEAF_MODULE_SPO2 != gtWitleafPacket.u8ParaType ) 
		{
			WitleafSndRcvCmdResult(gtWitleafPacket.u32No, WITLEAF_RCV_CMD_RESULT_MODULE_ERROR);
		}
		else if ( (WITLEAF_PACKET_TYPE_DC != gtWitleafPacket.u8Type) && (WITLEAF_PACKET_TYPE_DR != gtWitleafPacket.u8Type) ) 
		{
			WitleafSndRcvCmdResult(gtWitleafPacket.u32No, WITLEAF_RCV_CMD_RESULT_TYPE_ERROR);
		}
	}

}

//把得到的数据包发送至串口上传到上位机中
void WitleafWriteToBuff(UINT8 * pu8Temp, UINT8 u8Len)
{
	UINT8 i = 0;
	
  USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
	for ( i = 0; i < u8Len; i ++ ) 
	{
		gpu8CommStoreBuff[gu16CommStoreTail] = (* pu8Temp);
		gu16CommStoreTail ++;
		if ( gu16CommStoreTail > COMM_OUT_BUFF_LEN )		//COMM_OUT_BUFF_LEN为 0x08FF ?
		{
			gu16CommStoreTail = 0;
		}
		pu8Temp ++;
	}	        
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //使能串口发送中断  
}

 
void USART_Transmit_Buffer_Data()
{
	if(gu16CommStoreTail != gu16CommStoreHead)
   {
			USART_SendData(USART1, gpu8CommStoreBuff[gu16CommStoreHead]);
			gu16CommStoreHead++;
			
			if(COMM_OUT_BUFF_LEN < gu16CommStoreHead)
			{
					gu16CommStoreHead = 0;
			}
		  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    }
	else
	{
		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);  //发送数据完成后需要关闭发送中断
	}
}


/*打包函数,用于计算校验和添加于数据包中，再将包中各个元素添加到数组gpu8WitleafCommOutBuff中，发至串口*/
void WitleafSndPacket(WITLEAF_PACKET tPacket)
{
	UINT8 i = 0;

	UINT8 u8Checksum = 0;  //定义校验和变量
	u8Checksum += tPacket.u8Len;				//包长度
	u8Checksum += tPacket.u8ParaType;				//参数类型
	u8Checksum += tPacket.u8Type;							//包类型
	u8Checksum += tPacket.u8Id;								//数据包ID	

	u8Checksum += tPacket.u32No & 0xFF;				//包序列，合并成32位
	u8Checksum += (tPacket.u32No >> 8) & 0xFF;
	u8Checksum += (tPacket.u32No >> 16) & 0xFF;
	u8Checksum += (tPacket.u32No >> 24) & 0xFF;
	//
	for ( i = 0; i < tPacket.u8Len - 10; i ++ ) {
		u8Checksum += tPacket.pu8Data[i];
	}

	gpu8WitleafCommOutBuff[0] = WITLEAF_PACKET_HEAD;		//包起始字符0xFA
	gpu8WitleafCommOutBuff[1] = tPacket.u8Len;
	gpu8WitleafCommOutBuff[2] = tPacket.u8ParaType;
	gpu8WitleafCommOutBuff[3] = tPacket.u8Type;
	gpu8WitleafCommOutBuff[4] = tPacket.u8Id;
	gpu8WitleafCommOutBuff[5] = tPacket.u32No & 0xFF;
	gpu8WitleafCommOutBuff[6] = (tPacket.u32No >> 8) & 0xFF;
	gpu8WitleafCommOutBuff[7] = (tPacket.u32No >> 16) & 0xFF;
	gpu8WitleafCommOutBuff[8] = (tPacket.u32No >> 24) & 0xFF;
	for ( i = 0; i < tPacket.u8Len - 10; i ++ ) {
		gpu8WitleafCommOutBuff[i + 9] = tPacket.pu8Data[i];
	}
	gpu8WitleafCommOutBuff[tPacket.u8Len - 1] = u8Checksum;

	WitleafWriteToBuff(gpu8WitleafCommOutBuff, tPacket.u8Len);
}

// 向上位机返回脑电数据
void WitleafSndWavedata(INT32 eeg1,INT32 eeg2,INT32 eeg3,INT32 eeg4,INT32 eeg5,INT32 eeg6,INT32 eeg7,INT32 eeg8)
{
	gtWitleafPacket.u8Len = 34;
	gtWitleafPacket.u8ParaType = 	WITLEAF_MODULE_RESP;
	gtWitleafPacket.u8Type = WITLEAF_PACKET_TYPE_DD;
	gtWitleafPacket.u8Id = WITLEAF_SPO2_DATA;
	gtWitleafPacket.u32No = 0;

	gtWitleafPacket.pu8Data[0] = (UINT8) (eeg1>>24);	//先发高八位
  gtWitleafPacket.pu8Data[1] = (UINT8) (eeg1>>16);	//再发中八位
	gtWitleafPacket.pu8Data[2] = (UINT8) (eeg1>>8);	//后发低八位
	
	gtWitleafPacket.pu8Data[3] = (UINT8) (eeg2>>24);	//先发高八位
  gtWitleafPacket.pu8Data[4] = (UINT8) (eeg2>>16);	//再发中八位
	gtWitleafPacket.pu8Data[5] = (UINT8) (eeg2>>8);	//后发低八位
	
	gtWitleafPacket.pu8Data[6] = (UINT8) (eeg3>>24);	//先发高八位
  gtWitleafPacket.pu8Data[7] = (UINT8) (eeg3>>16);	//再发中八位
	gtWitleafPacket.pu8Data[8] = (UINT8) (eeg3>>8);	//后发低八位
	
	gtWitleafPacket.pu8Data[9] = (UINT8) (eeg4>>24);	//先发高八位
  gtWitleafPacket.pu8Data[10] = (UINT8) (eeg4>>16);	//再发中八位
	gtWitleafPacket.pu8Data[11] = (UINT8) (eeg4>>8);	//后发低八位
	
	gtWitleafPacket.pu8Data[12] = (UINT8) (eeg5>>24);	//先发高八位
  gtWitleafPacket.pu8Data[13] = (UINT8) (eeg5>>16);	//再发中八位
	gtWitleafPacket.pu8Data[14] = (UINT8) (eeg5>>8);	//后发低八位
	
	gtWitleafPacket.pu8Data[15] = (UINT8) (eeg6>>24);	//先发高八位
  gtWitleafPacket.pu8Data[16] = (UINT8) (eeg6>>16);	//再发中八位
	gtWitleafPacket.pu8Data[17] = (UINT8) (eeg6>>8);	//后发低八位
	
	gtWitleafPacket.pu8Data[18] = (UINT8) (eeg7>>24);	//先发高八位
  gtWitleafPacket.pu8Data[19] = (UINT8) (eeg7>>16);	//再发中八位
	gtWitleafPacket.pu8Data[20] = (UINT8) (eeg7>>8);	//后发低八位
	
	gtWitleafPacket.pu8Data[21] = (UINT8) (eeg8>>24);	//先发高八位
  gtWitleafPacket.pu8Data[22] = (UINT8) (eeg8>>16);	//再发中八位
	gtWitleafPacket.pu8Data[23] = (UINT8) (eeg8>>8);	//后发低八位

	WitleafSndPacket(gtWitleafPacket);	
}

void WitleafSndRcvCmdResult(UINT32 u32No, WITLEAF_RCV_CMD_RESULT_LIST tResult)
{
	gtWitleafPacket.u8Len = 10 + 1;
	gtWitleafPacket.u8ParaType = WITLEAF_MODULE_SPO2;
	gtWitleafPacket.u8Type = WITLEAF_PACKET_TYPE_DA;
	gtWitleafPacket.u8Id = WITLEAF_RESPONSE_ANSWER;
	gtWitleafPacket.u32No = u32No;
	gtWitleafPacket.pu8Data[0] = tResult ;
	WitleafSndPacket(gtWitleafPacket);
}

/*
上电握手处理函数，当模块刚上电时程序在次阻塞，模块以1Hz的频率上传上电握手数据包与主机进行上电握手
在上电握手过程中模块不接受除上电握手控制包以外的气体主机命令。
当与主机上电握手成功后模块退出上电握手模式进入到正常模式，接受主机的所有命令
*/
void HandShakeProc(void)
{
		    
	if(gu8ConnectionFlag==0){
			
			gpu8ConnectionOf1sTimer = 500;
			gtWitleafPacket.u8Len = 10;
			gtWitleafPacket.u8ParaType = WITLEAF_MODULE_SPO2;
			gtWitleafPacket.u8Type = WITLEAF_PACKET_TYPE_DD;
			gtWitleafPacket.u8Id = WITLEAF_POWERON_HAND_REQUEST;					   //上电握手数据包ID
			gtWitleafPacket.u32No = gu32WitleafDataNo;
			gu32WitleafDataNo++; //序列号递增

			WitleafSndPacket(gtWitleafPacket);   //以1Hz的频率上传上电握手数据包
		}

	
}

