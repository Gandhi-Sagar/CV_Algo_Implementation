#ifndef _IMAGE_CLASS
#define _IMAGE_CLASS

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <assert.h>

#pragma pack(push, 1)

typedef struct 
{
  unsigned char 	b_filetype[2];
  unsigned int 		b_filesize;
  unsigned short 	reserved1;
  unsigned short	reserved2;
  unsigned int 		dataoffset;              //No of bytes before actual pixel data
}BITMAPFILEHEADER;

typedef struct 
{
   unsigned int			headersize;
   unsigned int			width;
   unsigned int			height;
   unsigned short		planes;
   unsigned short		bits_per_pixel;         //code is written for 24 bits only and No other format is supported..
   unsigned int			compression;
   unsigned int			imagesize;              
   unsigned int			xresolution;
   unsigned int			yresolution;
   unsigned int			ncolors;
   unsigned int			impcolors;
}BITMAPINFOHEADER;

#pragma pack(pop)

/*	It is expected that you read before you write.
 *	If you make any changes to "image data" i.e. R, G, B channels, MAKE SURE you change the header accordingly
 *	
 *	e.g. If you convert the image to grayscale, change the following entries in the header:
 *	BMP_INFO_HEADER - planes, bits_per_pixel, imagesize
 *  BMP_FILE_HEADER - b_filesize (this = imagesize + headersize)
 */

class CImage
{
public:
			CImage();
			~CImage();

			bool	LoadBitmapImage(std::string filename);
			bool	SaveBitmap(std::string filename);

private:
			int								m_rows;
			int								m_cols;
			
			std::vector<unsigned char>		m_data_red;
			std::vector<unsigned char>		m_data_green;
			std::vector<unsigned char>		m_data_blue;

			bool							m_bFileRead;

			BITMAPFILEHEADER				*m_bmpHeader;		// store headers in case we need to write back to HDD
			BITMAPINFOHEADER				*m_bmpInfo;
};


#endif