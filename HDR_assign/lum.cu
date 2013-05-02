__device__ float safelog(float x) {

	if(x < 4.6e-5f)
		return -10.0f;
	return logf(x);
}

__global__ void lumKernel(float *ptr, float *lum, int width, int height, float *logged)
{
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	if(x < width && y < height)
	{
		int idx = width * y + x;
		ptr += 3 * idx;
		lum += idx;
		logged += idx;

		*lum = 0.2125f * ptr[0] + 0.7154f * ptr[1] + 0.0721f * ptr[2];
		*logged = safelog(*lum);
	}
}

extern "C" 
void makeLum(float *ptr, float *lum, int width, int height, float *logged)
{
	int lum_memory = width * height * sizeof(*ptr);
	int image_memory = lum_memory * 3;

	float *gpuPtr = NULL;
	float *lumPtr = NULL;
	float *logLumPtr = NULL;
	cudaMalloc((void**) &gpuPtr, image_memory);
	cudaMalloc((void**) &lumPtr, lum_memory);
	cudaMalloc((void**) &logLumPtr, lum_memory);
		
	cudaMemcpy(gpuPtr, ptr, image_memory, cudaMemcpyHostToDevice);

	dim3 threads(16, 16);
	dim3 blocks((width + threads.x - 1) / threads.x, (height + threads.y - 1) / threads.y);
	lumKernel<<<blocks, threads>>>(gpuPtr, lumPtr, width, height, logLumPtr);

	cudaMemcpy(lum, lumPtr, lum_memory, cudaMemcpyDeviceToHost);
	if(logged)
		cudaMemcpy(logged, logLumPtr, lum_memory, cudaMemcpyDeviceToHost);
	
	cudaFree(gpuPtr);
	cudaFree(lumPtr);
	cudaFree(logLumPtr);
}
