#include <iostream>
using namespace std;

__device__ float slog(float x) {

	if(x < 4.6e-5f)
		return -10.0f;
	return logf(x);
}

__global__ void logMapKernel(float *ptr, int width, int height, float av, float logAv, float power, float innerMult, float mult)
{
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	if(x < width && y < height)
	{
		int idx = 3 * (width * y + x);
		ptr += idx;
		float lum = 0.2125f * ptr[0] + 0.7154f * ptr[1] + 0.0721f * ptr[2];
		float map = (slog(lum + av) - logAv) / slog(2.0f + powf(lum, power) * innerMult) * mult / lum;
		ptr[0] *= map;
		ptr[1] *= map;
		ptr[2] *= map;
	}
}

extern "C" 
void logMapApply(float *ptr, int width, int height, float av, float logAv, float power, float innerMult, float mult)
{	
	int image_memory = width * height * 3 * sizeof(*ptr);

	float *gpuPtr = NULL;
	cudaMalloc((void**) &gpuPtr, image_memory);
		
	cudaMemcpy(gpuPtr, ptr, image_memory, cudaMemcpyHostToDevice);

	dim3 threads(16, 16);
	dim3 blocks((width + threads.x - 1) / threads.x, (height + threads.y - 1) / threads.y);

	logMapKernel<<<blocks, threads>>>(gpuPtr, width, height, av, logAv, power, innerMult, mult);

	cudaMemcpy(ptr, gpuPtr, image_memory, cudaMemcpyDeviceToHost);
	cudaFree(gpuPtr);
}
