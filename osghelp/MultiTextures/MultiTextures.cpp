// MultiTextures.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osg/TexEnvCombine>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osg/Geometry>

osg::Geode* CreateQuad( const osg::Vec3& vPos )
{
    // create drawable geometry object
    osg::Geometry* pGeo = new osg::Geometry;

    // add 4 vertices creating a quad
    osg::Vec3Array* pVerts = new osg::Vec3Array;
    pVerts->push_back( osg::Vec3( 0, 0, 0 )+vPos );
    pVerts->push_back( osg::Vec3( 1, 0, 0 )+vPos );
    pVerts->push_back( osg::Vec3( 1, 0, 1 )+vPos );
    pVerts->push_back( osg::Vec3( 0, 0, 1 )+vPos );
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
	pGeo->setTexCoordArray( 1, pTexCoords );

    // create geometry node that will contain all our drawables
    osg::Geode* pGeode = new osg::Geode;
	pGeode->addDrawable( pGeo );

	return pGeode;
}

osg::Node* CreateScene()
{
	osg::Group* pGroup = new osg::Group;

	// load texture of a pencil (contains alpha)
    osg::Texture2D* pTexPencil = new osg::Texture2D;
    pTexPencil->setImage( osgDB::readImageFile( "pencil.tga" ) );

	// load texture of cracks (no alpha)
    osg::Texture2D* pTexCracks = new osg::Texture2D;
    pTexCracks->setImage( osgDB::readImageFile( "cracks.tga" ) );

	// create quad on left side
	osg::Geode* pGeode0 = CreateQuad( osg::Vec3(-0.5,0,0) );
	pGroup->addChild( pGeode0 );
    osg::StateSet* pStateSet0 = pGeode0->getOrCreateStateSet();
    pStateSet0->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	pStateSet0->setMode( GL_BLEND, osg::StateAttribute::ON );
    pStateSet0->setTextureAttributeAndModes( 0, pTexPencil, osg::StateAttribute::ON );
    pStateSet0->setTextureAttributeAndModes( 1, pTexCracks, osg::StateAttribute::ON );

	// add combiner, setup so it multiplies texture0 with texture1
	osg::TexEnvCombine* pTexEnv0 = new osg::TexEnvCombine;
    pTexEnv0->setCombine_RGB( osg::TexEnvCombine::MODULATE );		
    pTexEnv0->setSource0_RGB( osg::TexEnvCombine::TEXTURE0 );		
	pTexEnv0->setOperand0_RGB( osg::TexEnvCombine::SRC_COLOR );		
	pTexEnv0->setSource1_RGB( osg::TexEnvCombine::TEXTURE1 );		
	pTexEnv0->setOperand1_RGB( osg::TexEnvCombine::SRC_COLOR );		
    pStateSet0->setTextureAttribute( 1, pTexEnv0 );

	// create quad on right side
	osg::Geode* pGeode1 = CreateQuad( osg::Vec3(0.5,0,0) );
	pGroup->addChild( pGeode1 );
    osg::StateSet* pStateSet1 = pGeode1->getOrCreateStateSet();
    pStateSet1->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
	pStateSet1->setMode( GL_BLEND, osg::StateAttribute::ON );
    pStateSet1->setTextureAttributeAndModes( 0, pTexPencil, osg::StateAttribute::ON );
    pStateSet1->setTextureAttributeAndModes( 1, pTexCracks, osg::StateAttribute::ON );

	// add combiner, setup so we draw texture1 (cracks)
	// and invert the alpha in texture0 (pencil) 
	osg::TexEnvCombine* pTexEnv1 = new osg::TexEnvCombine;
    pTexEnv1->setCombine_RGB( osg::TexEnvCombine::REPLACE );			
    pTexEnv1->setSource0_RGB( osg::TexEnvCombine::TEXTURE1 );		
	pTexEnv1->setOperand0_RGB( osg::TexEnvCombine::SRC_COLOR );		
	pTexEnv1->setCombine_Alpha( osg::TexEnvCombine::MODULATE );		
	pTexEnv1->setSource1_Alpha( osg::TexEnvCombine::TEXTURE0 );		
	pTexEnv1->setOperand1_Alpha( osg::TexEnvCombine::ONE_MINUS_SRC_ALPHA );
	pStateSet1->setTextureAttribute( 1, pTexEnv1 );

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

