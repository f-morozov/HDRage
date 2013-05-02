#ifndef RADIANCE_H
#define RADIANCE_H

#include "tonemap.h"


HDRImage *calcRadiance(int size, float power, float real_size, float source_size, float tetha, 
					   QString &surface_path, QString &spectra_path);

#endif