#include "ImgProc.h"
#include <iostream>

int main()
{
	ImgProc proc;
	Image img = proc.ReadBitmap("cat.bmp");
	if(! proc.WriteBitmap(img, "cat_out.bmp"))
	{
		return -1;
	}
	return 0;
}
