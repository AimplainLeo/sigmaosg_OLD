// SulLightSpot.h

#ifndef __SULLIGHTSPOT_H__
#define __SULLIGHTSPOT_H__

#include "SulLightBase.h"
#include "SulExport.h"

class SUL_EXPORT CSulLightSpot : public CSulLightBase
{
public:
						CSulLightSpot( CSulLightManager* lm );

	void				setDirection( const osg::Vec3& dir );
	const osg::Vec3&	getDirection() const;

	void				setViewMatrixDirection( const osg::Vec3& dir );
	const osg::Vec3&	getViewMatrixDirection();

	void				setCutOff( float cutoff );
	float				getCutOff();

	virtual void		traverse( osg::NodeVisitor& nv );

private:
	osg::Vec3			m_dir;
	osg::Vec3			m_dirViewMatrix;
	float				m_cutoff;
};

typedef std::vector< CSulLightSpot* >	VEC_LIGHT_SPOT;


#endif // __SULLIGHTSPOT_H__