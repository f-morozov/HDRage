#include "tonemap.h"
#include <cmath>
#include <vector>
#include <cstring>
#include <iostream>
using namespace std;

void HALEQ(HDRImage* lum, HDRImage* dest, int xmin, int xmax, int ymin, int ymax, int histSize, float betta) ;
HDRImage * histInitialMap(HDRImage *img, float key, int total);

void histogramMap(HDRImage *img, float key, float betta)
{
	int total = img->getWidth() * img->getHeight();

	HDRImage *lum = histInitialMap(img, key, total);
	HDRImage *mapped = new HDRImage(img->getWidth(), img->getHeight(), 1);

	HALEQ(lum, mapped, 0, img->getWidth() - 1, 0, img->getHeight() - 1, 10000, betta);

	float *imgPtr = img->getPtr();
	float *mappedPtr = mapped->getPtr();
	for(int pxl = 0; pxl < total; pxl++, imgPtr += 3, mappedPtr++) {

		float map = *mappedPtr / 255.0f / rgb2lum(imgPtr);
		for(int channel = 0; channel < 3; channel++)
			imgPtr[channel] *= map;
	}
	delete mapped;
	delete lum;
}

float histogramMapFunction(float lum, float min, float max, float t) {

	return (safeLog(lum + t) - safeLog(min + t)) / (safeLog(max + t) - safeLog(min + t));
}

int binarySearch(int val, int *arr, int left, int right) {

	if(val < arr[left])
		return left;

	if(val > arr[right])
		return right + 1;

	while(right > left) {

		int mid = (left + right) / 2;
		if(arr[mid] >= val)
			right = mid;
		else
			left = mid + 1;
	}
	return right;
}

void HALEQ(HDRImage* lum, HDRImage* dest, int xmin, int xmax, int ymin, int ymax, int histSize, float betta) 
{

	float binSize = 1.0f / histSize;
	int *hist = new int[histSize];
	memset(hist, 0, sizeof(int) * histSize);

	float *lumPtr = lum->getPtr();
	for(int y = ymin, yOffset = ymin * lum->getWidth(); y <= ymax; y++, yOffset += lum->getWidth()) {

		for(int x = xmin; x < xmax; x++) {

			int idx = lumPtr[yOffset + x] / binSize;
			if(idx > histSize - 1)
				idx = histSize - 1;
			if(idx < 0)
				idx = 0;
			hist[idx]++;
		}
	}

	for(int i = 1; i < histSize; i++)
		hist[i] += hist[i - 1];
	
	vector<int> delimeters(2);
	delimeters[0] = 0; delimeters[1] = histSize;
	for(int step = 0; step < 8; step++) {

		int delimSize = delimeters.size();
		for(int i = 0, curDelim = 0; i < delimSize - 1; i++, curDelim += 2) {

			int left = delimeters[curDelim];
			int right = delimeters[curDelim + 1] - 1;
			int valMid = (left + right + 1) / 2;

			int numMid = binarySearch(hist[left] + (hist[right] - hist[left]) / 2, hist, left, right);
			int mid = valMid + betta * (numMid - valMid);
			delimeters.insert(delimeters.begin() + curDelim + 1, mid);
		}
	}

	for(int delim = 0; delim < 256; delim++) {

		for(int i = delimeters[delim]; i < delimeters[delim + 1]; i++) {

			hist[i] = delim;
		}
	}

	lumPtr = lum->getPtr();
	float *destPtr = dest->getPtr();
	for(int y = ymin, yOffset = ymin * lum->getWidth(); y <= ymax; y++, yOffset += lum->getWidth()) {

		for(int x = xmin; x < xmax; x++) {

			int idx = lumPtr[yOffset + x] / binSize;
			if(idx > histSize - 1)
				idx = histSize - 1;
			if(idx < 0)
				idx = 0;

			destPtr[yOffset + x] = hist[idx];
		}
	}
	delete hist;
}

HDRImage * histInitialMap(HDRImage *img, float key, int total) {

	HDRImage *logLum;
	HDRImage *lum = getLum(img, &logLum);
	float *lumPtr = lum->getPtr();
	float min = 2e30f, max = -2e30f;
	float av = 0.0f;
	for(int i = 0; i < total; i++, lumPtr++) {

		if(*lumPtr > max)
			max = *lumPtr;
		if(*lumPtr < min)
			min = *lumPtr;
		av += safeLog(*lumPtr);
	}
	key = (safeLog(max) - av / total) / (safeLog(max) - safeLog(min));
	av = exp(av / total);
	float bias = 1.0f - key;

	lumPtr = lum->getPtr();
	float newMax = -2e30f, newMin = 2e30f;
	for(int i = 0; i < total; i++, lumPtr++) {

		*lumPtr = histogramMapFunction(*lumPtr, min, max, bias);
		if(*lumPtr > newMax)
			newMax = *lumPtr;
		if(*lumPtr < newMin)
			newMin = *lumPtr;
	}

	max = newMax; min = newMin;

	lumPtr = (float*)lum->getPtr();
	for(int i = 0; i < total; i++, lumPtr++) {
	
		if(max - min > 0.0f)
			*lumPtr = (*lumPtr - min) / (max - min);
	}
	delete logLum;
	return lum;
}

