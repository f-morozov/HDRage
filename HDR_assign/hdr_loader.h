#ifndef HDRLOADER_H
#define HDRLOADER_H

#include "hdr_image.h"
#include <QString>

HDRImage *loadHdrFile(QString path);
bool saveHdrFile (QString path, HDRImage *img);

#endif