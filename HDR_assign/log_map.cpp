#include "tonemap.h"
#include <cmath>

#include <iostream>
using namespace std;

extern "C" void logMapApply(float *ptr, int width, int height, float av, float logAv, float power, float innerMult, float mult);

void logMap(HDRImage *img, float bias)
{
	HDRImage *logLum = NULL;
	HDRImage *lum = getLum(img, &logLum);
	int total = img->getWidth() * img->getHeight();
	float maxLum = 0.0f, logSum = 0.0f;
	float *ptr = img->getPtr();
	float *lumPtr = lum->getPtr();
	float *logPtr = logLum->getPtr();
	for(int i = 0; i < total; i++, ptr += 3, lumPtr++, logPtr++)
	{
		if(*lumPtr > maxLum) {
			maxLum = *lumPtr;
		}
		logSum += *logPtr;
	}

	float logAv = logSum / total;
	float av = exp(logAv);
	float mult = 1.0f / log10(maxLum / av + 1.0f);
	float power = log(bias) / log(0.5f);
	float innerMult = pow(maxLum, -power) * 8.0f;
	logMapApply(img->getPtr(), img->getWidth(), img->getHeight(), av, logAv, power, innerMult, mult);
	delete logLum;
	delete lum;
}