// Switch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
 
class CSwitchUpdate : public osg::NodeCallback
{
public:
	virtual void operator() ( osg::Node* pNode, osg::NodeVisitor* nv )
	{
		int i = ((int)osg::Timer::instance()->time_s()%3);

		osg::Switch* pSwitch = dynamic_cast<osg::Switch*>(pNode);

		// set dimmed 
		pSwitch->setValue( 0, i==0?false:true ); 
		pSwitch->setValue( 1, i==1?false:true ); 
		pSwitch->setValue( 2, i==2?false:true ); 
		
		// set bright
		pSwitch->setValue( 3, i==0?true:false );
		pSwitch->setValue( 4, i==1?true:false );
		pSwitch->setValue( 5, i==2?true:false );
	}
};

osg::Geode* CreateSphere( float z_ofs, const osg::Vec4& vColor )
{
   osg::Geode* pGeode = new osg::Geode();

	osg::ShapeDrawable* pDraw = new osg::ShapeDrawable( new osg::Sphere( osg::Vec3(0.0f,0.0f,z_ofs), 0.5f ) );
	pDraw->setColor( vColor );
    pGeode->addDrawable( pDraw );

	return pGeode;
}

osg::Node* CreateScene()
{
	// create a switch node
	osg::Switch* pSwitch = new osg::Switch;

	// create stop lights (dimmed)
	pSwitch->addChild( CreateSphere( 2.0f, osg::Vec4(0.5f,0,0,1) ) );
	pSwitch->addChild( CreateSphere( 1.0f, osg::Vec4(0.5f,0.5f,0,1) ) );
	pSwitch->addChild( CreateSphere( 0.0f, osg::Vec4(0,0.5f,0,1) ) );

	// create stop lights (bright)
	pSwitch->addChild( CreateSphere( 2.0f, osg::Vec4(1,0,0,1) ) );
	pSwitch->addChild( CreateSphere( 1.0f, osg::Vec4(1,1,0,1) ) );
	pSwitch->addChild( CreateSphere( 0.0f, osg::Vec4(0,1,0,1) ) );

	pSwitch->setUpdateCallback( new CSwitchUpdate );

    return pSwitch;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    viewer->setSceneData( CreateScene() );

    // execute main loop
    return viewer->run();
}
