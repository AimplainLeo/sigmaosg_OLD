// HeightField.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <SigmaUtil/SulPlasma.h>
 
osg::Node* CreateScene()
{
	int x = 256;
	int y = 256;
	osg::ref_ptr<CSulPlasma> rPlasma = new CSulPlasma;
	rPlasma->Create( x, y );

    osg::Geode* pGeode = new osg::Geode();

	osg::HeightField* pHeightField = new osg::HeightField;
    pHeightField->allocate( x, y );
    pHeightField->setXInterval( 8.0f );
    pHeightField->setYInterval( 8.0f );

	for ( int yy=0; yy<y; yy++ )
	{
		for ( int xx=0; xx<x; xx++ )
		{
			pHeightField->setHeight( xx, yy, rPlasma->GetValue( xx, yy ) );
		}
	}

	pGeode->addDrawable( new osg::ShapeDrawable(pHeightField) );

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
