// SulMath.h

#ifndef __SULMATH_H__
#define __SULMATH_H__

namespace sigma
{
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
