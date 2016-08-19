#include "Image.h"

Image::Image():	m_rows(0),
		m_cols(0),
		m_bmpHeader(NULL),
		m_bmpInfo(NULL),
		m_type(_undefined)
{

}

Image::~Image()
{
	if(m_bmpHeader)
		delete m_bmpHeader;
	if(m_bmpInfo)
		delete m_bmpInfo;
}

