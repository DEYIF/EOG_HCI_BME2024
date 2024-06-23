#include "filter.h"
#include "math.h"

/**
*  采样频率: 250Hz

*  陷波频率: 50Hz

*         0.796084903553*X(n) - 0.4920075283264*X(n-1) + 0.796084903553*X(n-2)
*  H(z) = -----------------------------------------------------------
*               Y(n) -0.4920075283264*Y(n-1) + 0.5921698071059*Y(n-2)
*
**/
float X_DataNotch_EEG[3] = {0,0,0};
float Y_DataNotch_EEG[3] = {0,0,0};
INT32 IIR_Notch_EEG(INT32 data)
{

	X_DataNotch_EEG[0] = data;
	
		  Y_DataNotch_EEG[0] = 0.9755351247463*(X_DataNotch_EEG[0] + X_DataNotch_EEG[2]) - 0.6029138643126*X_DataNotch_EEG[1]
	              + 0.6029138643126*Y_DataNotch_EEG[1] - 0.9510702494926*Y_DataNotch_EEG[2];//Fnotch：50Hz Bandwidth：2Hz 3dB
	
	
	Y_DataNotch_EEG[2] = Y_DataNotch_EEG[1];
	Y_DataNotch_EEG[1] = Y_DataNotch_EEG[0];
	
	X_DataNotch_EEG[2] = X_DataNotch_EEG[1];
	X_DataNotch_EEG[1] = X_DataNotch_EEG[0];
	
	return Y_DataNotch_EEG[0];
 
}


/**
*  采样频率: 250Hz

*  高通频率: 0.05Hz、0.1Hz、0.15Hz、0.2Hz、0.3Hz、0.5Hz

*         0.8901359304*X(n) - 1.780271861*X(n-1) + 0.8901359304*X(n-2)
*  H(z) = -----------------------------------------------------------
*               Y(n) -1.997495020*Y(n-1) + 0.99750074252*Y(n-2)
*
**/
float X_Data_EEG_HP[3] = {0,0,0};
float Y_Data_EEG_HP[3] = {0,0,0};
INT32 SampleRate250Filter_EEG_HP(INT32 data)
{
	X_Data_EEG_HP[0] = data;
	
//	Y_Data_EEG_HP[0] = 0.9911535951017*(X_Data_EEG_HP[0] + X_Data_EEG_HP[2]) - 1.982307190203*X_Data_EEG_HP[1]   //0.5Hz
//	              + 1.982228929793*Y_Data_EEG_HP[1] - 0.9823854506141*Y_Data_EEG_HP[2];
	
//	Y_Data_EEG_HP[0] = 0.9946827273808*(X_Data_EEG_HP[0] + X_Data_EEG_HP[2]) - 1.989365454762*X_Data_EEG_HP[1]  //0.3Hz
//	              + 1.989337181174*Y_Data_EEG_HP[1] - 0.9893937283496*Y_Data_EEG_HP[2];
	
	Y_Data_EEG_HP[0] = 0.9964520026995*(X_Data_EEG_HP[0] + X_Data_EEG_HP[2]) - 1.992904005399*X_Data_EEG_HP[1]  //0.2Hz
	              + 1.992891417074*Y_Data_EEG_HP[1] - 0.9929165937234*Y_Data_EEG_HP[2];

//	Y_Data_EEG_HP[0] = 0.9973378201333*(X_Data_EEG_HP[0] + X_Data_EEG_HP[2]) - 1.994675640267*X_Data_EEG_HP[1]  //0.15Hz
//	              + 1.994668553052*Y_Data_EEG_HP[1] - 0.9946827274809*Y_Data_EEG_HP[2];

//	Y_Data_EEG_HP[0] =  0.9982244250252*(X_Data_EEG_HP[0] + X_Data_EEG_HP[2]) - 1.99644885005*X_Data_EEG_HP[1]  //0.1Hz
//	              + 1.996445697381*Y_Data_EEG_HP[1] - 0.9964520027193*Y_Data_EEG_HP[2];
	
//	Y_Data_EEG_HP[0] = 0.9991118180796*(X_Data_EEG_HP[0] + X_Data_EEG_HP[2]) - 1.998223636159*X_Data_EEG_HP[1]  //0.05Hz
//	              + 1.998222847292*Y_Data_EEG_HP[1] - 0.9982244250264*Y_Data_EEG_HP[2];
	
	
	Y_Data_EEG_HP[2] = Y_Data_EEG_HP[1];
	Y_Data_EEG_HP[1] = Y_Data_EEG_HP[0];
	
	X_Data_EEG_HP[2] = X_Data_EEG_HP[1];
	X_Data_EEG_HP[1] = X_Data_EEG_HP[0];
	
	return Y_Data_EEG_HP[0];
}


/**
*  采样频率: 250Hz

*  低通频率: 3Hz、3.25Hz、4Hz、5Hz、8Hz、10Hz、30Hz

*         0.00362337859*X(n) - 0.00724675718*X(n-1) + 0.00362337859*X(n-2)
*  H(z) = -----------------------------------------------------------
*               Y(n) -1.8550593924*Y(n-1) + 0.8713213829*Y(n-2)
*
**/
float X_Data_EEG_LP[3] = {0,0,0};
float Y_Data_EEG_LP[3] = {0,0,0};
INT32 SampleRate250Filter_EEG_LP(INT32 data)
{
	X_Data_EEG_LP[0] = data;
	
//	Y_Data_EEG_LP[0] = 0.001348711948356*(X_Data_EEG_LP[0] + X_Data_EEG_LP[2]) + 0.002697423896713*X_Data_EEG_LP[1] //3hz
//	              + 1.893464146362*Y_Data_EEG_LP[1] - 0.8988589941553*Y_Data_EEG_LP[2];
	
//	Y_Data_EEG_LP[0] = 0.001576105306545*(X_Data_EEG_LP[0] + X_Data_EEG_LP[2]) + 0.003152210613091*X_Data_EEG_LP[1] //3.25hz
//	              + 1.884602940861*Y_Data_EEG_LP[1] - 0.8909073620871*Y_Data_EEG_LP[2];

//	Y_Data_EEG_LP[0] = 0.002357208772852 *(X_Data_EEG_LP[0] + X_Data_EEG_LP[2]) + 0.004714417545705 *X_Data_EEG_LP[1] //4hz
//	              + 1.8580432987*Y_Data_EEG_LP[1] -  0.8674721337917*Y_Data_EEG_LP[2];
	
//	Y_Data_EEG_LP[0] = 0.003621681514929*(X_Data_EEG_LP[0] + X_Data_EEG_LP[2]) + 0.007243363029857*X_Data_EEG_LP[1] //5hz
//	              + 1.822694925196*Y_Data_EEG_LP[1] - 0.837181651256*Y_Data_EEG_LP[2];
	
//	Y_Data_EEG_LP[0] = 0.008826086668431*(X_Data_EEG_LP[0] + X_Data_EEG_LP[2]) + 0.01765217333686*X_Data_EEG_LP[1] //8hz
//	              + 1.717211834908*Y_Data_EEG_LP[1] - 0.7525161815818*Y_Data_EEG_LP[2];
	
//	Y_Data_EEG_LP[0] = 0.01335920002786*(X_Data_EEG_LP[0] + X_Data_EEG_LP[2]) + 0.02671840005571*X_Data_EEG_LP[1] //10hz
//	              + 1.647459981077*Y_Data_EEG_LP[1] - 0.7008967811884*Y_Data_EEG_LP[2];
		
		Y_Data_EEG_LP[0] = 0.04613180209331*(X_Data_EEG_LP[0] + X_Data_EEG_LP[2]) + 0.09226360418663*X_Data_EEG_LP[1] //20hz
	              + 1.307285028849*Y_Data_EEG_LP[1] - 0.4918122372226*Y_Data_EEG_LP[2];
	
//	Y_Data_EEG_LP[0] = 0.09131490043583*(X_Data_EEG_LP[0] + X_Data_EEG_LP[2]) + 0.1826298008717*X_Data_EEG_LP[1] //30hz
//	              + 0.9824057931084*Y_Data_EEG_LP[1] - 0.3476653948517*Y_Data_EEG_LP[2];
	
	
	Y_Data_EEG_LP[2] = Y_Data_EEG_LP[1];
	Y_Data_EEG_LP[1] = Y_Data_EEG_LP[0];
	
	X_Data_EEG_LP[2] = X_Data_EEG_LP[1];
	X_Data_EEG_LP[1] = X_Data_EEG_LP[0];
	
	return Y_Data_EEG_LP[0];
}

//5点线性平滑滤波
INT32 SmoothBuffer[5]={0};
INT32 SmoothResult = 0;
INT32 Point5Smooth(INT32 Data)
{
	static u8 mi = 0;
	mi++;
  SmoothBuffer[4] = Data;
	
	SmoothBuffer[0] = SmoothBuffer[1];
	SmoothBuffer[1] = SmoothBuffer[2];
	SmoothBuffer[2] = SmoothBuffer[3];
	SmoothBuffer[3] = SmoothBuffer[4];
	
	if(mi==5)
	{
		mi=4;
	  SmoothResult = (12*(SmoothBuffer[1] + SmoothBuffer[3]) - 3*(SmoothBuffer[0] + SmoothBuffer[4]) 
		                    + 17*SmoothBuffer[2])/35.0;
	}
	
  return SmoothResult; 
}
