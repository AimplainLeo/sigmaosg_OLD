// SulCloudPlane2.h

#ifndef __SULCLOUDPLANE2_H__
#define __SULCLOUDPLANE2_H__

#include "SulExport.h"
#include "SulGeomQuad.h"
#include "SulCloudScrollTexture.h"
#include <osg/program>
#include <osg/group>

class mytest : public osg::Drawable::DrawCallback
{
public:
	mytest( osg::TexMat* pTexMat, float fPlaneSize ) :
	m_wind( 0.0, 0.0 )
	{
		m_rTexMat = pTexMat;
		m_fPlaneSize = fPlaneSize;
	}

	virtual ~mytest()
	{
	}

	void setPlaneSize( float fPlaneSize )
	{
		m_fPlaneSize = fPlaneSize;
	}

	void setWind( float x, float y )
	{
		m_wind.set( x, y );
	}

	virtual void drawImplementation( osg::RenderInfo& renderInfo, const osg::Drawable* drawable ) const
	{
		//osg::Drawable::DrawCallback::drawImplementation( renderInfo, drawable );
		//return;

#include <gl/gl.h>

		glColor4us(1,1,1,1);

		osg::FrameStamp* pFrameStamp = renderInfo.getView()->getFrameStamp();
		double simTime = pFrameStamp->getSimulationTime();

		osg::Camera* pCam = renderInfo.getCurrentCamera();
		
		osg::Vec3 v;
		osg::Vec3 c;
		osg::Vec3 u;
		pCam->getViewMatrixAsLookAt( v, c, u );

		// calc absolute position to use on texture coordinates relative to sky quad size
		float k = 1.0f/m_fPlaneSize;
		osg::Vec3 vNew = v*k;

		osg::Matrix mat = m_rTexMat->getMatrix();
		mat(3,0) = (-k*m_wind.x())*simTime-vNew.x();
		mat(3,1) = (-k*m_wind.y())*simTime-vNew.y();
		m_rTexMat->setMatrix(mat);

		osg::State* s = renderInfo.getState();
		unsigned int i = s->getActiveTextureUnit();
		s->setActiveTextureUnit( 0 );
		m_rTexMat->apply( *s );

		//osg::Drawable::DrawCallback::drawImplementation( renderInfo, drawable );
		drawable->drawImplementation( renderInfo );
		
		// reset
		m_rTexMat->setMatrix( osg::Matrix::identity() );
		m_rTexMat->apply( *s );
		s->setActiveTextureUnit( i );
	}

private:
	osg::ref_ptr<osg::TexMat>	m_rTexMat;
	float						m_fPlaneSize;
	osg::Vec2					m_wind;
};



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

	osg::ref_ptr<mytest>				m_mytestass;
};


#endif // __SULCLOUDPLANE2_H__