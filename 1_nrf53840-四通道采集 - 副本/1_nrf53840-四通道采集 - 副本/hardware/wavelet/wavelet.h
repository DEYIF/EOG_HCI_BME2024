
#ifndef WAVELET_H
#define WAVELET_H

#include <stdint.h>
#include <stdio.h>
#include "math.h"
#include "Datatype.h"
#include <stdlib.h>
	
extern double db4_Lo_D[8];
extern double db4_Hi_D[8];
extern double db4_Lo_R[8];
extern double db4_Hi_R[8];

void WaveletDwt(double sourceData[], int dataLen, double *cA, double *cD);
void WaveletDB4(double sourceData[], int dataLen, double *C, int *L);
void WaveletDB4(double sourceData[], int dataLen, double *C, int *L);
void WaveletIdwt_CA(double *cA, int cALength, double *recData, int recLength);	

void getcD1(double *C, int *L, double *cD1);
void getcD2(double *C, int *L, double *cD2);
void getcD3(double *C, int *L, double *cD3);
void getcD4(double *C, int *L, double *cD4);
void getcD5(double *C, int *L, double *cD5);
void getcA5(double *C, int *L, double *cA5);

void reconstructSignal(double* cD1, double* cD2, double* cD3, double* cD4, double* cD5, int L, double* reconstructedSignal);
		
#endif /* WAVELET_H */
