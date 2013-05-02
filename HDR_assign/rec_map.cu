#include <iostream>
using namespace std;

__global__ void recMapKernel(float *ptr, int width, int height, float contrast, float intensity, float colorc, float lighta, float *adaptGlobal)
{
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	if(x < width && y < height)
	{
		int idx = 3 * (width * y + x);
		ptr += idx;
		float lum = 0.2125f * ptr[0] + 0.7154f * ptr[1] + 0.0721f * ptr[2];
		for(int channel = 0; channel < 3; channel++) {

			float adaptLocal = colorc * ptr[channel] + (1.0f - colorc) * lum;
			float adapt = lighta * adaptLocal + (1.0f - lighta) * adaptGlobal[channel];
			ptr[channel] = ptr[channel] / (ptr[channel] + powf(intensity * adapt, contrast));
		}
	}
}

extern "C" 
void recMapApply(float *ptr, int width, int height, float contrast, float intensity, float colorc, float lighta, float *adaptGlobal)
{	
	int image_memory = width * height * 3 * sizeof(*ptr);

	float *gpuPtr = NULL;
	cudaMalloc((void**) &gpuPtr, image_memory);
	float *adaptGpu = NULL;
	cudaMalloc((void**) &adaptGpu, 3*sizeof(float));
	cudaMemcpy(gpuPtr, ptr, image_memory, cudaMemcpyHostToDevice);
	cudaMemcpy(adaptGpu, adaptGlobal, 3*sizeof(float), cudaMemcpyHostToDevice);
	dim3 threads(16, 16);
	dim3 blocks((width + threads.x - 1) / threads.x, (height + threads.y - 1) / threads.y);

	recMapKernel<<<blocks, threads>>>(gpuPtr, width, height, contrast, intensity, colorc, lighta, adaptGpu);

	cudaMemcpy(ptr, gpuPtr, image_memory, cudaMemcpyDeviceToHost);
	cudaFree(gpuPtr);
}
