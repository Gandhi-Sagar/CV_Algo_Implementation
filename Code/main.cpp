#include "ImgProc.h"
#include <iostream>

int main()
{
	ImgProc proc;
	Image img = proc.imread_bitmap("red.bmp");
	if(! proc.imwrite_bitmap(img, "red_out.bmp"))
	{
		return -1;
	}

	for(unsigned i = 0; i < 300; ++i)
	{
		rgbTuple t = proc.at(0, i, img);
		std::cout << i << ": " << (int)std::get<0>(t) << ", " << (int)std::get<1>(t) << ", " << (int)std::get<2>(t) <<"\n";
	}
	Image gray_img = proc.rgb2gray(img);

	std::cout << "see.. everything is fine till here,\n";
	return 0;
}
