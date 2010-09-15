// SulPerlinNoiseBase.h

#ifndef __SULPERLINNOISEBASE_H__
#define __SULPERLINNOISEBASE_H__

#include "SulTypes.h"
#include <osg/Referenced>

class CSulPerlinNoiseBase : public osg::Referenced
{
public:
						CSulPerlinNoiseBase();

	void				SetRandomSeed( sigma::uint32 iSeed );

	void				SetOctave( sigma::uint32 iOctaves );
	sigma::uint32		GetOctave();

	void				SetPersistence( float fPersistence );
	float				GetPersistence();

protected:
	sigma::uint32		m_iOctaves;
	float				m_fPersistence;
	sigma::uint32		m_iSeed;
};

#endif // __SULPERLINNOISEBASE_H__