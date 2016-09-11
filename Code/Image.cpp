#include "Image.h"

Image::Image():	m_rows(0),
		m_cols(0),
		m_bmpHeader(NULL),
		m_bmpInfo(NULL),
		m_type(_undefined)
{

}

// copy constructor
Image::Image(const Image& img)
{
	m_bmpHeader = (BITMAPFILEHEADER*) new unsigned char[sizeof(BITMAPFILEHEADER)]; 
	*m_bmpHeader = *img.m_bmpHeader;

	m_bmpInfo = (BITMAPINFOHEADER*)new unsigned char[sizeof(BITMAPINFOHEADER)];
	*m_bmpInfo = *img.m_bmpInfo;

	m_rows = img.m_rows;
	m_cols = img.m_cols;
	m_type = img.m_type;
	rgb = img.rgb;
	gray = img.gray;
}


Image::~Image()
{
	if(m_bmpHeader)
		delete m_bmpHeader;
	if(m_bmpInfo)
		delete m_bmpInfo;
}

