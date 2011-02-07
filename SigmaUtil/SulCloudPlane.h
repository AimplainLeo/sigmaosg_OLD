// SulCloudPlane2.h

#ifndef __SULCLOUDPLANE2_H__
#define __SULCLOUDPLANE2_H__

#include "SulExport.h"
#include "SulGeomQuad.h"
#include "SulCloudScrollTexture.h"
#include <osg/program>
#include <osg/group>

class SUL_EXPORT CSulCloudPlane : public osg::Group
{
public:
					CSulCloudPlane();
					CSulCloudPlane( float size );

	void			create();

	void			setSize( float s );

	void			setWind( float x, float y );

	CSulGeomQuad*	getQuad();

protected:
	virtual osg::Program* createShaderProgram();

private:
	osg::ref_ptr<CSulGeomQuad>			m_rQuad;
	osg::ref_ptr<CCloudScrollTexture>	m_rCloudScrollTexture;
	float								m_size;
	float								m_height;
};

#endif // __SULCLOUDPLANE2_H__