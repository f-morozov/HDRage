#include "tonemap.h"

__global__ void bilateralKernel(float *imgPtr, float *resPtr, int width, int height, float *kerPtr, float lumSigma, int ker_size)
{
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	if(x < width && y < height)
	{
		int center = ker_size / 2;
		int idx = width * y + x;
		resPtr += idx;
		float sum = 0;
		float k = 0;
		float Is = imgPtr[y * width + x];
		for(int kery = 0; kery < ker_size; kery++) {

			for(int kerx = 0; kerx < ker_size; kerx++) {

				if(x - center + kerx >= 0 && x - center + kerx < width &&
				   y - center + kery >= 0 && y - center + kery < height)
			    	{
						float Ip = imgPtr[(y - center + kery) * width + x - center + kerx];
						float a = kerPtr[kery * ker_size + kerx] * exp(-(Ip - Is) * (Ip - Is) / (2.0f * lumSigma));
						sum += a * Ip;					
						k += a;
					}
				}
			}
			if(k > 0)
				*resPtr = sum / k;
			else
				*resPtr = 0;
	}
}

extern "C" 
void bilateralFilter(HDRImage *img, HDRImage *res, float lumSigma, float spaceSigma)
{
	int ker_size = 10;//((spaceSigma - 0.8) / 0.3 + 1) * 2;
	int width = img->getWidth();
	int height = img->getHeight();
	HDRImage *kernel = makeGaussian(ker_size, ker_size, spaceSigma);
	
	float *gpuPtr = NULL;
	float *gpuKer = NULL;
	float *gpuRes = NULL;
	cudaMalloc((void**) &gpuPtr, width * height * sizeof(float));
	cudaMalloc((void**) &gpuRes, width * height * sizeof(float));
	cudaMalloc((void**) &gpuKer, ker_size * ker_size * sizeof(float));
		
	cudaMemcpy(gpuPtr, img->getPtr(), width * height * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(gpuKer, kernel->getPtr(), ker_size * ker_size * sizeof(float), cudaMemcpyHostToDevice);

	dim3 threads(16, 16);
	dim3 blocks((width + threads.x - 1) / threads.x, (height + threads.y - 1) / threads.y);

	bilateralKernel<<<blocks, threads>>>(gpuPtr, gpuRes, width, height, gpuKer, lumSigma, ker_size);

	cudaMemcpy(res->getPtr(), gpuRes, width * height * sizeof(float), cudaMemcpyDeviceToHost);
	cudaFree(gpuPtr);
	cudaFree(gpuKer);
	cudaFree(gpuRes);
	delete kernel;
}