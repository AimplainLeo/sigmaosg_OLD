// ShaderNormalMapTangentSpace.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulGeomQuad.h>
#include <SigmaUtil/SulShaderNormalMapTangentSpace.h>
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
 
osg::Node* CreateScene()
{
	CSulGeomQuad* p = new CSulGeomQuad;
	p->createUV();
	p->setTexture( "images/BubblyBricks-ColorMap.png", 0 );
	p->setTexture( "images/BubblyBricks-NormalMap.png", 1 );

	CSulShaderNormalMapTangentSpace* pShader = new CSulShaderNormalMapTangentSpace( p );

	return p;
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

