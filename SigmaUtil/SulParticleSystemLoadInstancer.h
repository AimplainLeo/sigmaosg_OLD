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
			~CSulParticleSystemLoadInstancer();

	// loads the xml file that defines the particle effect
	bool	load( const CSulString& file );

	// create as many particle effects as you want
	void	create( const osg::Vec3& pos );

	void	setWind( const osg::Vec3& wind );

	CSulParticleSystemOsg* getParticleSystem();

private:
	osg::ref_ptr<CSulParticleSystemOsg>								m_particleSystem;

	typedef std::vector< osg::ref_ptr<CSulParticleSystemDataOsg> >	VEC_DATA;
	VEC_DATA														m_vecData;

	osg::Vec3														m_wind;
};

#endif // __SULPARTICLESYSTEMLOADINSTANCER_H__


