// SulParticleDebris.h

#ifndef __SULPARTICLEDEBRIS_H__
#define __SULPARTICLEDEBRIS_H__

#include "SulParticle.h"
#include "SulGeodeSphere.h"
#include "SulParticleSystemDataOsg.h"
#include <osgParticle\centeredplacer>

class CSulParticleDebris : public CSulParticle
{
public:
	CSulParticleDebris( 
		osgParticle::CenteredPlacer* placer,
		CSulParticleSystemDataOsg* data,
		const osg::Vec3& velocity,
		float debrisPosOffsetMin = 0.0f,
		float debrisPosOffsetMax = 0.0f
	);

	void enableDebug( osg::Group* root );
	
	void reset()
	{
	}

	// note: getPosition is the local particle system
	virtual void update( double dt );

private:
	CSulParticleSystemDataOsg* m_data;
	osg::ref_ptr<osgParticle::CenteredPlacer>	m_placer;

	bool m_debug;
	osg::ref_ptr<CSulGeodeSphere>		m_sphere;
	osg::ref_ptr<osg::MatrixTransform>	m_sphereMT;
};

#endif // __SULPARTICLEDEBRIS_H__