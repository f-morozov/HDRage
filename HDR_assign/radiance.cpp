#include "radiance.h"

#include <iostream>
#include <cstdio>
#include <cmath>
using namespace std;

#include <QDir>

void readArray(const char *path, float *arr)
{
	FILE *f = fopen(path, "r");
	int wavelen; float val;
	for(int i = 0; i < 81; i++)
		arr[i] = 0;
	if(!f)
		return;
	while(fscanf(f, "%d,%f", &wavelen, &val) == 2)
	{
		if(380 <= wavelen && wavelen <= 780) {

			int i = (wavelen - 380) / 5;
			arr[i] = val;
		}
	}
	fclose(f);
}

void readArray3(const char *path, float *arr)
{
	FILE *f = fopen(path, "r");
	int wavelen; float val[3];
	for(int i = 0; i < 81*3; i++)
		arr[i] = 0;
	if(!f)
		return;
	while(fscanf(f, "%d,%f,%f,%f", &wavelen, val, val + 1, val + 2) == 4)
	{
		if(380 <= wavelen && wavelen <= 780) {

			int i = (wavelen - 380) / 5;
			arr[3*i] = val[0];
			arr[3*i+1] = val[1];
			arr[3*i+2] = val[2];
		}
	}
	fclose(f);
}

HDRImage *calcRadiance(int size, float power, float real_size, float source_real_size, float tetha, QString &surface_path, QString &spectra_path)
{
	const int wavenum = 81;
	float source[wavenum], surface[wavenum], observer[wavenum*3];

	readArray(spectra_path.toStdString().c_str() , source);
	float ssum = 0;
	for(int i = 0; i < wavenum; i++)
		ssum += source[i];
	for(int i = 0; i < wavenum; i++)
		source[i] /= ssum;
	readArray(surface_path.toStdString().c_str(), surface);
	readArray3("Observer.csv", observer);
	float pi = 3.1415926f;
	int dsize = size * 2 - 1;
	HDRImage *img = new HDRImage(dsize, dsize, 3);
	float *imgPtr = img->getPtr();
	float k = real_size / size;

	int source_size_good = source_real_size / (2 * k) > 0? source_real_size / (2 * k) : 1;
	int source_size = source_size_good < 25 ? source_size_good : 25;
	float source_mult = source_size_good / source_size;

	float cost = cos(tetha / 180 * pi);
	float sint = sin(tetha / 180 * pi);
	float k2 = k * k;
	for(int y = 0; y < size; y++) {
		cout << y * 100 / size << '%' << endl;
		for(int x = -size + 1; x < size; x++) {
	
			float rad = 0;
			float xs = (-source_size + 1) * source_mult;
			float ys = (-source_size + 1) * source_mult;
			for(int xsr = -source_size + 1; xsr < source_size; xsr++, xs += source_mult) {
				for(int ysr = -source_size + 1; ysr < source_size; ysr++, ys += source_mult) {

					float xsturn = xs * cost;
					float zs = size + sint * xs;
					float R2 = (x - xsturn)*(x - xsturn) + (y - ys)*(y - ys) + zs * zs;
					float R = sqrt(R2);
					float cosa = zs / R;
					if(source_real_size == 0 || 
					   tetha >= 0 && tetha <= 90 && -size / sint * cost < x || 
					   tetha >= 270 && tetha <= 360 && -size / sint * cost > x)
						rad += power * cosa / (4 * pi * R2 * k2 * source_size * source_size);
				}
			}
			float xyz[3] = {0, 0, 0};
			for(int c = 0; c < 3; c++) {

				for(int i = 0; i < wavenum; i++) {

					float val = rad * (1 - surface[i]) * source[i];
					xyz[c] += val * observer[3*i + c];
				}
			}
			
			int xim = x + size - 1;
			int yim = y + size - 1;
			int coord = 3*(yim*dsize+xim);
			int yim2 = -y + size - 1;
			int coord2 = 3*(yim2*dsize+xim);
			imgPtr[coord2] = imgPtr[coord] = xyz[0] *  2.0413690 + xyz[1] * -0.5649464 + xyz[2] * -0.3446944;
			imgPtr[coord2+1] = imgPtr[coord+1] = xyz[0] * -0.9692660 + xyz[1] *  1.8760108 + xyz[1] *  0.0415560;
			imgPtr[coord2+2] = imgPtr[coord+2] = xyz[0] *  0.0134474 + xyz[1] * -0.1183897 + xyz[2] *  1.0154096;
		}
	}
	cout << "Mischief Managed" << endl;
	return img;
}
