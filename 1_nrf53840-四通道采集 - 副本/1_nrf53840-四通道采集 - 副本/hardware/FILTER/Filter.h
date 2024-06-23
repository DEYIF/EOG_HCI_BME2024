#ifndef __Filter_H
#define __Filter_H

#include "DataType.h"
	    													 
INT32 SampleRate250Filter_EEG_HP(INT32 data);
INT32 SampleRate250Filter_EEG_LP(INT32 data);
INT32 IIR_Notch_EEG(INT32 data);
INT32 Point5Smooth(INT32 Data);

#endif

