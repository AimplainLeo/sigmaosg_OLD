// SulPerlinNoise1D.h

#ifndef __SULPERLINNOISE1D_H__
#define __SULPERLINNOISE1D_H__

#include "SulPerlinNoiseBase.h"
#include "SulArray2D.h"
#include "SulTexImage.h"

class CSulPerlinNoise1D : public CSulPerlinNoiseBase
{
public:
	// will create 2D representation of the perlin noise
	void CreateArray2D( CSulArray2D<sigma::uint8>* p, float fScale=1.0f, float fOfs=128.0f, sigma::uint32 iMin=0, sigma::uint32 iMax=255 );

	void CreateArray2D( CSulTexImage* pTexImage, float fScale=1.0f, float fOfs=128.0f, sigma::uint32 iMin=0, sigma::uint32 iMax=255 );

private:
	float				IntNoise( sigma::int32 x );
	float				IntNoiseLoop( sigma::int32 x, float freq );
	float				SmoothedNoise1( float x, float freq );
	float				LinearInterpolate( float a, float b, float t );
	float				CosInterpolate( float a, float b, float t );
	float				CubicInterpolate( float a, float b, float t );
	float				InterpolatedNoise( float x, float freq );
	float				PerlinNoise1D( float x );

};

#endif // __SULPERLINNOISE1D_H__