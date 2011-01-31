// Articulated.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osg/animationpath>
#include <osgDB/FileUtils>
#include <iostream>

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

// will add a matrixtransform to pNode (does not handle multipal parents!)
osg::MatrixTransform* AddMatrixTransform( osg::Node* pNode )
{
	// parent must derive from osg::Group
	osg::Group* pGroup = pNode->getParent(0)->asGroup();
	if ( pGroup )
	{
		// make sure we have a reference count at all time!
		osg::ref_ptr<osg::Node> pNodeTmp = pNode;

		// remove pNode from parent
		pGroup->removeChild( pNodeTmp.get() );

		// create matrixtransform and do connections
		osg::MatrixTransform* pMT = new osg::MatrixTransform;
		pMT->addChild( pNodeTmp.get() );
		pGroup->addChild( pMT );
		return pMT;
	}

	return 0;
}

osg::Node* createScene()
{
	// load our model
	std::string file = osgDB::findDataFile( "htest.3ds" );
	if ( file.empty() )
		return 0;
	
	osg::Node* pModel =  osgDB::readNodeFile( file );

	// find the node for the cannon
	osg::Node* pFound = FindNodeByName( pModel, "LOD01_g6_c" );

	// add a matrixtransform to the cannon
	osg::MatrixTransform* pMT = AddMatrixTransform( pFound );

	// create simple animation path to demostrate transformation
	osg::ref_ptr<osg::AnimationPath> rPath = new osg::AnimationPath;
	rPath->setLoopMode( osg::AnimationPath::SWING );
	osg::AnimationPath::ControlPoint c0(osg::Vec3(0,0,0), osg::Quat(0, osg::Vec3(1,0,0)) );
	osg::AnimationPath::ControlPoint c1(osg::Vec3(0,0,0), osg::Quat(osg::PI/6.0f, osg::Vec3(1,0,0))); 
	rPath->insert( 0.0f, c0 );
	rPath->insert( 4.0f, c1 );
	osg::ref_ptr<osg::AnimationPathCallback> rAniCallback = new osg::AnimationPathCallback( rPath.get() );
	pMT->setUpdateCallback( rAniCallback.get() );


	return pModel;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
	osg::Node* pNode = createScene();
	if ( pNode )
	{
		rViewer->setSceneData( pNode );
	}
	else
	{
		std::cout << "WARNING: scene not created. Perhaps a missing file" << std::endl;
	}

    // execute main loop
    return rViewer->run();
}