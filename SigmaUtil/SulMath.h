// SulMath.h

#ifndef __SULMATH_H__
#define __SULMATH_H__

namespace sigma
{
	inline double DEG2RAD(const double val) { return val*0.0174532925199432957692369076848861;}
	inline double RAD2DEG(const double val) { return val*57.2957795130823208767981548141052;}

	const unsigned long RAND_MAX_PLUS_1 = RAND_MAX + 1;
	const unsigned long BIG_RAND_MAX = RAND_MAX_PLUS_1 * RAND_MAX_PLUS_1 - 1;

	static unsigned long bigRand()
	{
		return rand() * (RAND_MAX_PLUS_1) + rand();
	}

	static float rand0to1()
	{
		return static_cast<float>(bigRand()) / BIG_RAND_MAX;
	}



}; // end namespace Sigma

#endif // __SULMATH_H__
