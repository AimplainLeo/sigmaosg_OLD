// SulParticleSystemLoadInstancer.h

#ifndef __SULPARTICLESYSTEMLOADINSTANCER_H__
#define __SULPARTICLESYSTEMLOADINSTANCER_H__

#include "SulParticleSystemOsg.h"
#include "SulParticleSystemDataOsg.h"
#include "SulString.h"
#include "SulExport.h"

class SUL_EXPORT CSulParticleSystemLoadInstancer : public osg::Group
{
public:
			CSulParticleSystemLoadInstancer();

	void	create( const osg::Vec3& pos );

	bool	load( const CSulString& file );

private:
	osg::ref_ptr<CSulParticleSystemOsg>								m_particleSystem;

	typedef std::vector< osg::ref_ptr<CSulParticleSystemDataOsg> >	VEC_DATA;
	VEC_DATA														m_vecData;
};

#endif // __SULPARTICLESYSTEMLOADINSTANCER_H__


