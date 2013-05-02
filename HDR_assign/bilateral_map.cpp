#include "tonemap.h"
#include "convolutionFFT2D_common.h"
#include <cufft.h>
#include <cuda.h>
#include<cuda_runtime.h>
#include <iostream>
#include <cstring>
using namespace std;

extern "C" 
void bilateralFilter(HDRImage *img, HDRImage *res, float lumSigma, float spaceSigma);

//void bilateralFilter(HDRImage *img, HDRImage *res, float lumSigma, float spaceSigma)
//{
//	float *resPtr = res->getPtr();
//	float *imgPtr = img->getPtr();
//	int ker_size = 5;
//	int center = ker_size / 2;
//	HDRImage *kernel = makeGaussian(ker_size, ker_size, spaceSigma);
//	float *kerPtr = kernel->getPtr();
//	int width = img->getWidth();
//	int height = img->getHeight();
//	
//	for( int y = 0; y < height; y++ ) {    
//
//		for( int x = 0; x < width; x++ ) {
//			
//			float sum = 0;
//			float k = 0;
//			float Is = imgPtr[y * width + x];
//			for(int kery = 0; kery < ker_size; kery++) {
//
//				for(int kerx = 0; kerx < ker_size; kerx++) {
//
//					if(x - center + kerx >= 0 && x - center + kerx < width &&
//					   y - center + kery >= 0 && y - center + kery < height)
//					{
//						float Ip = imgPtr[(y - center + kery) * width + x - center + kerx];
//						float a = kerPtr[kery * ker_size + kerx] * exp(-(Ip - Is) * (Ip - Is) / (2.0f * lumSigma));
//						sum += a * Ip;					
//						k += a;
//					}
//				}
//			}
//			if(k > 0)
//				*resPtr++ = sum / k;
//			else
//				*resPtr++ = 0;
//		}
//	}
//}

void bilateralMap(HDRImage *img, float contrast, float lumSigma, float spaceSigma)
{
	HDRImage *lum;
	getLum(img, &lum);
	int width = img->getWidth();
	int height = img->getHeight();
	int total = width * height;
	HDRImage *filtered = new HDRImage(width, height, 1);
	HDRImage *detail = new HDRImage(width, height, 1);

	bilateralFilter(lum, filtered, lumSigma, spaceSigma);

	float *lumPtr = lum->getPtr();
	float *filtPtr = filtered->getPtr();
	float *detPtr = detail->getPtr();

	float max = -2e30f, min = 2e30f;
	for(int pxl = 0; pxl < total; pxl++) {

		if(*filtPtr > max)
			max = *filtPtr;
		if(*filtPtr < min)
			min = *filtPtr;
		*detPtr++ = *lumPtr++ - *filtPtr++;
	}
	
	lumPtr = lum->getPtr();
	filtPtr = filtered->getPtr();
	detPtr = detail->getPtr();
	float *imgPtr = img->getPtr();

	float globalCorr = safeLog(contrast) / (max - min);
	for(int pxl = 0; pxl < total; pxl++) {

		float correction = exp(*filtPtr++ * globalCorr + *detPtr++ - *lumPtr++);
		for(int channel = 0; channel < 3; channel++) {

			*imgPtr++ *=correction;
		}
	}
	delete lum;
	delete filtered;
	delete detail;
}

