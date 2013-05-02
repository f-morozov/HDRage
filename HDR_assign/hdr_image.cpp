#include "hdr_image.h"
#include <cstring>

HDRImage::HDRImage(int w, int h, int c = 3) : width(w), height(h), chan(c)
{
	ptr = new float[w * h * c];
}

HDRImage::~HDRImage()
{
	delete []ptr;
}
HDRImage *HDRImage::clone()
{
	HDRImage *res = new HDRImage(width, height, chan);
	memcpy(res->ptr, ptr, width * height * chan * sizeof(*ptr));
	return res;
}