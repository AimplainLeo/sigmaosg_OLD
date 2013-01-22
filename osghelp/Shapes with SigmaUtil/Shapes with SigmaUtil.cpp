// Shapes with SigmaUtil.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SigmaUtil/SulGeode.h>
#include <SigmaUtil/SulGeomHemiSphere.h>
#include <osgViewer/Viewer>

#include <osg/material>
#include <osg/lightmodel>

osg::Node* createScene()
{
	osg::Group* group = new osg::Group;

	group->addChild( new CSulGeode(
		new CSulGeomHemiSphere(1,3,2)
	));


	group->getOrCreateStateSet()->setMode( GL_CULL_FACE, osg::StateAttribute::OFF );

osg::Material* m = new osg::Material;  
m->setColorMode(osg::Material::DIFFUSE);  
m->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0.1, 0, 1));  
m->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 1, 0, 1));  
m->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0.1, 0, 1));  
m->setShininess(osg::Material::FRONT_AND_BACK, 1.0f);  
group->getOrCreateStateSet()->setAttributeAndModes( m, osg::StateAttribute::ON );  
osg::LightModel* ltModel = new osg::LightModel;  
ltModel->setTwoSided(true);  
group->getOrCreateStateSet()->setAttribute(ltModel); 


	return group;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

	// set the scene-graph data the viewer will render
	osg::Node* pScene = createScene();
  
	viewer->setSceneData( pScene );

	// execute main loop
	return viewer->run();
}
