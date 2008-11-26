#include "tiffImageFile.h"
#include "Image.h"

namespace Torch {

///////////////////////////////////////////////////////////////////////////////////////////
// Constructor

tiffImageFile::tiffImageFile()
	:	ImageFile()
#ifdef HAVE_TIFF
		,
		tif(0)
#endif
{
}

///////////////////////////////////////////////////////////////////////////////////////////
// Destructor

tiffImageFile::~tiffImageFile()
{
#ifdef HAVE_TIFF
	if (tif != 0)
	{
		TIFFClose(tif);
	}
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////
// loads some file - overriden

bool tiffImageFile::open(const char* file_name, const char* open_flags)
{
#ifdef HAVE_TIFF

	if (tif != 0)
	{
		TIFFClose(tif);
	}
	tif = 0;

	if(!strcmp(open_flags, "r"))
	{
		tif = TIFFOpen(file_name, "r");
		scanline_buf = NULL;
		return true;
	}
	else if(!strcmp(open_flags, "w"))
	{
		tif = TIFFOpen(file_name, "w");
		scanline_buf = NULL;
		bpp = 3;
		return true;
	}
	else
	{
		message("tiffImageFile::load(%s, %s) incorrect open flag.", file_name, open_flags);
		return false;
	}
#else
	message("tiffImageFile::load TIFF format not supported.");
	return false;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////
// Read the image header

bool tiffImageFile::readHeader(Image& image)
{
	if (isOpened() == false)
	{
		return false;
	}

#ifdef HAVE_TIFF
	int width, height;
	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
	TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &bpp);

    	if(Object::getBOption("verbose"))
    		print("tiffImageFIle::readHeader - %dx%d (%d-bit) pixels\n", width, height, 8 * bpp);

	// OK, resize the image to the new dimensions
	return image.resize(width, height, image.getNPlanes());// Keep the number of color channels
#else
	return false;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////
// Read the image pixmap

bool tiffImageFile::readPixmap(Image& image)
{
	if (isOpened() == false)
	{
		return false;
	}

#ifdef HAVE_TIFF

	const int width = image.getWidth();
	const int height = image.getHeight();
	unsigned char* pixmap = new unsigned char[3 * width * height];

	// Read the pixmap
	uint32* raster = (uint32 *)_TIFFmalloc(width * height * sizeof(uint32));
    	if(!raster)
    	{
    		delete[] pixmap;
		return false;
    	}

   	TIFFReadRGBAImage(tif, width, height, raster, true);
    	unpack_tiff_raster(raster, pixmap);

	// OK, update the image pixels to the pixmap
	ImageFile::fillImage(pixmap, 3, image);
	delete[] pixmap;
	_TIFFfree(raster);
	return true;
#else
	return false;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////
// Write the image header

bool tiffImageFile::writeHeader(const Image& image)
{
#ifdef HAVE_TIFF

	const int width = image.getWidth();
	const int height = image.getHeight();
	const unsigned int scanline_size = bpp * width;

	TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
	TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
	TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, bpp);
	TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    	TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, width * bpp);
	TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
	TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
	TIFFSetField(tif, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
	TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);

	if(TIFFScanlineSize(tif) != scanline_size)
	{
		warning("tiffImageFile::writeHeader - Mismatch with library's expected scanline size.\n");
		return false;
    	}

	return true;
#else
	return false;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////
// Write the image pixmap

bool tiffImageFile::writePixmap(const Image& image)
{
#ifdef HAVE_TIFF

	const int width = image.getWidth();
	const int height = image.getHeight();

	const int scanline_size = bpp * width;
	char* scanline_buf = new char[scanline_size];

    	// Fill the pixmap with the image pixels
	unsigned char* pixmap = new unsigned char[3 * width * height];
	ImageFile::fillPixmap(pixmap, 3, image);

	// Write the pixmap
   	char* scanline = (char *) pixmap;
    	for(int y = 0 ; y < height ; y++)
    	{
		memcpy(scanline_buf, scanline, scanline_size);

		TIFFWriteScanline(tif, scanline_buf, y, 0);

		scanline += scanline_size;
    	}

	// OK
	delete[] pixmap;
	delete[] scanline_buf;
	return true;
#else
	return -1;
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////
// Specific TIFF decoding/encoding functions

#ifdef HAVE_TIFF
void tiffImageFile::unpack_tiff_raster(uint32 *raster, unsigned char* pixmap)
{
	int x,y;
	uint32 pixel;
	uint32 pic_index = 0;

	for(y=(height-1); y>=0; y--)
		for(x=0; x<width; x++)
		{
	    		pixel = raster[y*width + x];

	    		if(bpp == 1)
	    		{
	       			unsigned char r_ = TIFFGetR(pixel);

	    			pixmap[pic_index++] = r_;
	    			pixmap[pic_index++] = r_;
	    			pixmap[pic_index++] = r_;
	    		}
	    		else if(bpp == 3)
	    		{
	    			pixmap[pic_index++] = TIFFGetR(pixel);
	    			pixmap[pic_index++] = TIFFGetG(pixel);
	    			pixmap[pic_index++] = TIFFGetB(pixel);
	    		}
		}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////


}
