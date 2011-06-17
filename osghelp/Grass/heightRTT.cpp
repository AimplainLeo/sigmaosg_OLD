// heightRTT.cpp

#include "stdafx.h"
#include "heightRTT.h"
#include "camsync.h"
#include <SigmaUtil/SulGeomQuad.h>
#include <SigmaUtil/SulTransScreenAlign.h>
#include <osg/ref_ptr>
#include <osg/camera>
#include <osg/geode>
#include <osg/projection>

class CKeyboardHandler : public osgGA::GUIEventHandler 
{
public:
	CKeyboardHandler( osg::Camera* pCam )
		: osgGA::GUIEventHandler()
	{
		m_rCam = pCam;
		m_last_t = 0.0;
		dx = 0.0f;
		dy = 0.0f;
	}
	
	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
    {
        if ( ea.getEventType()==osgGA::GUIEventAdapter::KEYDOWN )
        {
            if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Right )	dx = 1.0f;
			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Left )	dx = -1.0f;
            if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Up )		dy = 1.0f;
			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Down )	dy = -1.0f;

			return false; // return true, event handled
		}

		if ( ea.getEventType()==osgGA::GUIEventAdapter::KEYUP )
        {
            if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Right )	dx = 0.0f;
			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Left )	dx = 0.0f;
            if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Up )		dy = 0.0f;
			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Down )	dy = 0.0f;
            
			return false; // return true, event handled
		}

		if ( ea.getEventType()==osgGA::GUIEventAdapter::FRAME )
		{
			if ( m_last_t==0.0 )
			{
				m_last_t = ea.getTime();
				return true;
			}

			double t = ea.getTime();
			double dt = t-m_last_t;
			m_last_t = ea.getTime();

			osg::Matrix m = m_rCam->getViewMatrix();
			m = m*osg::Matrix::translate( osg::Vec3(dt*dx*10.0f,dt*dy*10.0f,0 ) );
			m_rCam->setViewMatrix( m );
			
			return true;
		}
        
        return false;
    }

private:
	osg::ref_ptr<osg::Camera>	m_rCam;
	double						m_last_t;
	bool						m_bRight;
	float						dx;
	float						dy;
};

void CHeightRTT::init( osgViewer::Viewer* pViewer, osgText::Text* pTextCam, osg::Camera* pMainCam, osg::Group* pGroup, osg::Node* pRenderMe )
{
	m_pTextCam = pTextCam;
	m_pMainCam = pMainCam;

//	rRTT = new CSulRTT( 256, 256, GL_LUMINANCE32F_ARB, GL_FLOAT ); 
	rRTT = new CSulRTT( 1024, 1024, GL_LUMINANCE, GL_FLOAT ); 

	osg::ref_ptr<CKeyboardHandler> rKeyboardHandler = new CKeyboardHandler( rRTT );
	pViewer->addEventHandler( rKeyboardHandler );

	rRTT->getTexture()->setInternalFormat( GL_LUMINANCE32F_ARB );
	rRTT->getTexture()->setSourceType( GL_FLOAT );
	rRTT->getTexture()->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::NEAREST);
	rRTT->getTexture()->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::NEAREST);

	rRTT->setClearColor( osg::Vec4(0,0,0,1) );
	pGroup->addChild( rRTT );
	
	// quad to show rendered texture on screen (for debugging)
	/*
	osg::ref_ptr<CSulGeomQuad> rQuad = new CSulGeomQuad( osg::Vec3(128,128,0), 256, 256 );
	osg::ref_ptr<CSulTransScreenAlign> rAlign = new CSulTransScreenAlign( 800, 600 );
	osg::Geode* pGeodeQuadScreen = new osg::Geode;
	pGeodeQuadScreen->addDrawable( rQuad->GetGeometry() );
	rAlign->AddChild( pGeodeQuadScreen );
	pGroup->addChild( rAlign->GetProjection() );
	rQuad->SetTexture( rRTT->GetTexture() );
	rQuad->GetGeometry()->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED | osg::StateAttribute::OVERRIDE );
*/

	// cc = 40			cc*cc
	// sp = 0.2
	// need to ortho camera
	float l = 8.0f/2.0f;
	float r = -l;
	float b = l;
	float t = -l;
	rRTT->setProjectionMatrixAsOrtho( l, r, b, t, 0.1f, 1000000.0f  );

	// RTT camera should follow main camera
	osg::Matrix mViewMatrix;
	float m[16] =
	{
		 0, 1, 0, 0,
		-1, 0, 0, 0,
		 0, 0, 1, 0,
		 0, 0,-100, 1
	};
	mViewMatrix.set( m );
	rRTT->setViewMatrix( mViewMatrix );

	////////////////////////////////////////////////////////////////////
	// shader to make the render height instead of texture
	////////////////////////////////////////////////////////////////////

	osg::Group* pGroupRenderMe = new osg::Group;

	pGroupRenderMe->addChild( pRenderMe );
	getCamera()->addChild( pGroupRenderMe );
	osg::StateSet* ss = pGroupRenderMe->getOrCreateStateSet();

	osg::ref_ptr< osg::Shader > vertexShader = new osg::Shader();
    vertexShader->setType( osg::Shader::VERTEX );
    vertexShader->loadShaderSourceFromFile( "height_render.vert" );

	osg::ref_ptr< osg::Shader > fragShader = new osg::Shader();
    fragShader->setType( osg::Shader::FRAGMENT );
    fragShader->loadShaderSourceFromFile( "height_render.frag" );

	osg::ref_ptr< osg::Program > program = new osg::Program();
    program->addShader( vertexShader.get() );
	program->addShader( fragShader.get() );

	osg::Uniform* uniformInverseViewMatrix = new osg::Uniform( "viewInverseMatrixCam", rRTT->getInverseViewMatrix() );
    ss->addUniform( uniformInverseViewMatrix );

	ss->setAttribute( program.get(), osg::StateAttribute::ON | osg::StateAttribute::PROTECTED );
}

void CHeightRTT::setOrtho( float cc, float sp )
{
	float tmp = cc*sp;
	// cc = 40			cc*cc
	// sp = 0.2
	// need to ortho camera
	float l = tmp/2.0f;
	float r = -l;
	float b = l;
	float t = -l;
	rRTT->setProjectionMatrixAsOrtho( l, r, b, t, 0.1f, 1000000.0f  );
}

osg::Camera* CHeightRTT::getCamera()
{
	return rRTT;
}

osg::Texture2D*	CHeightRTT::getTexture()
{
	return rRTT->getTexture();
}

void CHeightRTT::setLock( bool bLock )
{
	if ( bLock )
	{
		rRTT->setUpdateCallback( new CCamSync( m_pTextCam, m_pMainCam, CSulNodeCallbackCameraSync::MODE_POSITION_XY ) );
	}
	else
	{
		rRTT->setUpdateCallback( 0 );
	}
}
