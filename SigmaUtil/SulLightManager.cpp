// SulLightManager.cpp

#include "stdafx.h"
#include "SulLightManager.h"
#include <osg/texture2d>

template <class T>
T nexthigher(T k) 
{
    if ( k==0 )
		return 1;
    k--;
    for ( int i=1; i<sizeof(T)*CHAR_BIT; i<<=1 )
            k = k | k >> i;
    return k+1;
}	

CSulLightManager::CSulLightManager() :
osg::Group()
{
	m_lastFrame		= -1;
	m_countCameras	= 0;

	m_uniformTexLightPoint = new osg::Uniform( osg::Uniform::SAMPLER_2D, "texLightPoint" );
	m_uniformTexLightPoint->set( (int)7 );
	getOrCreateStateSet()->addUniform( m_uniformTexLightPoint );

	m_uniformCountLightPoints = new osg::Uniform( osg::Uniform::INT, "countLightPoints" );
	//getOrCreateStateSet()->addUniform( m_uniformCountLightPoints );

	m_uniformCountLightSpots = new osg::Uniform( osg::Uniform::INT, "countLightSpots" );
	getOrCreateStateSet()->addUniform( m_uniformCountLightSpots );

//	createTextureData( 8, 1024 ); // support for 1024 lights

	// create one default texture 
	createTextureData( 8, 1024 );		
	m_vecImage.push_back( m_rImage );
	m_vecTextureRectangle.push_back( m_rTextureData );
}

const VEC_LIGHT_POINT& CSulLightManager::getLightPointList()
{
	return m_lightPoint;
}

const VEC_LIGHT_SPOT& CSulLightManager::getLightSpotList()
{
	return m_lightSpot;
}

void CSulLightManager::reset()
{
	m_lightPoint.clear();
	m_lightSpot.clear();
}

void CSulLightManager::addVisible( CSulLightPoint* pLight )
{
	m_lightPoint.push_back( pLight );
//	buildTextureData();
//	m_uniformCountLightPoints->set( (int)m_lightPoint.size() );
}

void CSulLightManager::addVisible( CSulLightSpot* pLight )
{
	m_lightSpot.push_back( pLight );
//	buildTextureData();
//	m_uniformCountLightSpots->set( (int)m_lightSpot.size() );
}

void CSulLightManager::createTextureData( sigma::uint32 sizeX, sigma::uint32 sizeY )
{
	m_rImage = new osg::Image;
	m_rImage->allocateImage( sizeX, sizeY, 1, GL_RGBA, GL_FLOAT );	
	m_rImage->setInternalTextureFormat( GL_RGBA32F_ARB );

	m_rTextureData = new osg::TextureRectangle( m_rImage );
	m_rTextureData->setInternalFormat( GL_RGBA32F_ARB );
	m_rTextureData->setSourceFormat( GL_FLOAT );
	m_rTextureData->setFilter( osg::Texture2D::MIN_FILTER, osg::Texture2D::NEAREST );
	m_rTextureData->setFilter( osg::Texture2D::MAG_FILTER, osg::Texture2D::NEAREST );

//	getOrCreateStateSet()->setTextureAttributeAndModes( 7, m_rTextureData, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED );
}

osg::Uniform* CSulLightManager::getUniformTexLightData()
{
	return m_uniformTexLightPoint;
}

osg::Uniform* CSulLightManager::getUniformCountLightPoints()
{
	return m_uniformCountLightPoints;
}

osg::Uniform* CSulLightManager::getUniformCountLightSpots()
{
	return m_uniformCountLightSpots;
}

osg::TextureRectangle* CSulLightManager::getTextureData()
{
	return m_rTextureData;
}

void CSulLightManager::buildTextureData()
{
	float* p = reinterpret_cast<float*>(m_rImage->data());

	///////////////////////////////////////////////////////////////
	// point light
	///////////////////////////////////////////////////////////////

	{
		VEC_LIGHT_POINT::const_iterator is = m_lightPoint.begin();
		VEC_LIGHT_POINT::const_iterator ie = m_lightPoint.end();
		sigma::uint32 i = 0;
		while ( is!=ie )
		{
			const osg::Vec3& pos = (*is)->getViewPosition();
			const osg::Vec4& diffuseColor = (*is)->getDiffuseColor();
			const osg::Vec4& ambientColor = (*is)->getAmbientColor();

			*p++ = pos.x();						// r
			*p++ = pos.y();						// g
			*p++ = pos.z();						// b
			*p++ = diffuseColor.r();			// a
			*p++ = diffuseColor.g();			// r
			*p++ = diffuseColor.b();			// g
			*p++ = diffuseColor.a();			// b
			*p++ = (*is)->getConstantAtt();		// a
			*p++ = (*is)->getLinearAtt();		// r
			*p++ = (*is)->getQuadraticAtt();	// g
			*p++ = (*is)->isEnabled()?1:0;		// b
			*p++ = (*is)->getId();				// a
			*p++ = ambientColor.r();			// r
			*p++ = ambientColor.g();			// g
			*p++ = ambientColor.b();			// b
			*p++ = ambientColor.a();			// a
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;

			++is;
		}
	}

	///////////////////////////////////////////////////////////////
	// spot light
	///////////////////////////////////////////////////////////////

	{
		VEC_LIGHT_SPOT::const_iterator is = m_lightSpot.begin();
		VEC_LIGHT_SPOT::const_iterator ie = m_lightSpot.end();
		while ( is!=ie )
		{
			const osg::Vec3& pos = (*is)->getViewPosition();
			const osg::Vec4& diffuseColor = (*is)->getDiffuseColor();
			const osg::Vec4& ambientColor = (*is)->getAmbientColor();

			*p++ = pos.x();						// r
			*p++ = pos.y();						// g
			*p++ = pos.z();						// b
			*p++ = diffuseColor.r();			// a
			*p++ = diffuseColor.g();			// r
			*p++ = diffuseColor.b();			// g
			*p++ = diffuseColor.a();			// b
			*p++ = (*is)->getConstantAtt();		// a
			*p++ = (*is)->getLinearAtt();		// r
			*p++ = (*is)->getQuadraticAtt();	// g
			*p++ = (*is)->isEnabled()?1:0;		// b
			*p++ = (*is)->getId();				// a
			*p++ = ambientColor.r();			// r
			*p++ = ambientColor.g();			// g
			*p++ = ambientColor.b();			// b
			*p++ = ambientColor.a();			// a

			// spot stuff here
			const osg::Vec3& dir = (*is)->getViewMatrixDirection();
			float cutoff = (float)cos( osg::DegreesToRadians((*is)->getCutOff()) );
	
			*p++ = dir.x();						// r
			*p++ = dir.y();						// g
			*p++ = dir.z();						// b
			*p++ = cutoff;						// a

			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;
			*p++;


			++is;
		}
	}


	m_rImage->dirty();

	m_uniformCountLightPoints->set( (int)m_lightPoint.size() );	// set the uniform to indicate how many light points we have
	m_uniformCountLightSpots->set( (int)m_lightSpot.size() );	// set the uniform to indicate how many light points we have
}

void CSulLightManager::traverse( osg::NodeVisitor& nv )
{
	if ( nv.getVisitorType()==osg::NodeVisitor::CULL_VISITOR )
	{
		// if we get to here.. then it is because the light is visible in the scene for this camera
		osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor*>(&nv);

		// we create a double buffer, so we don't overwrite because of threading
		sigma::int32 frame = cv->getFrameStamp()->getFrameNumber();
		if ( frame > m_lastFrame+1 )
		{
			m_countCameras = 0;
			m_lastFrame = frame;
		}

		if ( m_countCameras>=(sigma::int32)m_vecImage.size() )
		{
			createTextureData( 8, 1024 );		
			m_vecImage.push_back( m_rImage );
			m_vecTextureRectangle.push_back( m_rTextureData );
		}
	
		m_rImage = m_vecImage[ m_countCameras ];
		m_rTextureData = m_vecTextureRectangle[ m_countCameras ];

		// support forward shading
		osg::ref_ptr<osg::StateSet> ss = new osg::StateSet;
		ss->setTextureAttributeAndModes( 7, m_rTextureData, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED );
		cv->pushStateSet( ss );

		reset();
		osg::Group::traverse( nv );

		buildTextureData();

		cv->popStateSet();

		m_countCameras++;
	}
	else
	{
		osg::Group::traverse( nv );
	}
}

