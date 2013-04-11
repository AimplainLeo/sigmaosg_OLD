// SulParticleSystemInterpolatorQuadratic.h

#ifndef __SULPARTICLESYSTEMINTERPOLATORQUADRATIC_H__
#define __SULPARTICLESYSTEMINTERPOLATORQUADRATIC_H__

#include <osgParticle\interpolator>

class CSulParticleSystemInterpolatorQuadratic: public osgParticle::Interpolator
{
public:
	CSulParticleSystemInterpolatorQuadratic()
	: osgParticle::Interpolator() {}
        
	CSulParticleSystemInterpolatorQuadratic(const CSulParticleSystemInterpolatorQuadratic& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)
	: osgParticle::Interpolator(copy, copyop) {}
        
	META_Object(osgParticle, CSulParticleSystemInterpolatorQuadratic);
        
	virtual float interpolate(float t, float y1, float y2) const
	{
		float x = t;
		float b = 4*y2+y1;
		float a = -b;
		float c = y1;

		return a*x*x+b*x+c;
	}
        
protected:
	virtual ~CSulParticleSystemInterpolatorQuadratic() {}
};

#endif // __SULPARTICLESYSTEMINTERPOLATORQUADRATIC_H__