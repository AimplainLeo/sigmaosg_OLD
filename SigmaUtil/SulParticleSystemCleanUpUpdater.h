// SulParticleSystemCleanUpUpdater.h

#ifndef __SULPARTICLESYSTEMCLEANUPUPDATER_H__
#define __SULPARTICLESYSTEMCLEANUPUPDATER_H__

/*
	The purpose of this updater is to check to see if the particlessystemcontainer objects in the SulParticleSystemOsg are still alive.
	If they are all dead then the updater will call remove all in the SulParticleSystemOsg, this will effectily cleanup all the dead particles.
*/

#include <osg/nodecallback>

class CSulParticleSystemCleanUpUpdater : public osg::NodeCallback
{
public:
	CSulParticleSystemCleanUpUpdater( class CSulParticleSystemOsg* ps );

private:
	void operator()( osg::Node* node, osg::NodeVisitor* nv );

private:
	class CSulParticleSystemOsg* m_ps;
};

#endif // __SULPARTICLESYSTEMCLEANUPUPDATER_H__