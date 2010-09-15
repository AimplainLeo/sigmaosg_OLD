// SulPerlinNoise1D.cpp

#include "stdafx.h"
#include "SulPerlinNoise1D.h"

float CSulPerlinNoise1D::LinearInterpolate( float a, float b, float t )
{
	return  a*(1.0f-t) + b*t;
}

float CSulPerlinNoise1D::CosInterpolate( float a, float b, float t )
{
	float ft = t * 3.1415927f;
	float f = (1.0f - cos(ft)) * 0.5f;

	return  a*(1-f) + b*f;
}

float CSulPerlinNoise1D::CubicInterpolate( float a, float b, float t )
{
	//  not done,.. requires for points
	return 0.0f;
}

float CSulPerlinNoise1D::IntNoise( sigma::int32 x )
{
	// FIXME: bad seeding
	x = m_iSeed+x*57;

	sigma::int32 n = x;
	n =	(n<<13) ^ n;
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);    
}

float CSulPerlinNoise1D::IntNoiseLoop( sigma::int32 x, float freq )
{
	return ( (freq-x)*IntNoise(x)+x*IntNoise(x-freq) ) / freq;
}

float CSulPerlinNoise1D::InterpolatedNoise( float x, float freq )
{
	sigma::int32 integer_X    = int(x);
	float fractional_X = x - integer_X;

	float v1 = IntNoiseLoop(integer_X, freq );
	float v2 = IntNoiseLoop(integer_X+1, freq );
	
	//return LinearInterpolate( v1 , v2 , fractional_X );
	return CosInterpolate( v1 , v2 , fractional_X );
}

float CSulPerlinNoise1D::PerlinNoise1D( float x )
{
	float total = 0.0f;

	sigma::uint32 i;
	for ( i=0; i<m_iOctaves; i++ )
	{
		float frequency = pow( 2.0f,(sigma::int32)i );
		float amplitude = pow( m_fPersistence,(sigma::int32)i );

		total = total + InterpolatedNoise( x*frequency, frequency ) * amplitude;
	}
	
	return total;
}

void CSulPerlinNoise1D::CreateArray2D( CSulArray2D<sigma::uint8>* p, float fScale, float fOfs, sigma::uint32 iMin, sigma::uint32 iMax )
{
	float w = p->GetWidth();
	
	m_iOctaves = 6;

	sigma::uint32 x;
	for ( x=0; x<w; x++ )
	{
		float val = (float)x/w;
		float f = PerlinNoise1D( val );	

		f = f*fScale+fOfs;
		if ( f<iMin ) f=iMin;
		if ( f>iMax ) f=iMax;
		p->Set( x, f, 0 );
	}
}

void CSulPerlinNoise1D::CreateArray2D( CSulTexImage* p, float fScale, float fOfs, sigma::uint32 iMin, sigma::uint32 iMax )
{
	float w = p->GetWidth();
	
	m_iOctaves = 6;

	sigma::uint32 x;
	for ( x=0; x<w; x++ )
	{
		float val = (float)x/w;
		float f = PerlinNoise1D( val );	

		f = f*fScale+fOfs;
		if ( f<iMin ) f=iMin;
		if ( f>iMax ) f=iMax;
		p->Set( x, f, 0 );
	}
}