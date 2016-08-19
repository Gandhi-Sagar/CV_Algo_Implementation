#ifndef __IMG_PROC__
#define __IMG_PROC__

#include <fstream>
#include <string>
#include <iostream>

#include "BMPHeader.h"

class Image;

class ImgProc
{
	public:
		ImgProc() {};
		~ImgProc() {};

		Image ReadBitmap(std::string filename);
		bool WriteBitmap(Image& img, std::string filename);
};


#include "Image.h"

#endif
