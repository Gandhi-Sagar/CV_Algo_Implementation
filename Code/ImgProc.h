#ifndef __IMG_PROC__
#define __IMG_PROC__

#include <fstream>
#include <string>
#include <iostream>

#include "BMPHeader.h"

class Image;
typedef std::tuple<unsigned char, unsigned char, unsigned char> rgbTuple;

class ImgProc
{
	public:
		ImgProc() {};
		~ImgProc() {};

		Image		imread_bitmap(std::string filename);
		bool		imwrite_bitmap(Image& img, std::string filename);
		Image		rgb2gray(const Image& img);
		rgbTuple	at(int row, int col, Image img);
};


#include "Image.h"

#endif
