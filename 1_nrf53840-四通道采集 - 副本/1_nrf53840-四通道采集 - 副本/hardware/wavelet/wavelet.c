
#include "wavelet.h"

void WaveletDwt(double sourceData[], int dataLen, double *cA, double *cD) // rawData, the length of rawData , cA, cD
{
    int filterLen = 8; // 滤波器的长度
    int n, k, p;
    int decLen = (dataLen + filterLen - 1) / 2; // 计算离散小波变换后的长度
    double tmp = 0;
    
    // 对每一个变换后的系数进行计算
    for (n = 0; n < decLen; n++)
    {
        cA[n] = 0; // 初始化近似系数
        cD[n] = 0; // 初始化细节系数
        
        // 对滤波器进行卷积
        for (k = 0; k < filterLen; k++)
        {
            p = 2 * n - k + 1; // 计算卷积位置
            
            // 根据卷积位置不同情况获取源数据，实现边界处理
            if ((p < 0) && (p >= -filterLen + 1))
                tmp = sourceData[-p - 1]; // 边界外，使用源数据的反向索引
            else if ((p > dataLen - 1) && (p <= dataLen + filterLen - 2))
                tmp = sourceData[2 * dataLen - p - 1]; // 边界外，使用源数据的反向索引
            else if ((p >= 0) && (p < dataLen - 1 + 1))
                tmp = sourceData[p]; // 边界内，直接使用源数据
            else
                tmp = 0; // 边界外，填充0
            
            cA[n] += db4_Lo_D[k] * tmp; // 计算近似系数
            cD[n] += db4_Hi_D[k] * tmp; // 计算细节系数
        }
    }
    return; // 返回
}

/*******************五层小波分解***************************/
void WaveletDB4(double sourceData[], int dataLen, double *C, int *L)
{
    // 定义用于存储各层近似系数和细节系数的数组
    double cA[300], cD[300], cA1[300], cD1[300];
    double cA2[300], cD2[300], cA3[300], cD3[300];
    double cA4[300],cD4[300];  
	
    // 将原始数据长度记录到L数组的第一个位置
    L[0] = dataLen;
    int i;
    
    // 第一层小波分解
    WaveletDwt(sourceData, dataLen, cA, cD); //One-layer decomposition 
    L[1] = (dataLen + 7) / 2; // 计算第一层分解后的长度
    for (i = 0; i < L[1]; i++)
    {
        C[i] = cD[i]; // 将第一层细节系数存储到数组C中
    }
    
    // 第二层小波分解
    WaveletDwt(cA, L[1], cA1, cD1); //Two-layer decomposition 
    L[2] = (L[1] + 7) / 2; // 计算第二层分解后的长度
    for (i = L[1]; i < L[1] + L[2]; i++)
    {
        C[i] = cD1[i - L[1]]; // 将第二层细节系数存储到数组C中
    }
    
    // 第三层小波分解
    WaveletDwt(cA1, L[2], cA2, cD2); //Three-layer decomposition 
    L[3] = (L[2] + 7) / 2; // 计算第三层分解后的长度
    for (i = L[1] + L[2]; i < L[1] + L[2] + L[3]; i++)
    {
        C[i] = cD2[i - L[1] - L[2]]; // 将第三层细节系数存储到数组C中
    }
    
    // 第四层小波分解
    WaveletDwt(cA2, L[3], cA3, cD3); //Four-layer decomposition 
    L[4] = (L[3] + 7) / 2; // 计算第四层分解后的长度
    for (i = L[1] + L[2] + L[3]; i < L[1] + L[2] + L[3] + L[4]; i++)
    {
        C[i] = cD3[i - L[1] - L[2] - L[3]]; // 将第四层细节系数存储到数组C中
    }
    
		// 第五层小波分解
    WaveletDwt(cA3, L[4], cA4, cD4); //Four-layer decomposition 
    L[5] = (L[4] + 7) / 2; // 计算第五层分解后的长度
    for (i = L[1] + L[2] + L[3] + L[4]; i < L[1] + L[2] + L[3] + L[4] + L[5]; i++)
    {
        C[i] = cD4[i - L[1] - L[2] - L[3] - L[4]]; // 将第四层细节系数存储到数组C中
    }
		
    // 将最后一层的近似系数存储到数组C中
    L[6] = (L[5] + 7) / 2;
    for (i = L[1] + L[2] + L[3] + L[4] + L[5]; i < L[1] + L[2] + L[3] + L[4] + L[5] + L[6]; i++)
    {
        C[i] = cA4[i - L[1] - L[2] - L[3] - L[4] - L[5]];
    }
    return;
}

/****************************************************************************/
void WaveletIdwt_CD(double cD[], int cALength, double *recData, int recLength)
{
    //int filterLen = 8; // 小波滤波器的长度
    int recLen = recLength; // 恢复数据的长度
    
    int num = cALength * 2; // 计算临时数组的长度，为细节系数长度的两倍
	
    double *temp = (double *)malloc(num * sizeof(double)); // 分配临时数组的内存空间
    
    int k = 0;
    // 上采样
    for (int n = 0; n < num; n++)
    {
        if (n % 2 == 0)
        {
            temp[n] = 0; // 偶数索引位置填充0
        }
        else
        {
            temp[n] = cD[k]; // 奇数索引位置填充细节系数
            k++;
        }
    }
    
    int num_conv = cALength * 2 + 8 - 1; // 计算卷积后结果数组的长度		
    double *xx = (double *)malloc(num_conv * sizeof(double)); // 分配存储卷积结果的数组内存空间
    
    // 初始化卷积结果数组
    for (int i = 0; i < num_conv; i++) {
        xx[i] = 0;
    }
    
    // 卷积过程
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < cALength * 2; j++) {
            xx[i + j] += temp[j] * db4_Hi_R[i]; // 使用Daubechies-4小波的高通滤波器进行卷积
        }
    }
    
    // 将卷积结果存储到恢复数据数组中
    for (int i = 7; i < recLen + 7; i++) {
        recData[i - 7] = xx[i];
    }

    // 释放临时数组和卷积结果数组的内存空间
    free(temp);
    free(xx);
    return;
}

void WaveletIdwt_CA(double *cA, int cALength, double *recData, int recLength)
{
	//int filterLen = 8; // 小波滤波器长度（这里假设为8）
	int recLen = recLength; // 重构数据长度

	int num = cALength * 2; // 计算临时数组长度，为近似系数长度的两倍
	double *temp = (double *)malloc(num * sizeof(double)); // 分配临时数组内存空间

	int k = 0;
	// 上采样
	for (int n = 0; n < num; n++)
	{
		if (n % 2 == 0)
		{
			temp[n] = 0; // 偶数索引位置填充0
		}
		else
		{
			temp[n] = cA[k]; // 奇数索引位置填充近似系数
			k++;
		}
	}

	int num_conv = cALength * 2 + 8 - 1; // 计算卷积后结果数组的长度
	double *xx = (double *)malloc(num_conv * sizeof(double)); // 分配存储卷积结果的数组内存空间

	// 初始化卷积结果数组
	for (int i = 0; i < num_conv; i++) {
		xx[i] = 0;
	}

	// 卷积过程
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < cALength * 2; j++) {
			xx[i + j] += temp[j] * db4_Lo_R[i]; // 使用Daubechies-4小波的低通滤波器进行卷积
		}
	}

	// 将卷积结果存储到重构数据数组中
	for (int i = 7; i < recLen + 7; i++) {
		recData[i - 7] = xx[i];
	}

	// 释放临时数组和卷积结果数组的内存空间
	free(temp);
	free(xx);
	return;
}

/******************重构cD1***********************/
void getcD1(double *C, int *L, double *cD1) {
	// 函数声明：重构一级细节系数 cD1，输入参数为原始细节系数 C、长度信息 L，输出参数为重构的一级细节系数 cD1
	int recLen = L[0]; // 获取重构数据的长度
	int num = L[1]; // 获取细节系数的长度

	// 分配内存并复制原始细节系数到临时数组 cD
	double *cD = (double *)malloc(num * sizeof(double));
	for (int i = 0; i < num; i++) {
		cD[i] = C[i];
	}

	// 调用另一个函数来执行一级离散小波反变换，将 cD 重构为 cD1
	WaveletIdwt_CD(cD, num, cD1, recLen);

	// 释放临时数组的内存空间
	free(cD);
	return;
}

/******************重构cD2***********************/
void getcD2(double *C, int *L, double *cD2) {
	// 函数声明：重构二级细节系数 cD2，输入参数为原始细节系数 C、长度信息 L，输出参数为重构的二级细节系数 cD2

	int recLen = L[0]; // 获取重构数据的长度
	int num_cd1 = L[1]; // 获取一级细节系数的长度
	int num_cd2 = L[2]; // 获取二级细节系数的长度

	// 分配内存并复制原始细节系数的部分到临时数组 cD
	double *cD = (double *)malloc(num_cd2 * sizeof(double)); // 分配内存空间用于存储部分原始细节系数 cD
	double *rec1 = (double *)malloc(num_cd1 * sizeof(double)); // 分配内存空间用于存储一级细节系数 rec1
	
	for (int i = num_cd1; i < num_cd1 + num_cd2; i++) {
		cD[i - num_cd1] = C[i]; // 将原始细节系数中的部分复制到临时数组 cD 中
	}

	// 一级离散小波反变换，将 cD 重构为一级细节系数 rec1
	WaveletIdwt_CD(cD, num_cd2, rec1, num_cd1);

	// 一级离散小波反变换，将一级细节系数 rec1 与原始近似系数 C 重构为二级细节系数 cD2
	WaveletIdwt_CA(rec1, num_cd1, cD2, recLen);

	// 释放内存空间
	free(cD);
	free(rec1);
	return;
}

/******************重构cD3***********************/
void getcD3(double *C, int *L, double *cD3) {
	// 函数声明：重构三级细节系数 cD3，输入参数为原始细节系数C、长度信息L，输出参数为重构的三级细节系数cD3

	int recLen = L[0]; // 获取重构数据的长度
	int num_cd1 = L[1]; // 获取一级细节系数的长度
	int num_cd2 = L[2]; // 获取二级细节系数的长度
	int num_cd3 = L[3]; // 获取三级细节系数的长度

	// 分配内存并复制原始细节系数的部分到临时数组 cD
	double *cD = (double *)malloc(num_cd3 * sizeof(double)); // 分配内存空间用于存储部分原始细节系数 cD
	double *rec1 = (double *)malloc(num_cd2 * sizeof(double)); // 分配内存空间用于存储一级细节系数 rec1
	double *rec2 = (double *)malloc(num_cd1 * sizeof(double)); // 分配内存空间用于存储二级细节系数 rec2
	for (int i = num_cd1 + num_cd2; i < num_cd1 + num_cd2 + num_cd3; i++) {
		cD[i - num_cd1 - num_cd2] = C[i]; // 将原始细节系数中的部分复制到临时数组 cD 中
	}

	// 一级离散小波反变换，将 cD 重构为一级细节系数 rec1
	WaveletIdwt_CD(cD, num_cd3, rec1, num_cd2);

	// 一级离散小波反变换，将一级细节系数 rec1 与原始近似系数 C 重构为二级细节系数 rec2
	WaveletIdwt_CA(rec1, num_cd2, rec2, num_cd1);

	// 一级离散小波反变换，将二级细节系数 rec2 与原始近似系数 C 重构为三级细节系数 cD3
	WaveletIdwt_CA(rec2, num_cd1, cD3, recLen);

	// 释放内存空间
	free(cD);
	free(rec1);
	free(rec2);
	return;
}

/******************重构cD4***********************/
void getcD4(double *C, int *L, double *cD4) {
	// 函数声明：重构四级细节系数 cD4，输入参数为原始细节系数C、长度信息L，输出参数为重构的四级细节系数cD4

	int recLen = L[0]; // 获取重构数据的长度
	int num_cd1 = L[1]; // 获取一级细节系数的长度
	int num_cd2 = L[2]; // 获取二级细节系数的长度
	int num_cd3 = L[3]; // 获取三级细节系数的长度
	int num_cd4 = L[4]; // 获取四级细节系数的长度

	// 分配内存并复制原始细节系数的部分到临时数组 cD
	double *cD = (double *)malloc(num_cd4 * sizeof(double)); // 分配内存空间用于存储部分原始细节系数 cD
	double *rec1 = (double *)malloc(num_cd3 * sizeof(double)); // 分配内存空间用于存储一级细节系数 rec1
	double *rec2 = (double *)malloc(num_cd2 * sizeof(double)); // 分配内存空间用于存储二级细节系数 rec2
	double *rec3 = (double *)malloc(num_cd1 * sizeof(double)); // 分配内存空间用于存储三级细节系数 rec3
	for (int i = num_cd1 + num_cd2 + num_cd3; i < num_cd1 + num_cd2 + num_cd3 + num_cd4; i++) {
		cD[i - num_cd1 - num_cd2 - num_cd3] = C[i]; // 将原始细节系数中的部分复制到临时数组 cD 中
	}

	// 一级离散小波反变换，将 cD 重构为一级细节系数 rec1
	WaveletIdwt_CD(cD, num_cd4, rec1, num_cd3);

	// 一级离散小波反变换，将一级细节系数 rec1 与原始近似系数 C 重构为二级细节系数 rec2
	WaveletIdwt_CA(rec1, num_cd3, rec2, num_cd2);

	// 一级离散小波反变换，将二级细节系数 rec2 与原始近似系数 C 重构为三级细节系数 rec3
	WaveletIdwt_CA(rec2, num_cd2, rec3, num_cd1);

	// 一级离散小波反变换，将三级细节系数 rec3 与原始近似系数 C 重构为四级细节系数 cD4
	WaveletIdwt_CA(rec3, num_cd1, cD4, recLen);

	// 释放内存空间
	free(cD);
	free(rec1);
	free(rec2);
	free(rec3);
	return;
}

/******************重构cD5***********************/
void getcD5(double *C, int *L, double *cD5) {
	// 函数声明：重构五级细节系数 cD4，输入参数为原始细节系数C、长度信息L，输出参数为重构的四级细节系数cD4

	int recLen = L[0]; // 获取重构数据的长度
	int num_cd1 = L[1]; // 获取一级细节系数的长度
	int num_cd2 = L[2]; // 获取二级细节系数的长度
	int num_cd3 = L[3]; // 获取三级细节系数的长度
	int num_cd4 = L[4]; // 获取四级细节系数的长度
	int num_cd5 = L[5]; // 获取五级细节系数的长度

	// 分配内存并复制原始细节系数的部分到临时数组 cD
	double *cD = (double *)malloc(num_cd5 * sizeof(double)); // 分配内存空间用于存储部分原始细节系数 cD
	double *rec1 = (double *)malloc(num_cd4 * sizeof(double)); // 分配内存空间用于存储一级细节系数 rec1
	double *rec2 = (double *)malloc(num_cd3 * sizeof(double)); // 分配内存空间用于存储二级细节系数 rec2
	double *rec3 = (double *)malloc(num_cd2 * sizeof(double)); // 分配内存空间用于存储三级细节系数 rec3
	double *rec4 = (double *)malloc(num_cd1 * sizeof(double)); // 分配内存空间用于存储三级细节系数 rec4
	
	for (int i = num_cd1 + num_cd2 + num_cd3 + num_cd4; i < num_cd1 + num_cd2 + num_cd3 + num_cd4 + num_cd5; i++) {
		cD[i - num_cd1 - num_cd2 - num_cd3 - num_cd4] = C[i]; // 将原始细节系数中的部分复制到临时数组 cD 中
	}

	// 一级离散小波反变换，将 cD 重构为一级细节系数 rec1
	WaveletIdwt_CD(cD, num_cd5, rec1, num_cd4);

	// 一级离散小波反变换，将一级细节系数 rec1 与原始近似系数 C 重构为二级细节系数 rec2
	WaveletIdwt_CA(rec1, num_cd4, rec2, num_cd3);

	// 一级离散小波反变换，将二级细节系数 rec2 与原始近似系数 C 重构为三级细节系数 rec3
	WaveletIdwt_CA(rec2, num_cd3, rec3, num_cd2);

	// 一级离散小波反变换，将三级细节系数 rec3 与原始近似系数 C 重构为四级细节系数 rec4
	WaveletIdwt_CA(rec3, num_cd2, rec4, num_cd1);
	
	// 一级离散小波反变换，将四级细节系数 rec4 与原始近似系数 C 重构为四级细节系数 cD5
	WaveletIdwt_CA(rec4, num_cd1, cD5, recLen);

	// 释放内存空间
	free(cD);
	free(rec1);
	free(rec2);
	free(rec3);
	free(rec4);
	return;
}

///******************重构cA5***********************/
void getcA5(double *C, int *L, double *cA5) {
	// 函数声明：重构五级近似系数 cA5，输入参数为原始细节系数C、长度信息L，输出参数为重构的五级近似系数cA5
	int recLen = L[0]; // 获取重构数据的长度
	int num_cd1 = L[1]; // 获取一级细节系数的长度
	int num_cd2 = L[2]; // 获取二级细节系数的长度
	int num_cd3 = L[3]; // 获取三级细节系数的长度
	int num_cd4 = L[4]; // 获取四级细节系数的长度
	int num_cd5 = L[5]; // 获取五级近似系数的长度
	int num_ca5 = L[6]; // 获取五级近似系数的长度
	
	// 分配内存并复制原始细节系数的部分到临时数组 cA
	double *cA = (double *)malloc(num_ca5 * sizeof(double)); // 分配内存空间用于存储部分原始细节系数 cA
	double *rec1 = (double *)malloc(num_cd4 * sizeof(double)); // 分配内存空间用于存储一级细节系数 rec1
	double *rec2 = (double *)malloc(num_cd3 * sizeof(double)); // 分配内存空间用于存储二级细节系数 rec2
	double *rec3 = (double *)malloc(num_cd2 * sizeof(double)); // 分配内存空间用于存储三级细节系数 rec3
	double *rec4 = (double *)malloc(num_cd1 * sizeof(double)); // 分配内存空间用于存储三级细节系数 rec4
	
	for (int i = num_cd1 + num_cd2 + num_cd3 + num_cd4 + num_cd5; i < num_cd1 + num_cd2 + num_cd3 + num_cd4 + num_cd5 + num_ca5; i++) {
		cA[i - num_cd1 - num_cd2 - num_cd3 - num_cd4 - num_cd5] = C[i]; // 将原始细节系数中的部分复制到临时数组 cA 中
	}

	// 一级离散小波反变换，将 cA 重构为一级细节系数 rec1
	WaveletIdwt_CA(cA, num_ca5, rec1, num_cd4);
	
	// 一级离散小波反变换，将一级细节系数 rec1 与原始近似系数 C 重构为二级细节系数 rec2
	WaveletIdwt_CA(rec1, num_cd4, rec2, num_cd3);

	// 一级离散小波反变换，将二级细节系数 rec1 与原始近似系数 C 重构为三级细节系数 rec3
	WaveletIdwt_CA(rec2, num_cd3, rec3, num_cd2);

	// 一级离散小波反变换，将三级细节系数 rec2 与原始近似系数 C 重构为四级细节系数 rec4
	WaveletIdwt_CA(rec3, num_cd2, rec4, num_cd1);

	// 一级离散小波反变换，将四级细节系数 rec3 与原始近似系数 C 重构为五级近似系数 cA5
	WaveletIdwt_CA(rec4, num_cd1, cA5, recLen);

	// 释放内存空间
	free(cA);
	free(rec1);
	free(rec2);
	free(rec3);
	free(rec4);
	return;
}

/********************合成信号***********************/
void reconstructSignal(double* cD1, double* cD2, double* cD3, double* cD4, double* cD5, int L, double* reconstructedSignal) {
    int i;
    // 合成 cD1、cD2、cD3、cD4、cD5、cA5 到 temp 数组中
    for (i = 0; i < L; i++) {
        reconstructedSignal[i] = cD1[i] + cD2[i] + cD3[i] + cD4[i] + cD5[i];
    }
}





