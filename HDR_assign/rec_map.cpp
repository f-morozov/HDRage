#include "tonemap.h"
#include <cmath>

#include <iostream>
using namespace std;

extern "C" void recMapApply(float *ptr, int width, int height, float contrast, float intensity, float colorc, float lighta, float *adaptGlobal);

void recMap(HDRImage *img, float contrast, float intensity, float colorc, float lighta)
{
	float maxLum = 0.0f;
	float minLum = 1e30f;
	float av[3] = {0.0f, 0.0f, 0.0f};
	float avLum = 0.0f;
	float logAvLum = 0.0f;

	HDRImage *logLumImg;
	HDRImage *lumImg = getLum(img, &logLumImg);
	float *lum = lumImg->getPtr();
	float *imgPtr = img->getPtr();
	float *logPtr = logLumImg->getPtr();
	int total = img->getWidth() * img->getHeight();
	for(int i = 0; i < total; i++, imgPtr += 3, lum++, logPtr++) {

		logAvLum += *logPtr;
		avLum += *lum;

		if(*lum > maxLum)
			maxLum = *lum;
		if(*lum < minLum)
			minLum = *lum;

		for(int channel = 0; channel < 3; channel++) {

			av[channel] += imgPtr[channel];
		}
	}
	
	avLum /= total;
	logAvLum /= total;
	for(int channel = 0; channel < 3; channel++) {

		av[channel] /= total;
	}

	intensity = exp(-intensity);
	if(contrast <= 0.0f) {

		float k = (safeLog(maxLum) - logAvLum) / (safeLog(maxLum) - safeLog(minLum));
		contrast = 0.3f + 0.7f * pow(k, 1.4f);
	}

	float adaptGlobal[3];
	for(int channel = 0; channel < 3; channel++) {
			
		adaptGlobal[channel] = colorc * av[channel] + (1.0f - colorc) * avLum;
	}
	recMapApply(img->getPtr(), img->getWidth(), img->getHeight(), contrast, intensity, colorc, lighta, adaptGlobal);
	delete lumImg;
	delete logLumImg;
}