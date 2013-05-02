#include "tonemap.h"

#include <iostream>
using namespace std;

extern "C" void makeLum(float *ptr, float *lum, int width, int height, float *logged);

HDRImage *getLum(HDRImage *im, HDRImage **logged)
{
	float *log_ptr = 0;
	if(logged) {
	
		*logged = new HDRImage(im->getWidth(), im->getHeight(), 1);
		log_ptr = (*logged)->getPtr();
	}
	HDRImage *res = new HDRImage(im->getWidth(), im->getHeight(), 1);
	makeLum(im->getPtr(), res->getPtr(), im->getWidth(), im->getHeight(), log_ptr);
	return res;
}