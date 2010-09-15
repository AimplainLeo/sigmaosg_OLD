// SulPerlinNoise2D.h

#ifndef __SULPERLINNOISE2D_H__
#define __SULPERLINNOISE2D_H__

#include "SulArray2D.h"
#include "SulPerlinNoiseBase.h"
#include "SulTexImage.h"

class CSulPerlinNoise2D : public CSulPerlinNoiseBase
{
public:
	void CreateArray2D( CSulArray2D<float>* p, float fScale=1.0f )
	{
		m_iOctaves = 4;

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
				f = f*fScale;
				p->Set( x, y, f );
			}
		}
	}

	void CreateArray2D( CSulArray2D<sigma::uint8>* p, float fScale=1.0f, float fOfs=128.0f, sigma::uint8 iMin=0, sigma::uint8 iMax=255 )
	{
		m_iOctaves = 4;

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

	void CreateArray2D( CSulTexImage* pTexImage, float fScale=1.0f, float fOfs=128.0f, sigma::uint32 iMin=0, sigma::uint32 iMax=255 );

private:
	float				IntNoise( sigma::int32 x, sigma::int32 y );
	float				IntNoiseLoop( sigma::int32 x, sigma::int32 y, float freq );
	float				CosInterpolate( float a, float b, float t );
	float				InterpolatedNoise( float x, float y, float freq );
	float				PerlinNoise( float x, float y );
};

#endif // __SULPERLINNOISE2D_H__