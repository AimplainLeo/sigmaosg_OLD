// CustomTextureFloat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>

osg::Node* CreateScene()
{
    osg::Geode* pGeode = new osg::Geode();

    // we create the simplest form of shapes in OpenSceneGraph
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Capsule(osg::Vec3(8.0f,0.0f,1.0f),1.5f,3.0f) ) );

	int s = 64;
	float* pData = new float[s*s*3];
	for ( int y=0; y<s; y++ )
		for ( int x=0; x<s; x++ )
		{
			unsigned char  c = (( ((y&0x8)==0) ^ (((x&0x8))==0) ))*1;

			pData[x+y*s] = c;
 		}

	osg::Image* pImage = new osg::Image;
	pImage->setImage(
		s, s, 1,								// 1=r? depth perhaps?
		GL_LUMINANCE,							// internal format
        GL_LUMINANCE,							// pixelformat
		GL_FLOAT,								// type
        (unsigned char*)pData,					// data
        osg::Image::USE_NEW_DELETE,				// mode
        1 );									// packing

	osg::Texture2D* pTex = new osg::Texture2D;
	pTex->setImage( pImage );

    osg::StateSet* pStateSet = pGeode->getOrCreateStateSet();
	pStateSet->setTextureAttributeAndModes( 0, pTex, osg::StateAttribute::ON );

    return pGeode;
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
