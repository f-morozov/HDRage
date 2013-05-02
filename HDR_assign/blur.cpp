#include "tonemap.h"

#include "convolutionFFT2D_common.h"
#include <cufft.h>
#include <cuda.h>
#include<cuda_runtime.h>
#include <cmath>

#include <iostream>
using namespace std;

HDRImage *makeGaussian(int width, int height, float sigma)
{
	float sigma2 = sigma * sigma;
	float pi = 3.1415926f;
	HDRImage *res = new HDRImage(width, height, 1);
	float *ptr = res->getPtr();
	int xc = width / 2;
	int yc = height / 2;
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {

			*ptr++ = exp(-((x - xc)*(x - xc)+(y - yc)*(y - yc)) / (2*sigma2)) / (2 * pi * sigma2);
		}
	}
	return res;
}

int snapTransformSize(int dataSize)
{
    int hiBit;
    unsigned int lowPOT, hiPOT;

    dataSize = iAlignUp(dataSize, 16);

    for (hiBit = 31; hiBit >= 0; hiBit--)
        if (dataSize & (1U << hiBit))
        {
            break;
        }

    lowPOT = 1U << hiBit;

    if (lowPOT == (unsigned int)dataSize)
    {
        return dataSize;
    }

    hiPOT = 1U << (hiBit + 1);

    if (hiPOT <= 1024)
    {
        return hiPOT;
    }
    else
    {
        return iAlignUp(dataSize, 512);
    }
}

void blurImage(float *img, float *res, float sigma, int width, int height)
{
	int ker_width = width;
	int ker_height = height;

	float *data = img;

    cufftHandle fftPlanFwd, fftPlanInv;

    int fft_height = snapTransformSize(height + ker_width - 1);
    int fft_width = snapTransformSize(width + ker_height - 1);

	float *gpu_data, *gpu_kernel, *padded_data, *padded_kernel;
	fComplex *data_transform, *kernel_transform;

	gpu_kernel = makeGaussianGpu(ker_width, ker_height, sigma);

	cudaMalloc((void **)&gpu_data, width * height * sizeof(float));

    cudaMalloc((void **)&padded_data,   fft_height * fft_width * sizeof(float));
    cudaMalloc((void **)&padded_kernel, fft_height * fft_width * sizeof(float));

    cudaMalloc((void **)&data_transform,   fft_height * (fft_width / 2 + 1) * sizeof(fComplex));
    cudaMalloc((void **)&kernel_transform, fft_height * (fft_width / 2 + 1) * sizeof(fComplex));

    cufftPlan2d(&fftPlanFwd, fft_height, fft_width, CUFFT_R2C);
    cufftPlan2d(&fftPlanInv, fft_height, fft_width, CUFFT_C2R);

    cudaMemcpy(gpu_data,   data,   width * height * sizeof(float), cudaMemcpyHostToDevice);
    cudaMemset(padded_data, 0, fft_height * fft_width * sizeof(float));
    cudaMemset(padded_kernel, 0, fft_height * fft_width * sizeof(float));

    padKernel(
        padded_kernel,
        gpu_kernel,
        fft_height,
        fft_width,
        ker_height,
        ker_width,
        ker_height / 2,
        ker_width / 2
    );

    padDataClampToBorder(
        padded_data,
        gpu_data,
        fft_height,
        fft_width,
        height,
        width,
        ker_height,
        ker_width,
        ker_height / 2,
        ker_width / 2
    );

    cufftExecR2C(fftPlanFwd, (cufftReal *)padded_kernel, (cufftComplex *)kernel_transform);    
	cufftExecR2C(fftPlanFwd, (cufftReal *)padded_data, (cufftComplex *)data_transform);
    modulateAndNormalize(data_transform, kernel_transform, fft_height, fft_width, 1);
    cufftExecC2R(fftPlanInv, (cufftComplex *)data_transform, (cufftReal *)padded_data);

	HDRImage *fftresImg = new HDRImage(fft_width, fft_height, 1);
	float *fftres = fftresImg->getPtr();
    cudaMemcpy(fftres, padded_data, fft_height * fft_width * sizeof(float), cudaMemcpyDeviceToHost);

	for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

			*res++ = fftres[y*fft_width + x];
		}
	}
	delete fftresImg;
	cudaFree(gpu_data);
	cudaFree(gpu_kernel);
	cudaFree(padded_data);
	cudaFree(padded_kernel);
	cudaFree(data_transform);
	cudaFree(kernel_transform);
	cufftDestroy(fftPlanFwd);
	cufftDestroy(fftPlanInv);
}

void blurMap(HDRImage *img, float k, float sigma, float correction)
{
	int width = img->getWidth();
	int height = img->getHeight();
	int total = width * height;
	HDRImage *lum = getLum(img, NULL);
	HDRImage *filtered = new HDRImage(width, height, 1);
	blurImage(lum->getPtr(), filtered->getPtr(), sigma, width, height);
	float *imgPtr = img->getPtr();
	float *lumPtr = lum->getPtr();
	float *filtPtr = filtered->getPtr();

	for(int i = 0; i < total; i++) {

		for(int c = 0; c < 3; c++) {

			*imgPtr++ = pow(*imgPtr / (k * *filtPtr), 1.0f / correction);
		}
		filtPtr++;
	}

	delete lum;
	delete filtered;
}