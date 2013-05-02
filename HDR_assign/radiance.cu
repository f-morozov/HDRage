_global__ void radKernel(float *imgPtr, int size, float source_size, float source_mult, float tetha, float cost, float sint, float source_real_size, float power,
                         float *source, float *surface, float *observer)
{
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	if(x < width && y < height)
	{
		x += -size + 1;

	}
}

extern "C" 
void doRadiance(float *imgPtr, int size, float source_size, float source_mult, float tetha, float cost, float sint, float source_real_size, float power,
                float *source, float *surface, float *observer)
{
	int wavenum = 81;
	float *gpuPtr = NULL;
	float *gpuSource = NULL;
	float *gpuSurface = NULL;
	float *gpuObserver = NULL;
	cudaMalloc((void**) &gpuPtr, size * size * sizeof(float) * 3);
	cudaMalloc((void**) &gpuSource, wavenum * sizeof(float));
	cudaMalloc((void**) &gpuSurface, wavenum sizeof(float));
	cudaMalloc((void**) &gpuObserver, wavenum * sizeof(float) * 3);

	cudaMemcpy(gpuPtr, imgPtr, size * size * sizeof(float) * 3, cudaMemcpyHostToDevice);

	dim3 threads(16, 16);
	dim3 blocks((size + threads.x - 1) / threads.x, (size + threads.y - 1) / threads.y);

	radKernel<<<blocks, threads>>>(gpuPtr, size, source_size, source_mult, tetha, cost, sint, source_real_size, power,
                                   gpuSource, gpuSurface, gpuObserver);

	cudaMemcpy(ptr, gpuPtr, size * size * sizeof(float) * 3, cudaMemcpyDeviceToHost);
	cudaFree(gpuPtr);
	cudaFree(gpuSource);
	cudaFree(gpuSurface);
	cudaFree(gpuObserver);
}
