
__global__ void gaussKernel(float *ptr, int width, int height, int sigma2)
{
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	if(x < width && y < height)
	{
		int xc = width / 2;
		int yc = height / 2;
		int idx = (width * y + x);
		ptr += idx;
		float pi = 3.1415926f;
		*ptr = expf(-((x - xc)*(x - xc)+(y - yc)*(y - yc)) / (2*sigma2)) / (2 * pi * sigma2);
	}
}

extern "C"
float *makeGaussianGpu(int width, int height, float sigma)
{
	float *ptr = NULL;
	cudaMalloc((void**) &ptr, width * height * sizeof(float));

	float sigma2 = sigma * sigma;
	
	dim3 threads(16, 16);
	dim3 blocks((width + threads.x - 1) / threads.x, (height + threads.y - 1) / threads.y);

	gaussKernel<<<blocks, threads>>>(ptr, width, height, sigma2);

	return ptr;
}
