#ifndef HDRIMAGE_H
#define HDRIMAGE_H

#include <QImage>

class HDRImage
{
	int width;
	int height;
	int chan;
	float *ptr;
public:
	HDRImage(int w, int h, int c);
	~HDRImage();
	int getWidth()
	{
		return width;
	}
	int getHeight()
	{
		return height;
	}
	int getChan()
	{
		return chan;
	}
	float *getPtr()
	{
		return ptr;
	}
	HDRImage *clone();
};

QImage hdrToQImage(HDRImage *hdrImg);
HDRImage *QImageToHDR(QImage img);


#endif