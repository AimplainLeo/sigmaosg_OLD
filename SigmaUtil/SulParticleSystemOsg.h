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

	// creates a new particle system instance using the current variable values
	CSulParticleSystemContainerOsg* createParticleSystem();
	void placeParticleSystem( const osg::Vec3& pos, CSulParticleSystemContainerOsg* p );

	// some Qt stuff
	void updateFromUI();
	void updateToUI();
	QWidget* createPropertySheet();

	void						setData( CSulParticleSystemDataOsg* data );
	CSulParticleSystemDataOsg*	getData();

private:
	osgParticle::ParticleSystemUpdater*	m_psu;

	//////////////////////////////////////////////////////////////////////////////////////
	// basic idea; you can setup all the values to be used by the particle system
	// it then will create that particle system at the specified position. This allows
	// us to create many particle systems using this as a template parameter list.
	//////////////////////////////////////////////////////////////////////////////////////

	CSulParticleSystemDataOsg*	m_data;
	
	CSulQtPropertySheet*		m_psParticles;
	CSulQtPropertySheet*		m_psParticleSystem;
	CSulQtPropertySheet*		m_psModularEmitter;
	CSulQtPropertySheet*		m_psCounterRandomRate;
	CSulQtPropertySheet*		m_psSectorPlacer;
	CSulQtPropertySheet*		m_psShooterRadial;
	CSulQtPropertySheet*		m_psProgramFluid;
};


#endif // __SULPARTICLESYSTEMOSG_H__