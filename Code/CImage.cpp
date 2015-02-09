#include "CImage.h"

CImage::CImage():	m_rows(0),
				m_cols(0),
				m_type(0),
				m_bFileRead(false)
{};

CImage::~CImage()
{
	if(m_bmpHeader)
		delete m_bmpHeader;
	if(m_bmpInfo)
		delete m_bmpInfo;
}

bool CImage::LoadBitmapImage(std::string filename)
{
	std::ifstream file(filename.c_str(), std::ios::binary);
	if(! file)
	{
		std::cout << "Unable to open file " << filename << std::endl;
		return false;
	}

	unsigned char *buf[2] = {0};
	buf[0] = new unsigned char[sizeof(BITMAPFILEHEADER)];
	buf[1] = new unsigned char[sizeof(BITMAPINFOHEADER)];

	if(! buf[0] || ! buf[1])
	{
		std::cout << "running outta memory " << std::endl;
		return false;
	}

	file.read((char*)buf[0], sizeof(BITMAPFILEHEADER));
	file.read((char*)buf[1], sizeof(BITMAPINFOHEADER));

	m_bmpHeader = (BITMAPFILEHEADER*) buf[0];
	m_bmpInfo = (BITMAPINFOHEADER*) buf[1];

	if(m_bmpHeader->b_filetype[0] != 'B' || m_bmpHeader->b_filetype[1] != 'M' ||
		m_bmpInfo-> compression != 0 || m_bmpInfo-> bits_per_pixel != 24)
	{
		std::cout << "Not a valid BMP file..Supporting 24 bit CImages only" ;
		return false;
	}

	// repacing char* by vec
	/*m_data = new unsigned char[m_bmpInfo->CImagesize];
	if(! m_data)
	{
		std::cout << "running outta memory " << std::endl;
		return false;
	}
	//file.read((char*)m_data, m_bmpInfo->CImagesize);
	*/
	std::vector<unsigned char> tempImageData;
	tempImageData.resize(m_bmpInfo->imagesize);
	// this is wrong, without taking care of padding, we will be reading file wrongly
	/*file.seekg(m_bmpHeader->dataoffset);
	file.read((char*)&tempImageData[0], m_bmpInfo->imagesize);*/
	
	int padding = 0;
	while (((m_bmpInfo->width*3+padding) % 4) != 0 )
		padding++;
	for(unsigned int i = 0 ; i < m_bmpInfo->height ; i++)
	{
		file.seekg(m_bmpHeader->dataoffset + i*(m_bmpInfo->width*3 + padding));
		file.read((char*)&tempImageData[i*m_bmpInfo->width*3], m_bmpInfo->width*3);
	}
	file.close();
	//bitmaps are stored as BGR -- lets convert to RGB
//	assert(m_bmpInfo->imagesize % 3 == 0);
	
	for (auto i = tempImageData.begin(); i != tempImageData.end(); i+=3)
	{
		m_data_red.push_back(*(i+2));
		m_data_green.push_back(*(i+1));
		m_data_blue.push_back(*(i+0));
	}

//	assert(m_data_red.size() == m_data_green.size() == m_data_blue.size() == (tempImageData.size()/3));

	m_rows = m_bmpInfo->width;
	m_cols = m_bmpInfo->height;
	m_type = IMG_DATA_U_CHAR;
	m_bFileRead = true;
	return true;
}

bool CImage::SaveBitmap(std::string filename)
{
	if (!m_bFileRead)
	{
		std::cout << "File was not read prior to writing, can't access header.. " << std::endl;
		return false;
	}
	std::ofstream file(filename.c_str(), std::ios::binary | std::ios::out);
	if (!file)
	{
		std::cout << "Could not create output file.. duhh" << std::endl;
		return false;
	}

	if (m_bmpHeader->b_filetype[0] != 'B' || m_bmpHeader->b_filetype[1] != 'M' ||
		m_bmpInfo->compression != 0 || m_bmpInfo->bits_per_pixel != 24)
	{
		std::cout << "Not a valid BMP file..Supporting 24 bit CImages only";
		return false;
	}

	// write BMP_HEADER
	file.write(reinterpret_cast<const char*>(m_bmpHeader), sizeof(BITMAPFILEHEADER));
	file.write(reinterpret_cast<const char*>(m_bmpInfo), sizeof(BITMAPINFOHEADER));

	// this is wrong.. format asks for bgr.. we are putting all r, all g, all b
	std::vector<unsigned char> img;
	img.reserve(m_data_red.size() + m_data_green.size() + m_data_blue.size());
	
	for(unsigned int i = 0 ; i < m_data_red.size() ; i++)
	{
		img.push_back(m_data_blue[i]);
		img.push_back(m_data_green[i]);
		img.push_back(m_data_red[i]);
	}

	char bmppad[3] = {0};

	for(unsigned int i = 0 ; i < m_bmpInfo->height ; i++)
	{
		// maybe something is wrong
		file.write(reinterpret_cast<const char*>(&img[i*m_bmpInfo->width*3]), m_bmpInfo->width * 3 * sizeof(unsigned char));
		file.write(bmppad, 1 * ((4-(m_bmpInfo->width*3)%4)%4) * sizeof(char));
	}

	file.close();
	return true;
}

int main()
{
	CImage img1;
	bool res = img1.LoadBitmapImage("test.bmp");
	if(res == false)
		std::cout << "we have a prob reading bmp";
	res = img1.SaveBitmap("test_out.bmp");
	if (res == false)
		std::cout << "we have a prob writing bmp";

	return 0;
}