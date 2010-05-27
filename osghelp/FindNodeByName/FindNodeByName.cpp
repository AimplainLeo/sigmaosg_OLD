// FindNodeByName.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/animationpath>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
 
osg::Node* CreateBox( const osg::Vec3& vPos, const std::string& sName, float fSize=1.0f )
{
	osg::Geode* pGeode = new osg::Geode();
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(vPos,fSize) ) );
	
	osg::MatrixTransform* pMatTrans = new osg::MatrixTransform;
	pMatTrans->setName( sName );
	pMatTrans->addChild( pGeode );
	
	return pMatTrans;
}

osg::Node* CreateScene()
{
	osg::Group* pGroup = new osg::Group;
	pGroup->addChild( CreateBox( osg::Vec3(-2,0,0), "box0" ) );
	pGroup->addChild( CreateBox( osg::Vec3(0,0,0), "box1" ) );
	pGroup->addChild( CreateBox( osg::Vec3(2,0,0), "box2" ) );
    return pGroup;
}

osg::Node* FindNodeByName( osg::Node* pNode, const std::string& sName )
{
	if ( pNode->getName()==sName )
	{
		return pNode;
	}

	osg::Group* pGroup = pNode->asGroup();
	if ( pGroup )
	{
		for ( unsigned int i=0; i<pGroup->getNumChildren(); i++ )
		{
			osg::Node* pFound = FindNodeByName( pGroup->getChild(i), sName );
			if ( pFound )
			{
				return pFound;
			}
		}
	}

	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 512, 512 );

	// create scene
	osg::Node* pScene = CreateScene();

	osg::MatrixTransform* p = dynamic_cast<osg::MatrixTransform*>(FindNodeByName( pScene, "box1" ));
	p->setUpdateCallback( new osg::AnimationPathCallback(osg::Vec3(0, 0, 0), osg::Z_AXIS, osg::inDegrees(45.0f)) );
	
    // set the scene-graph data the viewer will render
    rViewer->setSceneData( pScene  );

    // execute main loop
    return rViewer->run();
}