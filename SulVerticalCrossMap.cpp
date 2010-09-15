// SulVerticalCrossMap.cpp

#include "stdafx.h"
#include "SulVerticalCrossMap.h"
#include <osgDB/ReadFile>
#include <assert.h>
#include <iostream>

//---------------------------------------------------------------------------
// (c) 2006-2008   Jean-S?stien Guay
// This code is distributed in the hope it will be useful, but without 
// any warranty whatsoever. You may use it in any project, as well as modify
// it without restriction, as long as this notice is retained in the 
// relevant files.
//---------------------------------------------------------------------------

/** Implementation of copyImage. */
template<typename T>
void copyDataImpl(const osg::Image* source,
                  const unsigned int x1, const unsigned int y1,
                  const unsigned int x2, const unsigned int y2,
                  osg::Image* destination, 
                  const unsigned int xd = 0, const unsigned int yd = 0,
                  const bool clamp = false)
{
    if ((unsigned int)destination->s() >= xd + (x2 - x1) && 
        (unsigned int)destination->t() >= yd + (y2 - y1))
    {
        const unsigned int bpps =      source->getPixelSizeInBits() / (8 * sizeof(T));
        const unsigned int bppd = destination->getPixelSizeInBits() / (8 * sizeof(T));

        T* srcdata = (T*)source->data();
        T* dstdata = (T*)destination->data();

        for (unsigned int y = 0; y < y2 - y1; ++y)
        {
            for (unsigned int x = 0; x < x2 - x1; ++x)
            {
                T r = srcdata[(y + y1) * source->s() * bpps + (x + x1) * bpps + 0];
                T g = srcdata[(y + y1) * source->s() * bpps + (x + x1) * bpps + 1];
                T b = srcdata[(y + y1) * source->s() * bpps + (x + x1) * bpps + 2];

                if (clamp)
                {
                    r = osg::clampTo(r, (T)0, (T)1);
                    g = osg::clampTo(g, (T)0, (T)1);
                    b = osg::clampTo(b, (T)0, (T)1);
                }

                dstdata[(yd + y) * destination->s() * bppd + (xd + x) * bppd + 0] = r;
                dstdata[(yd + y) * destination->s() * bppd + (xd + x) * bppd + 1] = g;
                dstdata[(yd + y) * destination->s() * bppd + (xd + x) * bppd + 2] = b;
            }
        }
    }
    else
        assert(false && "copyDataImpl: Incorrect image dimensions.");
}

/** Copies a rectangle of corners (x1, y1), (x2, y2) from an image into 
    another image starting at position (xd, yd). No scaling is done, the
    pixels are just copied, so the destination image must be at least 
    (xd + (x2 - x1)) by (yd + (y2 - y1)) pixels. */
void copyData(const osg::Image* source,
              const unsigned int x1, const unsigned int y1, 
              const unsigned int x2, const unsigned int y2,
              osg::Image* destination, 
              const unsigned int xd, const unsigned int yd,
              const bool clamp)
{
    if (source->getDataType() == destination->getDataType())
    {
        if (source->getDataType() == GL_FLOAT)
        {
            copyDataImpl<float>(source, x1, y1, x2, y2, 
                                destination, xd, yd, clamp);
        }
        else if (source->getDataType() == GL_UNSIGNED_BYTE)
        {
            copyDataImpl<unsigned char>(source, x1, y1, x2, y2, 
                                        destination, xd, yd, clamp);
        }
        else
        {
            assert(false && "copyData not implemented for this data type");
        }
    }
    else
    {
        assert(false && "source and destination images must be of the same type.");
        return;
    }
}


/** Implementation of rotateImage. */
template<typename T>
osg::Image* rotateImageImpl(osg::Image* image)
{
    if (image->s() == image->t())
    {
        const unsigned int s = image->s();
        const unsigned int bpp = image->getPixelSizeInBits() / (8 * sizeof(T));

        osg::ref_ptr<osg::Image> destination  = new osg::Image;
        destination->allocateImage(s, s, 1,
            image->getPixelFormat(), image->getDataType(),
            image->getPacking());
        destination->setInternalTextureFormat(image->getInternalTextureFormat());

        T* srcdata = (T*)image->data();
        T* dstdata = (T*)destination->data();

        for (unsigned int y = 0; y < s; ++y)
        {
            for (unsigned int x = 0; x < s; ++x)
            {
                dstdata[y * s * bpp + x * bpp + 0] = srcdata[x * s * bpp + y * bpp + 0];
                dstdata[y * s * bpp + x * bpp + 1] = srcdata[x * s * bpp + y * bpp + 1];
                dstdata[y * s * bpp + x * bpp + 2] = srcdata[x * s * bpp + y * bpp + 2];
            }
        }

        return destination.release();
    }
    else
    {
        assert(false && "rotateImageImpl: Image must be square.");
        return 0;
    }
}

/** Rotates an osg::Image by 90 degrees. Returns a new osg::Image, be sure to
    store it in a ref_ptr so it will be freed correctly. */
osg::Image* rotateImage(osg::Image* image)
{
    if (image->getDataType() == GL_FLOAT)
    {
        return rotateImageImpl<float>(image);
    }
    else if (image->getDataType() == GL_UNSIGNED_BYTE)
    {
        return rotateImageImpl<unsigned char>(image);
    }
    else
    {
        assert(false && "rotateImage not implemented for this data type");
        return 0;
    }
}


template<typename T>
float getMaxValueImpl(const osg::Image* image)
{
    const unsigned int size = image->getImageSizeInBytes() / sizeof(T);
    T* data = (T*)image->data();

    T maxValue = FLT_MIN;
    for (unsigned int i = 0; i < size; i++)
    {
        if (data[i] > maxValue)
            maxValue = data[i];
    }

    return (T)maxValue;
}

float getMaxValue(osg::Image* image)
{
    if (image->getDataType() == GL_FLOAT)
    {
        return getMaxValueImpl<float>(image);
    }
    else if (image->getDataType() == GL_UNSIGNED_BYTE)
    {
        return 1.0f;
    }
    else
    {
        assert(false && "getMaxValue not implemented for this data type");
        return 0;
    }
}


CSulVerticalCrossMap::CSulVerticalCrossMap( const CSulString& file ) :
osg::TextureCubeMap()
{
//	setSourceFormat( GL_RGB16F_ARB );
	setInternalFormat( GL_RGB16F_ARB );
	setSourceType( GL_FLOAT );
	setResizeNonPowerOfTwoHint(false);
	setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
	setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);

	loadVerticalCrossMap( file );
}

bool CSulVerticalCrossMap::loadVerticalCrossMap( const CSulString& file )
{
    const osg::ref_ptr<osg::Image> cross = osgDB::readImageFile(file);
//	cross->setInternalTextureFormat( GL_RGB16F_ARB );
//	cross->setPixelFormat( GL_FLOAT );
    if (!cross.valid())
    {
        std::cout << "Image file " << file << " could not be loaded." << std::endl;
        return 0;
    }

    /*const*/ GLenum pixelFormat   = cross->getPixelFormat();
    /*const*/ GLenum dataType      = cross->getDataType();
    /*const*/ GLint internalFormat = cross->getInternalTextureFormat();
    unsigned int packing       = cross->getPacking();
    const unsigned int s = (unsigned int)cross->s();
    const unsigned int t = (unsigned int)cross->t();
    const unsigned int one_third_s = (unsigned int)(float(s) * 1/3);
    const unsigned int two_thirds_s = one_third_s * 2;
    const unsigned int one_quarter_t = (unsigned int)(float(t) * 1/4);
    const unsigned int one_half_t = one_quarter_t * 2;
    const unsigned int three_quarters_t = one_quarter_t * 3;

	dataType = GL_FLOAT;
//	pixelFormat = GL_FLOAT;
	internalFormat = GL_RGB16F_ARB;

  //  osg::ref_ptr<osg::TextureCubeMap> cubeMap = new osg::TextureCubeMap;
    setTextureSize(one_third_s, one_quarter_t);


    zplus  = new osg::Image;
    zplus->allocateImage(one_third_s, one_quarter_t, 1,
        pixelFormat, dataType, packing);
    zplus->setInternalTextureFormat(internalFormat);
    copyData(cross.get(), one_third_s, one_quarter_t, two_thirds_s, one_half_t, zplus.get(), 0, 0, false);
    setImage(osg::TextureCubeMap::POSITIVE_Z, zplus.get());

	zminus = new osg::Image;
    zminus->allocateImage(one_third_s, one_quarter_t, 1,
        pixelFormat, dataType, packing);
    zminus->setInternalTextureFormat(internalFormat);
    copyData(cross.get(), one_third_s, three_quarters_t, two_thirds_s, t, zminus.get(), 0, 0, false);
    zminus->flipVertical();
    zminus->flipHorizontal();
    setImage(osg::TextureCubeMap::NEGATIVE_Z, zminus.get());

    yplus  = new osg::Image;
    yplus->allocateImage(one_third_s, one_quarter_t, 1,
        pixelFormat, dataType, packing);
    yplus->setInternalTextureFormat(internalFormat);
    copyData(cross.get(), one_third_s, 0, two_thirds_s, one_quarter_t, yplus.get(), 0, 0, false);
    setImage(osg::TextureCubeMap::POSITIVE_Y, yplus.get());

    yminus = new osg::Image;
    yminus->allocateImage(one_third_s, one_quarter_t, 1,
        pixelFormat, dataType, packing);
    yminus->setInternalTextureFormat(internalFormat);
    copyData(cross.get(), one_third_s, one_half_t, two_thirds_s, three_quarters_t, yminus.get(), 0, 0, false);
    setImage(osg::TextureCubeMap::NEGATIVE_Y, yminus.get());

    xplus  = new osg::Image;
    xplus->allocateImage(one_third_s, one_quarter_t, 1,
        pixelFormat, dataType, packing);
    xplus->setInternalTextureFormat(internalFormat);
    copyData(cross.get(), two_thirds_s, one_half_t, s, three_quarters_t, xplus.get(), 0, 0, false);
    xplus = rotateImage(xplus.get());
    xplus->flipVertical();
    setImage(osg::TextureCubeMap::POSITIVE_X, xplus.get());

	xminus = new osg::Image;
    xminus->allocateImage(one_third_s, one_quarter_t, 1,
        pixelFormat, dataType, packing);
    xminus->setInternalTextureFormat(internalFormat);
    copyData(cross.get(), 0, one_half_t, one_third_s, three_quarters_t, xminus.get(), 0, 0, false);
    xminus = rotateImage(xminus.get());
    xminus->flipHorizontal();
    setImage(osg::TextureCubeMap::NEGATIVE_X, xminus.get());

	return true;
}

osg::Image*	CSulVerticalCrossMap::getImage( osg::TextureCubeMap::Face face )
{
	switch ( face )
	{	
		case osg::TextureCubeMap::POSITIVE_X:	return xplus;
		case osg::TextureCubeMap::NEGATIVE_X:	return xminus;
		case osg::TextureCubeMap::POSITIVE_Y:	return yplus;
		case osg::TextureCubeMap::NEGATIVE_Y:	return yminus;
		case osg::TextureCubeMap::POSITIVE_Z:	return zplus;
		case osg::TextureCubeMap::NEGATIVE_Z:	return zminus;

		default:
			assert( 0 );	// not implementated yet
	}

	return 0;
}
