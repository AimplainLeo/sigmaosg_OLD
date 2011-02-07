// Console.cpp

#include "stdafx.h"
#include "Console.h"
#include "ParserXml.h"
#include <SigmaUtil/SulStringList.h>
#include <SigmaUtil/SulCalcBoundingBox.h>
#include <SigmaUtil/SulGenPositionTextures.h>
#include <SigmaUtil/SulGeomBoundingBox.h>
#include <SigmaUtil/SulGeomTriangleList.h>
#include <SigmaUtil/SulGeomSphere.h>
#include <osg/shapedrawable>
#include <osg/group>
#include <osg/ComputeBoundsVisitor>
#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include "GeometryInstancingBuilder.h"

extern CParserXml	xmlParser;
extern bool			bSuppressTexture;
extern std::string	outputFile;
extern float		sizeMultiplier;
extern osg::Node*	pSceneLocal;
extern sigma::uint32 texUnit;
extern bool generateCollisionGeometry;

#define SHOW_BOUNDING_BOX

CConsole::CConsole( 
	osgViewer::Viewer* pViewer,
	CSulConsoleDisplay* pConsoleDisplay, 
	osg::PositionAttitudeTransform* pScene, 
	osg::PositionAttitudeTransform* pShape )
{
	m_rViewer = pViewer;
	m_rConsole = pConsoleDisplay;

	m_rScene = pScene;
	m_rShape = pShape;

    // use a functor to get call back for commands posted in console
    CSulConsoleFunctor<CConsole>* pConsoleFunctor = new CSulConsoleFunctor<CConsole>(this, &CConsole::Command);
    pConsoleDisplay->WantCommand( pConsoleFunctor );
}

void CConsole::Command( const CSulString& sText )
{
	CSulStringList list(sText);

	CSulString sAction = list.getString( 0 );

	if ( sAction=="save" )
	{
		save();
	}

	if ( sAction=="savemyass" )
	{
		osgDB::writeNodeFile( *m_rScene, "myass.ive" );
	}

	if ( sAction=="load" )
	{
		osg::Node* pRoot = osgDB::readNodeFile( outputFile );
		m_rScene->addChild( pRoot );
	}

	// generate texture that represents positions
	if ( sAction=="gen" )
	{
		generate();
	}
}

void CConsole::save()
{
	if (!generateCollisionGeometry)
	{
		osgDB::writeNodeFile( *pRenderInstances, outputFile );
	}
	else
	{
		osg::ref_ptr<osg::Group> group = new osg::Group;
		group->addChild(pRenderInstances);
		group->addChild(m_rTreeCollisionGeometry);
		osgDB::writeNodeFile(*group, outputFile);
	}
}

void CConsole::generate()
{
	// calculate bounding box of terrain block
/*
	CSulCalcBoundingBox ccb( false );
	m_rScene->accept( ccb );
	osg::BoundingBox b = ccb.getBoundBox();
*/

	// boundingbox is in world coordinates
	osg::ComputeBoundsVisitor computeBoundsVisitor;
	pSceneLocal->accept(computeBoundsVisitor);
	osg::BoundingBox b = computeBoundsVisitor.getBoundingBox();

	osg::Polytope polyTope;
	computeBoundsVisitor.getPolytope( polyTope, 0.0f );
	
#ifdef SHOW_BOUNDING_BOX
	CSulGeomBoundingBox* gbb = new CSulGeomBoundingBox(b);
	m_rScene->addChild(gbb);
//	m_rViewer->getSceneData()->asGroup()->addChild(bbgeode);
#endif // SHOW_BOUNDING_BOX

	// create position texture
	osg::Matrix mSceneWorldToLocal;
	m_rScene->computeWorldToLocalMatrix( mSceneWorldToLocal, 0 );

	osg::Matrix mShapeLocalToWorld;
	m_rShape->computeLocalToWorldMatrix( mShapeLocalToWorld, 0 );

	osg::Matrix m;
	m = mSceneWorldToLocal*mShapeLocalToWorld;
	CSulGenPositionTextures tmp( 
		m_rScene, 
		m, 
		m_rShape, 
		b, 
		polyTope.getPlaneList(),
		xmlParser.getDataShapeMask()
	 );
	tmp.process();

	CSulString s;
	s.Format( "....done (linesegs:%i) - (pos:%i)", tmp.getCount(), tmp.getCountPos() );
	m_rConsole->DisplayText( s );
	osg::notify(osg::NOTICE) << s << std::endl;

//	showPlaceholders( tmp.getImage(), tmp.getCountPos() );
	showTrees( tmp.getImage(), tmp.getCountPos(), b, tmp.getTexSizeSquared() );

	if (generateCollisionGeometry)
	{
		m_rTreeCollisionGeometry = createTreeCollisionGeometry(tmp.getTreePositions());
		m_rTreeCollisionGeometry->setNodeMask(0x1);
		m_rTreeCollisionGeometry->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
		m_rScene->addChild(m_rTreeCollisionGeometry);
	}

	////////////////////////////////////////////////////
	// show clipped triangles
	////////////////////////////////////////////////////
	
/*
	// debug show shape triangles created
	if ( tmp.getClipTriangles().getTriangleList().size() )
	{
		CSulGeomTriangleList* pClipTri = new CSulGeomTriangleList( tmp.getClipTriangles().getTriangleList() );
		pClipTri->setColor( 1,0,0,1 );
		osg::Geode* pGeodeClipTri = new osg::Geode;
		pGeodeClipTri->getOrCreateStateSet()->setMode( GL_CULL_FACE, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED | osg::StateAttribute::OVERRIDE );
		pGeodeClipTri->addDrawable( pClipTri->getDrawable() );
		osg::ref_ptr<osg::PositionAttitudeTransform> rPat = new osg::PositionAttitudeTransform;
		rPat->setPosition( osg::Vec3(0,0,30) );
		rPat->addChild( pGeodeClipTri );
		m_rScene->addChild( rPat );
	}
*/
	////////////////////////////////////////////////////
	// show clipped masks
	////////////////////////////////////////////////////


	////////////////////////////////////////////////////
	// show rejected positions
	////////////////////////////////////////////////////

	const sigma::VEC_VEC3& rejectedPos = tmp.getRejectedPositionsList();
	osg::notify(osg::NOTICE)  << "Rejected positions count: " << rejectedPos.size() << std::endl;
/*
	// debug show rejected positions
	
	Sigma::VEC_VEC3::const_iterator i;
	i = rejectedPos.begin();
	while ( i!=rejectedPos.end() )
	{
		CSulGeomSphere* pSphere = new CSulGeomSphere( 50.5f, *i );
		osg::Geode* pg = new osg::Geode;
		pg->addDrawable( pSphere->getDrawable() );
		m_rViewer->getSceneData()->asGroup()->addChild( pg );
		++i;
	}
	
*/
	
}

osg::Geode* CConsole::createSphere( const osg::Vec3& pos )
{
	osg::Geode* pGeode = new osg::Geode;
	
	osg::TessellationHints* pHints = new osg::TessellationHints;
	pHints->setDetailRatio(0.5f);

	osg::Sphere* pSphere = new osg::Sphere( pos, 10.0 );

	osg::ShapeDrawable* pShape = new osg::ShapeDrawable( pSphere, pHints );

	pShape->setColor( osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f) );
	pGeode->addDrawable( pShape );
	
	return pGeode;
}

void CConsole::showPlaceholders( osg::Image* pImage, sigma::uint32 posCount )
{
	float* p = reinterpret_cast<float*>(pImage->data());

	while ( posCount )
	{
		osg::Vec3 pos;
		pos.x() = *(p++);
		pos.y() = *(p++);
		pos.z() = *(p++);

		pos.z() = 40.0f;

		osg::Geode* pGeode = createSphere( pos );
		m_rScene->addChild( pGeode );
		
		--posCount;
	}
}

void CConsole::showTrees( osg::Image* pImage, sigma::uint32 posCount, osg::BoundingBox& bb, sigma::uint32 texSizeSquared )
{

#if 0  // FIXME: the CSulRenderInstances has changed too much.. now this doesn't work
	pRenderInstances = new CSulRenderInstances( pImage, posCount, bb, sizeMultiplier, bSuppressTexture, texUnit, texSizeSquared );
	pRenderInstances->create();
	m_rScene->addChild( pRenderInstances );
#endif
}

osg::ref_ptr<osg::Geometry> CConsole::createSingleTreeCollisionGeometry(float s)
{
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices->push_back( osg::Vec3(-0.5f, 0.0f, 0.0f) * s );
	vertices->push_back( osg::Vec3( 0.5f, 0.0f, 0.0f) * s );
	vertices->push_back( osg::Vec3( 0.5f, 0.0f, 1.0f) * s );
	vertices->push_back( osg::Vec3(-0.5f, 0.0f, 1.0f) * s );

	vertices->push_back( osg::Vec3(0.0f, -0.5f, 0.0f) * s );
	vertices->push_back( osg::Vec3(0.0f,  0.5f, 0.0f) * s );
	vertices->push_back( osg::Vec3(0.0f,  0.5f, 1.0f) * s );
	vertices->push_back( osg::Vec3(0.0f, -0.5f, 1.0f) * s );

	osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
	geometry->setVertexArray(vertices);
    geometry->addPrimitiveSet( new osg::DrawArrays( GL_QUADS, 0, 8) );

	return geometry;
}

osg::ref_ptr<osg::Node> CConsole::createTreeCollisionGeometry(const std::vector<osg::Vec3>& positionList)
{
	GeometryInstancingBuilder builder;
	builder.setPositionList(positionList);
	builder.setInstanceGeometry(createSingleTreeCollisionGeometry(sizeMultiplier));
	return builder.build();
}