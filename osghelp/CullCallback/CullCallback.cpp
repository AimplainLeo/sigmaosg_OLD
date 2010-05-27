// CullCallback.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/matrixtransform>
#include <assert.h>

struct CMyCullback : public osg::NodeCallback 
{
	CMyCullback()
	{
	}

	virtual void operator()( osg::Node* node, osg::NodeVisitor* nv )
	{ 
		if( nv->getVisitorType() == osg::NodeVisitor::CULL_VISITOR )
		{
			osgUtil::CullVisitor* pCullVisitor = dynamic_cast<osgUtil::CullVisitor*>(nv);
			if ( pCullVisitor )
			{
				// we can assume that the node is a osg::MatrixTransform because
				// we applied the cullback callback on it!
				osg::MatrixTransform* pMatTrans = dynamic_cast<osg::MatrixTransform*>(node);

				osg::Vec3 v = pCullVisitor->getEyeLocal();

				v.normalize();
				v = v*2.0f;

				osg::Matrix m;
				m.setTrans( v );
				pMatTrans->setMatrix( m );
			}
		}
	
		traverse(node,nv);
	}
};

osg::Node* CreateScene()
{
	osg::Group* pGroup = new osg::Group;

    osg::Geode* pGeode = new osg::Geode();
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,0.0f),2.0f) ) );
	pGroup->addChild( pGeode );

	// create a small ball (would like this to be transformable)
	pGeode = new osg::Geode();
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),0.1f) ) );
	osg::MatrixTransform* pMatTrans = new osg::MatrixTransform;
    pMatTrans->addChild( pGeode );
	pGroup->addChild( pMatTrans );

	pMatTrans->setCullCallback( new CMyCullback );

    return pGroup;
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

