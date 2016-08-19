#ifndef __ENCLOSE__
#define __ENCLOSE__

#include <tuple>
#include <vector>

#include "ImgProc.h"

typedef std::tuple<unsigned char, unsigned char, unsigned char> rgbTuple;
enum type { _undefined, _rgb, _gray };
class Image
{
	private: 
		int m_rows;
		int m_cols;
		
		type m_type;

		BITMAPFILEHEADER	*m_bmpHeader;	// store headers in case we need to write back to HDD
		BITMAPINFOHEADER	*m_bmpInfo;

	public:
		Image();
		~Image();
		
		class RGB
		{
			private: 
				std::vector <rgbTuple> data;
			friend class ImgProc;
		}rgb;

		class Gray
		{
			private:
				std::vector<unsigned char> data;
			friend class ImgProc;
		}gray;
		friend class ImgProc;
};
#endif
