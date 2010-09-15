// SulCloudScrollTexture.h

#ifndef __SULCLOUDSCROLLTEXTURE_H__
#define __SULCLOUDSCROLLTEXTURE_H__

#include <osg/geometry>
#include <osg/TexMat>

class CCloudScrollTexture : public osg::Geometry
{
public:
	CCloudScrollTexture( osg::TexMat* pTexMat, float fPlaneSize ) :
	m_wind( 0.0, 0.0 )
	{
		m_rTexMat = pTexMat;
		m_fPlaneSize = fPlaneSize;
	}

	virtual ~CCloudScrollTexture()
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

	virtual void drawImplementation( osg::RenderInfo& renderInfo ) const
	{
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

		osg::Geometry::drawImplementation(renderInfo);
		
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

#endif // __SULCLOUDSCROLLTEXTURE_H__