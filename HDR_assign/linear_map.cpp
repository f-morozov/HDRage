#include "tonemap.h"

#include <iostream>
using namespace std;

extern "C" void scaleChannels(float *ptr, int width, int height, float min_val, float scale, float new_min);
extern "C" void applyGamma(float *ptr, int width, int height, float invGamma);

void linearMap(HDRImage *img, bool scalefull)
{
	float max_val = -1e30f;
	float min_val = 1e30f;
	float *ptr = img->getPtr();
	int total = img->getWidth() * img->getHeight();
	int total_values = total * 3;
	for (int i = 0; i < total_values; i++, ptr++) { 

		if (*ptr > max_val) {
			max_val = *ptr;
		}
		if (*ptr < min_val) {
			min_val = *ptr;
		}
	}

	if(min_val >= 0.0f && max_val <= 1.0f) {

		return;
	}

	float new_max = max_val < 1.0f ? max_val : 1.0f;
	float new_min = min_val > 0.0f ? min_val : 0.0f;
	new_min = max_val > 1.0f ? new_min / max_val : new_min;
	new_max = scalefull ? 1.0f : new_max;
	new_min = scalefull ? 0.0f : new_min;
	float scale = (new_max - new_min) / (max_val - min_val);

	scaleChannels(img->getPtr(), img->getWidth(), img->getHeight(), min_val, scale, new_min);
}

QImage hdrToQImage(HDRImage *hdrImg)
{
	float *ptr = hdrImg->getPtr();
	int total = hdrImg->getWidth() * hdrImg->getHeight();
	int total_values = total * 3;

	QImage qImg(hdrImg->getWidth(), hdrImg->getHeight(), QImage::Format_RGB32);
	uchar *qptr = qImg.bits();
	int width = hdrImg->getWidth();
	for(int y = 0; y < hdrImg->getHeight(); y++) {
		for(int x = 0; x < hdrImg->getWidth(); x++) {

			qptr[4*(y*width + x) + 3] = 255;
			qptr[4*(y*width + x) + 2] = ptr[3*(y*width + x)] * 255.0f;
			qptr[4*(y*width + x) + 1] = ptr[3*(y*width + x) + 1] * 255.0f;
			qptr[4*(y*width + x)]     = ptr[3*(y*width + x) + 2] * 255.0f;
			//qptr[3*(y*hdrImg->getWidth() + x) + 1] = 0;
			//qptr[3*(y*hdrImg->getWidth() + x) + 2] = 0;
			//qptr[3*(y*hdrImg->getWidth() + x)] = ptr[3*(y*hdrImg->getWidth() + x)] * 255;
			//qptr[3*(y*hdrImg->getWidth() + x) + 1] = ptr[3*(y*hdrImg->getWidth() + x) + 1] * 255;
			//qptr[3*(y*hdrImg->getWidth() + x) + 2] = ptr[3*(y*hdrImg->getWidth() + x) + 2] * 255;
		}
	}

	//for (int i = 0; i < total_values; i++, qptr++, ptr++) {
	//		
	//	*qptr = *ptr * 255.0f;
	//}
	return qImg;
}

void gammaCorrection(HDRImage *img, float gamma)
{
	applyGamma(img->getPtr(), img->getWidth(), img->getHeight(), 1.0f / gamma);
}

HDRImage *QImageToHDR(QImage img) {

    HDRImage *res = new HDRImage(img.width(), img.height(), 3);
    QImage cImg = img.convertToFormat(QImage::Format_RGB888);
    float *hdrPtr = res->getPtr();
    uchar *qImgPtr = cImg.bits();
	int total = img.width() * img.height() * 3;

    for (int i = 0; i < total; i++)
        *hdrPtr++ = *qImgPtr++ / 255.0;
        
     return res;
}