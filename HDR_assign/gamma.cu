__global__ void gammaKernel(float *ptr, int width, int height, float invGamma)
{
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	if(x < width && y < height)
	{
		int idx = 3 * (width * y + x);
		ptr[idx] = powf(ptr[idx], invGamma);
		ptr[idx + 1] = powf(ptr[idx + 1], invGamma);
		ptr[idx + 2] = powf(ptr[idx + 2], invGamma);
	}
}

extern "C" 
void applyGamma(float *ptr, int width, int height, float invGamma)
{
	int image_memory = width * height * 3 * sizeof(*ptr);

	float *gpuPtr = NULL;
	cudaMalloc((void**) &gpuPtr, image_memory);
		
	cudaMemcpy(gpuPtr, ptr, image_memory, cudaMemcpyHostToDevice);

	dim3 threads(16, 16);
	dim3 blocks((width + threads.x - 1) / threads.x, (height + threads.y - 1) / threads.y);

	gammaKernel<<<blocks, threads>>>(gpuPtr, width, height, invGamma);

	cudaMemcpy(ptr, gpuPtr, image_memory, cudaMemcpyDeviceToHost);
	cudaFree(gpuPtr);
}