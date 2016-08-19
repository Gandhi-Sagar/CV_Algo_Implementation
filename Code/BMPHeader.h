#ifndef __BMPHEADER__
#define __BMPHEADER__

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

#endif
