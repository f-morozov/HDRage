#include "hdr_maker.h"
#include "hdr_image.h"
#include "tonemap.h"
#include <cmath>
#include <iostream>
#include <QImage>
#include <vector>

using namespace std;

HDRImage *composeHDR(vector<QImage> images, vector<float> evalues, float gamma)
{
	int width = images[0].width();
	int height = images[0].height();
	int total = width * height;
	int imgnum = images.size();

	float weight[256];
	for (int i = 0; i < 128; i++)
		weight[i] = (i+1) / 255.0f;
	for (int i = 128; i < 256; i++)
		weight[i] = (255 - i + 1) / 255.0f;

	float responce[256];
	for (int i = 0; i < 256; i++) {

		responce[i] = pow(i / 255.0f, gamma);
	}

	HDRImage *result = new HDRImage(width, height, 3);
	float *resPtr = result->getPtr();

	for(int pxl = 0; pxl < total; pxl++, resPtr += 3) {
		
		float sum[3] = {0, 0, 0};
		float wsum = 0;
		for(int im = 0; im < images.size(); im++) {

			uchar imgPtr[3];
			imgPtr[0] = images[im].bits()[4 * pxl + 2];
			imgPtr[1] = images[im].bits()[4 * pxl + 1];
			imgPtr[2] = images[im].bits()[4 * pxl];
			float w = (weight[imgPtr[0]] + weight[imgPtr[1]] + weight[imgPtr[2]]) / 3;
			for(int channel = 0; channel < 3; channel++) {

				sum[channel] += w * responce[imgPtr[channel]] / evalues[im];
				wsum += w;
			}
		}
		for(int channel = 0; channel < 3; channel++) {

			if(wsum > 0.0f)
				resPtr[channel] = sum[channel] / wsum;
			else
				resPtr[channel] = 1.0f;
		}
	}
	return result;
}
