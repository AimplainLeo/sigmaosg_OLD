// UpdateCallbackCustom.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>

class CMyUpdateCallback : public osg::NodeCallback
{
public:
	void operator()( osg::Node* node, osg::NodeVisitor* nv )
	{
		osg::MatrixTransform* pMatTrans = dynamic_cast<osg::MatrixTransform*>(node);
		if ( pMatTrans )
		{
			// rotate
			double t = osg::Timer::instance()->time_s();
			osg::Matrix m( osg::Quat( t, osg::Vec3(0,0,1) ) );
			pMatTrans->setMatrix( m );
		}

		// traverse subtree
		traverse(node,nv);
	}

};

osg::Node* CreateScene()
{
	osg::Geode* pGeode = new osg::Geode();

	// create simple box shape
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,0.0f),2.0f) ) );

	// create a matrixtransform so we can transform the box
	osg::MatrixTransform* pMatTrans = new osg::MatrixTransform;
	pMatTrans->addChild( pGeode );

	// callback 
	pMatTrans->setUpdateCallback( new CMyUpdateCallback );

	return pMatTrans;
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