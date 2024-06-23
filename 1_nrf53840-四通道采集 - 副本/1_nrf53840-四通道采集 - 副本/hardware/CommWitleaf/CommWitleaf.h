#ifndef _COMMWITLEAF_H_
#define _COMMWITLEAF_H_

#include "stm32f10x.h"
#include "Datatype.h"
#include "WitleafProtocol.h"
#include "variable.h"


void WitleafWriteToBuff(UINT8 * pu8Temp, UINT8 u8Len);
void USART_Transmit_Buffer_Data(void);
void WitleafRcvCmd(void);
void WitleafSndWavedata(INT32 eeg1,INT32 eeg2,INT32 eeg3,INT32 eeg4,INT32 eeg5,INT32 eeg6,INT32 eeg7,INT32 eeg8);
void WitleafSndRcvCmdResult(UINT32 u32No, WITLEAF_RCV_CMD_RESULT_LIST tResult);
void HandShakeProc(void);

#endif

