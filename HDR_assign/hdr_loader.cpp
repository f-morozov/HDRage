#include "hdr_loader.h"
#include <cmath>
#include <cstring>
#include <QFile>
#include <QByteArray>

typedef unsigned char uchar;

void rgbeToFloat (uchar *rgbe, float *f)
{
	if (rgbe[3]) {

		float mult = pow (2.0f, (float) ((int) rgbe [3] - 128 - 8));
		f [0] = mult * rgbe [0];
		f [1] = mult * rgbe [1];
		f [2] = mult * rgbe [2];
	}
	else {

		f[0] = f[1] = f[2] = 0.0f;
	}
}

void floatToRgbe (float *f, uchar *rgbe)
{
	float max;
	if (f[0] > f[1]) {
		if (f[2] > f[0])
			max = f[2];
		else
			max = f[0];
	} else {
		if (f[2] > f[1])
			max = f[2];
		else
			max = f[1];
	}	
	if (max < 1e-32) {
		memset (rgbe, 0, 4);
	}
	else {

		int e;
		float significand = frexp (max, &e);
		float mult = 256.0f / pow (2.0f, (float) e);
		for(int i = 0; i < 3; i++)
		{
			rgbe[i] = (uchar) (f[i] * mult);
		}
		rgbe[3] = e + 128;
	}
}

HDRImage *loadHdrFile(QString path) {
	
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly)) {
		return NULL;
	}

	const int maxLen = 256;
	QByteArray str = file.readLine(maxLen);

	while (str.length () > 1) {

		str = file.readLine(maxLen);
	}

	str = file.readLine(maxLen);
	str[str.size() - 1] = 0; // that was \n
	QList <QByteArray> tokens = str.split(' ');
	bool size_loaded = false;
	int height = 0, width = 0;

	if (tokens.size () == 4) {

		height = tokens [1].toInt(&size_loaded);
		if (size_loaded) {
			width = tokens [3].toInt(&size_loaded);
		}
	}

	if (!size_loaded) {

		file.close();
		return NULL;
	}
	
	QByteArray pixelArray = file.readAll();
	uchar *pixelData = (uchar *)pixelArray.data();
	QByteArray scanlines [4];

	int curByte = 0;
	file.close();
	HDRImage *img = new HDRImage(width, height, 3);
	float *imgPtr = img->getPtr();

	if (pixelData[curByte] != 2 || pixelData[curByte + 1] != 2) { // Not RLE

		int size = width * height;
		for (int i = 0; i < size; i++) {

			rgbeToFloat (pixelData, imgPtr);
			pixelData += 4;
			imgPtr += 3;
		}

	} else { // RLE

		for (int y = 0; y < height; y++) {

			if (pixelData[curByte] != 2 || pixelData[curByte + 1] != 2 || width != ((int)pixelData[curByte + 2] << 8) + pixelData[curByte + 3]) {
		
				delete img;
				return NULL;
			}
			curByte += 4;

			for (int channel = 0; channel < 4; channel++) {

				int fillIndex = 0;
				while (fillIndex < width) {

					if (pixelData[curByte] <= 128) { // Not a run
						int copyLength = pixelData[curByte++];
						for (int i = 0; i < copyLength; i++)
							scanlines[channel][fillIndex++] = pixelData[curByte++];

					} else { // Run

						int runLength = pixelData[curByte++] - 128;
						char runByte = pixelData [curByte++];
						for (int i = 0; i < runLength; i++)
							scanlines[channel][fillIndex++] = runByte;
					}
				}
				if (fillIndex != width) {
			
					delete img;
					return false;
				}
			}	
			uchar rgbe[4];
			for (int x = 0; x < width; x++) {

				for (int channel = 0; channel < 4; channel++)
					rgbe[channel] = scanlines[channel][x];

				rgbeToFloat(rgbe, imgPtr);
				imgPtr += 3;
			}
		}
	}
	return img;
}

bool saveHdrFile (QString path, HDRImage *img) {

	QFile file(path);
	if (!file.open(QIODevice::WriteOnly)) {
		return false;
	}

	QByteArray header("#?RGBE\n");
	header += "FORMAT=32-bit_rle_rgbe\n";
	header += "SOFTWARE=HDRage\n\n";
	QByteArray ySizeStr, xSizeStr;
	ySizeStr.setNum(img->getHeight());
	xSizeStr.setNum(img->getWidth());
	header += "-Y " + ySizeStr + " +X " + xSizeStr + "\n";
	file.write (header);

	QByteArray rleInfo;
	rleInfo [0] = rleInfo [1] = 2;
	rleInfo [2] = (img->getWidth() >> 8) & 0xFF;
	rleInfo [3] = img->getWidth() & 0xFF;

	QByteArray scanlines [4];
	for (int i = 0; i < 4; i++)
	{
		scanlines [i].resize (img->getWidth());
	}

	float *floatPtr = img->getPtr();
	QByteArray writeData;

	for (int y = 0; y < img->getHeight(); y++) {

		for (int x = 0; x < img->getWidth(); x++) {

			uchar rgbe[4];
			floatToRgbe(floatPtr, rgbe);
			floatPtr += 3;
			for (int channel = 0; channel < 4; channel++)
			{
				scanlines[channel][x] = rgbe[channel];
			}
		}
		file.write(rleInfo);
		for (int channel = 0; channel < 4; channel++) {

			writeData.clear();
			QByteArray nonRunBuffer;
			for (int x = 0; x < img->getWidth(); ) {

				int runEnd = x;
				while (runEnd < img->getWidth() && runEnd - x < 127 && scanlines[channel][runEnd] == scanlines[channel][x])
				{
					runEnd++;
				}
				int runLength = runEnd - x;
				if (runLength >= 4) {

					if (nonRunBuffer.size() != 0) {

						writeData += (uchar)nonRunBuffer.size();
						writeData += nonRunBuffer;
						nonRunBuffer.clear();
					}
					writeData += (uchar) runLength + 128;
					writeData += scanlines[channel][x];
				}
				else {
					for (int i = 0; i < runLength; i++) {

						nonRunBuffer += scanlines[channel][x];
						if (nonRunBuffer.size() == 128) {
								
							writeData += (uchar)nonRunBuffer.size();
							writeData += nonRunBuffer;
							nonRunBuffer.clear();
						}
					}
				}
				x = runEnd;
			}

			if (nonRunBuffer.size() != 0) {

				writeData += (uchar)nonRunBuffer.size();
				writeData += nonRunBuffer;
				nonRunBuffer.clear();
			}
			file.write(writeData);
		}
	}
	file.close();
	return true;
}
