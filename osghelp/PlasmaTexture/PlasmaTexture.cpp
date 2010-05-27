// PlasmaTexture.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgText/Text>
#include <osg/Geometry>
#include <SigmaUtil/SulPlasma.h>
#include <SigmaUtil/SulTypes.h>

osg::Node* CreateScene( CSulPlasma* pPlasma )
{
    // create drawable geometry object
    osg::Geometry* pGeo = new osg::Geometry;

    // add 4 vertices creating a quad
    osg::Vec3Array* pVerts = new osg::Vec3Array;
    pVerts->push_back( osg::Vec3( 0, 0, 0 ) );
    pVerts->push_back( osg::Vec3( 1, 0, 0 ) );
    pVerts->push_back( osg::Vec3( 1, 0, 1 ) );
    pVerts->push_back( osg::Vec3( 0, 0, 1 ) );
    pGeo->setVertexArray( pVerts );

    // create a primitive set
    osg::DrawElementsUInt* pPrimitiveSet = new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 );
    pPrimitiveSet->push_back( 3 );
    pPrimitiveSet->push_back( 2 );
    pPrimitiveSet->push_back( 1 );
    pPrimitiveSet->push_back( 0 );
    pGeo->addPrimitiveSet( pPrimitiveSet );

    // create an arraw for texture coordinates
    osg::Vec2Array* pTexCoords = new osg::Vec2Array( 4 );
    (*pTexCoords)[0].set( 0.0f, 0.0f );
    (*pTexCoords)[1].set( 1.0f, 0.0f );
    (*pTexCoords)[2].set( 1.0f, 1.0f );
    (*pTexCoords)[3].set( 0.0f, 1.0f );
    pGeo->setTexCoordArray( 0, pTexCoords );

	// create color array data 
    osg::Vec4Array* pColors = new osg::Vec4Array;
    pColors->push_back( osg::Vec4( 1.0f, 0.0f, 0.0f, 1.0f ) );
    pColors->push_back( osg::Vec4( 0.0f, 1.0f, 0.0f, 1.0f ) );
    pColors->push_back( osg::Vec4( 0.0f, 0.0f, 1.0f, 1.0f ) );
	pColors->push_back( osg::Vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );
    pGeo->setColorArray( pColors );

	// create indices to our color array
    osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4>* pColorIndices = 
        new osg::TemplateIndexArray<unsigned int, osg::Array::UIntArrayType, 4, 4>;
	pColorIndices->push_back( 3 );
    pColorIndices->push_back( 2 );
    pColorIndices->push_back( 1 );
    pColorIndices->push_back( 0 );
    pGeo->setColorIndices( pColorIndices );
    
    // make sure that our geometry is using one color per vertex
    pGeo->setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    // create geometry node that will contain all our drawables
    osg::Geode* pGeode = new osg::Geode;
    osg::StateSet* pStateSet = pGeode->getOrCreateStateSet();
    pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    pGeode->addDrawable( pGeo );

	pPlasma->Create( 256, 256 );

	osg::Image* pImage = new osg::Image;
	pImage->setImage(
		256, 256, 1,												// 1=r? depth perhaps?
		GL_ALPHA,													// internal format
        GL_ALPHA, GL_UNSIGNED_BYTE,									// pixelformat, type
        reinterpret_cast<unsigned char*>(pPlasma->GetDataPtr()),	// data
        osg::Image::NO_DELETE, 
        1 );														// packing

	osg::Texture2D* pTex = new osg::Texture2D;
	pTex->setImage( pImage );
	pStateSet->setTextureAttributeAndModes( 0, pTex, osg::StateAttribute::ON );

	pStateSet->setMode( GL_BLEND, osg::StateAttribute::ON );

    return pGeode;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    viewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
	osg::ref_ptr<CSulPlasma> rPlasma = new CSulPlasma;
    viewer->setSceneData( CreateScene(rPlasma.get()) );

    // execute main loop
    return viewer->run();
}
