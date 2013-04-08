// SulParticleSystemLoadInstancer.h

#ifndef __SULPARTICLESYSTEMLOADINSTANCER_H__
#define __SULPARTICLESYSTEMLOADINSTANCER_H__

#include "SulString.h"
#include "SulExport.h"

class SUL_EXPORT CSulParticleSystemLoadInstancer : public osg::Referenced
{
public:
	void	create( const osg::Vec3& pos );

	bool	load( const CSulString& file );
};

#endif // __SULPARTICLESYSTEMLOADINSTANCER_H__


