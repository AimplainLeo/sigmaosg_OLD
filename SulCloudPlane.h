// SulCloudPlane2.h

#ifndef __SULCLOUDPLANE2_H__
#define __SULCLOUDPLANE2_H__

#include "SulExport.h"
#include "SulGeomQuad.h"
#include "SulCloudScrollTexture.h"
#include <osg/program>

class SUL_EXPORT CSulCloudPlane : public CSulGeomQuad
{
public:
					CSulCloudPlane();
					CSulCloudPlane( float size );

	void			setSize( float s );

	void			setWind( float x, float y );

protected:
	virtual void	createDrawable();

	virtual osg::Program* createShaderProgram();

private:
	osg::ref_ptr<CCloudScrollTexture>	m_rCloudScrollTexture;
	float m_size;
	float m_height;
};

#endif // __SULCLOUDPLANE2_H__