// SulPerlinNoise2D.cpp

#include "stdafx.h"
#include "SulPerlinNoise2D.h"

CSulPerlinNoise2D::CSulPerlinNoise2D() :
CSulPerlinNoiseBase()
{
	m_w = 256;
	m_h = 256;
	m_s = 1.0f;
	m_o = 0.0f;
}

void CSulPerlinNoise2D::setWidth( sigma::uint32 w )
{
	m_w = w;
}

void CSulPerlinNoise2D::setHeight( sigma::uint32 h )
{
	m_h = h;
}

void CSulPerlinNoise2D::setScale( float s )
{
	m_s = s;
}

void CSulPerlinNoise2D::setOfs( float o )
{
	m_o = o;
}

float CSulPerlinNoise2D::IntNoise( sigma::int32 x, sigma::int32 y )
{
	// FIXME: bad seeding
	x = m_iSeed*m_iSeed*x;
	y = m_iSeed*m_iSeed*y;

	sigma::int32 n = x+y*57;
	n =	(n<<13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
}

float CSulPerlinNoise2D::IntNoiseLoop( sigma::int32 x, sigma::int32 y, float freq )
{
	float f = 
		(
			IntNoise(x,y)*(freq-x)*(freq-y)+
			IntNoise(x-freq,y)*x*(freq-y)+
			IntNoise(x-freq,y-freq)*x*y+
			IntNoise(x,y-freq)*(freq-x)*y
			)/(freq*freq);

	return f;
}

float CSulPerlinNoise2D::CosInterpolate( float a, float b, float t )
{
	float ft = t * 3.1415927f;
	float f = (1.0f - cos(ft)) * 0.5f;

	return  a*(1-f) + b*f;
}

float CSulPerlinNoise2D::InterpolatedNoise( float x, float y, float freq )
{
	sigma::int32 integer_X    = int(x);
	float fractional_X = x - integer_X;

	sigma::int32 integer_Y    = int(y);
	float fractional_Y = y - integer_Y;

	float v1 = IntNoiseLoop( integer_X,		integer_Y, freq );
	float v2 = IntNoiseLoop( integer_X+1,	integer_Y, freq );
	float v3 = IntNoiseLoop( integer_X,		integer_Y+1, freq );
	float v4 = IntNoiseLoop( integer_X+1,	integer_Y+1, freq );

	float i1= CosInterpolate( v1, v2, fractional_X );
	float i2= CosInterpolate( v3, v4, fractional_X );	  

	return  CosInterpolate( i1 ,i2 ,fractional_Y );	  
}

float CSulPerlinNoise2D::PerlinNoise( float x, float y )
{
	float total = 0.0f;

	sigma::uint32 i;
	for ( i=0; i<m_iOctaves; i++ )
	{
		float frequency = pow( 2.0f,(sigma::int32)i );
		float amplitude = pow( m_fPersistence,(sigma::int32)i );

		total = total + InterpolatedNoise( x*frequency, y*frequency, frequency ) * amplitude;
	}
	
	return total;
}

void CSulPerlinNoise2D::CreateArray2D( CSulTexImage* p, float fScale, float fOfs, sigma::uint32 iMin, sigma::uint32 iMax )
{
	float w = p->GetWidth();
	float h = p->GetHeight();

	sigma::uint32 x,y;
	for ( y=0; y<h; y++ )
	{
		float valY = (float)y/h;

		for ( x=0; x<w; x++ )
		{
			float valX = (float)x/w;
			
			float f = PerlinNoise( valX, valY );	
			f = f*fScale+fOfs;
			if ( f<iMin ) f = iMin;
			if ( f>iMax ) f = iMax;
			p->Set( x, y, (sigma::uint8)f );
		}
	}
}

void CSulPerlinNoise2D::create( osg::Image* pImage )
{
	float* pdata = new float[m_w*m_h];

	sigma::uint32 x,y;
	for ( y=0; y<m_h; y++ )
	{
		float valY = (float)y/(float)m_h;

		for ( x=0; x<m_w; x++ )
		{
			float valX = (float)x/m_w;
			
			float f = PerlinNoise( valX, valY );	
			f = f*m_s+m_o;

			f = 1.0+f;
			if ( f<0.0 ) f = 0.0;
			if ( f>1.0 ) f = 1.0;

			*(pdata+y*(int)m_w+x) = f;
		}
	}

	pImage->setImage(
        m_w, m_h, 1,								// 1=r? depth perhaps?
		GL_LUMINANCE,								// internal format
        GL_LUMINANCE, GL_FLOAT,						// pixelformat, type
        reinterpret_cast<unsigned char*>(pdata),	// data
        osg::Image::USE_NEW_DELETE,
        1 );										// packing

	//delete pdata;
}