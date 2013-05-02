#ifndef HDR_MAKER_H
#define HDR_MAKER_H

#include <QImage>
#include <vector>
#include "tonemap.h"
#include "hdr_image.h"

using namespace std;

HDRImage *composeHDR(vector<QImage> images, vector<float> evalues, float gamma);

#endif