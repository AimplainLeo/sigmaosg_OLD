// UpdateCallbackMaterialFade.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/Material>
#include <osg/BlendFunc>
#include <osgDB/FileUtils>

class CIuMaterialFaderCallback : public osg::NodeCallback
{
public:
	CIuMaterialFaderCallback(osg::Material* pMaterial);

	void SetVisible(bool visible) { m_bVisible = visible; }
	void SetFadeTime(float seconds);

	bool IsVisible() const { return m_bVisible; }

	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

private:
	bool m_bVisible;
	osg::Material* m_pMaterial;
	osg::Timer_t m_LastTime;
	float m_LastAlpha;
	float m_Speed;
};

CIuMaterialFaderCallback::CIuMaterialFaderCallback( osg::Material* pMaterial ):
m_pMaterial(pMaterial),
m_bVisible(false), 
m_LastAlpha(0.0f), 
m_Speed(1.0f)
{
	m_LastTime = osg::Timer::instance()->tick();
}


void CIuMaterialFaderCallback::SetFadeTime( float seconds )
{
	if ( seconds > 0 )
	{
		m_Speed = 1.0f / seconds;
	}
}

void CIuMaterialFaderCallback::operator()( osg::Node* node, osg::NodeVisitor* nv )
{
	osg::Timer_t now = osg::Timer::instance()->tick();
	double deltaT = osg::Timer::instance()->delta_s(m_LastTime, now);

	float alpha = m_LastAlpha;
	if ( m_bVisible )
	{
		alpha = m_LastAlpha + deltaT * m_Speed;
	} 
	else
	{
		alpha = m_LastAlpha - deltaT * m_Speed;
	}
	alpha = osg::clampBetween(alpha, 0.0f, 1.0f);
	m_pMaterial->setAlpha(osg::Material::FRONT_AND_BACK, alpha);

	m_LastAlpha = alpha;
	m_LastTime = now;

	traverse(node, nv);
}


osg::Node* CreateScene()
{
    // load model
	osg::Node* pLoadedModel = osgDB::readNodeFile(  osgDB::findDataFile( "LandRover.3DS" ) );

	osg::Material* pMaterial = new osg::Material;
	pMaterial->setAlpha( osg::Material::FRONT_AND_BACK, 0.0f );
	pLoadedModel->getOrCreateStateSet()->setAttribute( pMaterial, osg::StateAttribute::OVERRIDE );

	// enable blending
	pLoadedModel->getOrCreateStateSet()->setMode( GL_BLEND, osg::StateAttribute::ON );

	// set blending function
	osg::BlendFunc *pBlendFunc = new osg::BlendFunc();
	pBlendFunc->setFunction(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA); 
	pLoadedModel->getOrCreateStateSet()->setAttributeAndModes( pBlendFunc );

	CIuMaterialFaderCallback* p = new CIuMaterialFaderCallback( pMaterial );
	p->SetVisible( true );
	p->SetFadeTime( 48 );
	pLoadedModel->setUpdateCallback( p );

    return pLoadedModel;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

	viewer->getCamera()->setClearColor( osg::Vec4(0.8,0.8,0.8,1) );

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( CreateScene() );

    // execute main loop
    return viewer->run();
}