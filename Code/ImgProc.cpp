#include "Image.h"

/*
* About Header: offset is always equal to header size, unless otherwise specified in the input image.
* Note that different packages set it differently. For e.g. matlab sets it to 1024 + header_size
* Also, notable fact is header_size = 54; unlike mentioned in the 'headersize' parameter of header,
*  as it considers only INFO_HEADER.
*/
Image ImgProc::imread_bitmap(std::string filename)
{
	Image img;
	
	// open file in binary mode
	std::ifstream file(filename, std::ios::binary);
	if(! file)
	{
		std::cout << "Unable to open file " << filename << std::endl;
		return img;
	}

	// allocate memory for headers, 
	// don't worry about delete, it is being taken care by Image class
	unsigned char *buf[2] = {0};
	buf[0] = new unsigned char[sizeof(BITMAPFILEHEADER)]; 
	buf[1] = new unsigned char[sizeof(BITMAPINFOHEADER)];
	if(! buf[0] || ! buf[1])
	{
		std::cout << "running outta memory " << std::endl;
		return img;
	}

	// read headers and make sure it is "supported" bmp
	file.read((char*)buf[0], sizeof(BITMAPFILEHEADER));
	file.read((char*)buf[1], sizeof(BITMAPINFOHEADER));

	img.m_bmpHeader = (BITMAPFILEHEADER*) buf[0];
	img.m_bmpInfo 	= (BITMAPINFOHEADER*) buf[1];

	if(img.m_bmpHeader->b_filetype[0] != 'B' || img.m_bmpHeader->b_filetype[1] != 'M' ||
		img.m_bmpInfo-> compression != 0 || img.m_bmpInfo-> bits_per_pixel != 24)
	{
		std::cout << "Not a valid BMP file..Supporting 24 bit Images only" ;
		return img;
	}

	//read image data in local variable
	std::vector<unsigned char> tempImageData;
	tempImageData.resize(img.m_bmpInfo->imagesize);
	int padding = 0;
	while (((img.m_bmpInfo->width*3+padding) % 4) != 0 )
		padding++;
	for(unsigned int i = 0 ; i < img.m_bmpInfo->height ; i++)
	{
		file.seekg(img.m_bmpHeader->dataoffset + i*(img.m_bmpInfo->width*3 + padding));
		file.read((char*)&tempImageData[i*img.m_bmpInfo->width*3], img.m_bmpInfo->width*3);
	}
	file.close();

	//bitmaps are stored as BGR -- lets convert to RGB while reading 
	for (auto i = tempImageData.begin(); i != tempImageData.end(); i+=3)
	{
		img.rgb.data.push_back(std::make_tuple(*(i+2), *(i+1), *(i+0)));
	}
	img.m_type = _rgb;
	img.m_rows = img.m_bmpInfo->height;
	img.m_cols = img.m_bmpInfo->width;

	return img;
}

/*
* TODO: add facility to have offset different than HEADER_SIZE
* in order to incorporate this, there might be need of dummy data
*/
bool ImgProc::imwrite_bitmap(Image& img, std::string filename)
{
	if (img.m_type == _undefined)
	{
		std::cout << "Not a proper BMP for writing, type is undefined" << std::endl;
		return false;
	}

	std::ofstream file(filename.c_str(), std::ios::binary | std::ios::out);
	if (!file)
	{
		std::cout << "Could not create output file.. duhh" << std::endl;
		return false;
	}

	if (img.m_bmpHeader->b_filetype[0] != 'B' || img.m_bmpHeader->b_filetype[1] != 'M' ||
		img.m_bmpInfo->compression != 0 )
	{
		std::cout << "Not a valid BMP file.";
		return false;
	}

	// write BMP_HEADER
	file.write(reinterpret_cast<const char*>(img.m_bmpHeader), sizeof(BITMAPFILEHEADER));
	file.write(reinterpret_cast<const char*>(img.m_bmpInfo), sizeof(BITMAPINFOHEADER));

	// copy data to temporary vector, so that padding becomes easy
	std::vector<unsigned char> out_img;
	out_img.reserve(img.rgb.data.size() * sizeof(img.rgb.data[0]));
	
	for(unsigned int i = 0 ; i < img.rgb.data.size() ; ++i)
	{
		out_img.push_back( std::get<2> (img.rgb.data[i]) );//red
		out_img.push_back( std::get<1> (img.rgb.data[i]) );//green
		out_img.push_back( std::get<0> (img.rgb.data[i]) );//blue
	}

	// write while padding - a different way of padding
	char bmppad[3] = {0};
	for(unsigned int i = 0 ; i < img.m_bmpInfo->height ; i++)
	{
		file.write(reinterpret_cast<const char*> (&out_img[i * img.m_bmpInfo->width*3]), img.m_bmpInfo->width * 3 * sizeof(unsigned char));
		file.write(bmppad, 1 * ((4-(img.m_bmpInfo->width*3)%4)%4) * sizeof(char));
	}
	file.close();

	return true;
}

Image ImgProc::rgb2gray(const Image& img)
{
	// sanity check
	if(img.m_bmpHeader->b_filetype[0] != 'B' || img.m_bmpHeader->b_filetype[1] != 'M' ||
		img.m_bmpInfo-> compression != 0 || img.m_bmpInfo-> bits_per_pixel != 24)
	{
		std::cout << "rgb2gray: Not a valid color BMP file..Supporting 24 bit uncompressed Images only" ;
		return img;
	}

	// convert pixels
	std::vector<unsigned char> grays(img.m_cols*img.m_rows);

	for(unsigned i = 0; i < img.rgb.data.size(); ++i)
	{
		// use whatever logic
		grays.push_back((std::get<0> (img.rgb.data[i]) + std::get<1> (img.rgb.data[i]) + std::get<2> (img.rgb.data[i]))/3);
	}
	Image gray_img = img;
	unsigned col_size = gray_img.m_cols;
	for(int i = 0; i < gray_img.m_rows; ++i)
	{
		std::vector<unsigned char> temp(grays.begin()+ i*col_size, grays.begin() + ((i+1)*col_size));
		gray_img.gray.data.push_back(temp);
	}

	// change image type
	gray_img.m_type = _gray;
	
	// delete rgb part
	gray_img.rgb.data.clear();

	// change header
	gray_img.m_bmpHeader->b_filesize = (gray_img.m_bmpInfo->imagesize/3 + gray_img.m_bmpHeader->dataoffset);
	gray_img.m_bmpInfo->bits_per_pixel = 8;
	gray_img.m_bmpInfo->imagesize = gray_img.m_bmpInfo->imagesize/3;
	
	return gray_img;
}

/*
* TODO: fix a return type to efficiently return rgb values or gray value, based on type
* may be - a vector of values is a good choice
*/
rgbTuple ImgProc::at(int row, int col, Image img)
{
	if ((row > img.m_rows) || (col > img.m_cols)
			|| (row < 0) || (col < 0))
	{
		std::cout << "Invalid row or column number" << std::endl;
		return std::make_tuple (0, 0, 0);
	}

	if(img.m_type == _rgb)
	{
		return img.rgb.data[(row * img.m_cols + col)];
	}
	/*else if(img.m_type == _gray)
	{
		return img.gray.data[row][col];
	}*/
}