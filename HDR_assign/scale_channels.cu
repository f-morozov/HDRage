#include <iostream>

using namespace std;

__global__ void channelScaleKernel(float *ptr, int width, int height, float min_val, float scale, float new_min)
{
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	if(x < width && y < height)
	{
		int idx = 3 * (width * y + x);
		ptr[idx] = new_min + (ptr[idx] - min_val) * scale;
		ptr[idx + 1] = new_min + (ptr[idx + 1] - min_val) * scale;
		ptr[idx + 2] = new_min + (ptr[idx + 2] - min_val) * scale;
	}
}

extern "C" 
void scaleChannels(float *ptr, int width, int height, float min_val, float scale, float new_min)
{	
	int image_memory = width * height * 3 * sizeof(*ptr);

	float *gpuPtr = NULL;
	cudaMalloc((void**) &gpuPtr, image_memory);
		
	cudaMemcpy(gpuPtr, ptr, image_memory, cudaMemcpyHostToDevice);

	dim3 threads(16, 16);
	dim3 blocks((width + threads.x - 1) / threads.x, (height + threads.y - 1) / threads.y);

	channelScaleKernel<<<blocks, threads>>>(gpuPtr, width, height, min_val, scale, new_min);

	cudaMemcpy(ptr, gpuPtr, image_memory, cudaMemcpyDeviceToHost);
	cudaFree(gpuPtr);
}
