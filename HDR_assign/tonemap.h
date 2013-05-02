#ifndef TONEMAP_H
#define TONEMAP_H

#include "hdr_image.h"
#include <QImage>
#include <cmath>

void gammaCorrection(HDRImage *img, float gamma);
void linearMap(HDRImage *img, bool scalefull);
void logMap(HDRImage *img, float bias);
void bilateralMap(HDRImage *img, float contrast, float lumSigma, float spaceSigma);
void recMap(HDRImage *img, float contrast, float intensity, float colorc, float lighta);
void histogramMap(HDRImage *img, float key, float betta);
void blurMap(HDRImage *img, float k, float sigma, float correction);
HDRImage *getLum(HDRImage *im, HDRImage **logged);
void blurImage(float *img, float *res, float sigma, int width, int height);
HDRImage *makeGaussian(int width, int height, float sigma);
extern "C"
float *makeGaussianGpu(int width, int height, float sigma);

inline float rgb2lum(float *ptr)
{
	return 0.2125f * ptr[0] + 0.7154f * ptr[1] + 0.0721f * ptr[2];
}

inline float safeLog(float x) {

	if(x < 4.6e-5f)
		return -10.0f;
	return log(x);
}

#endif