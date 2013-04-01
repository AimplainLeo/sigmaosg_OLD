// SulParticleSystemOsg.h

#ifndef __SULPARTICLESYSTEMOSG_H__
#define __SULPARTICLESYSTEMOSG_H__

#include "SulTypes.h"
#include "SulString.h"
#include "SulQtPropertySheet.h"
#include "SulParticleSystemContainerOsg.h"
#include "SulParticleSystemDataOsg.h"
#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/FluidProgram>
#include <QtGui/qwidget>

class CSulParticleSystemOsg : public osg::Group
{
public:
	CSulParticleSystemOsg();

//	void removeAll();

	void create( CSulParticleSystemContainerOsg* psContainer, const osg::Vec3& pos );
	//void place( const osg::Vec3& pos );

private:
	osgParticle::ParticleSystemUpdater*	m_psu;
	/*
	typedef std::vector< osg::ref_ptr<CSulParticleSystemContainerOsg> >	VEC_PARTICLESYSTEMCONTAINEROSG;
	VEC_PARTICLESYSTEMCONTAINEROSG										m_vecParticleSystemContainer;
	*/
};


#endif // __SULPARTICLESYSTEMOSG_H__