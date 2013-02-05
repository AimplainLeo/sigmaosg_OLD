// SulScreenAlignedQuad.cpp

#include "stdafx.h"
#include "SulScreenAlignedQuad.h"
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>

CSulScreenAlignedQuad::CSulScreenAlignedQuad()
{
	setName( "CSulScreenAlignedQuad" );

	m_rGeomQuad = new CSulGeomQuad;

	m_geodeQuad = new CSulGeode;
	m_geodeQuad->addDrawable( m_rGeomQuad );

	osg::Matrixd mOrtho = osg::Matrix::ortho2D( -0.5f, 0.5f, -0.5f, 0.5f );
	setMatrix( mOrtho );

	initConstructor();
}

CSulScreenAlignedQuad::CSulScreenAlignedQuad( float fViewW, float fViewH, const CSulString& fileTexture )
{
	setName( "CSulScreenAlignedQuad" );

	osg::ref_ptr<osg::Image> image = osgDB::readImageFile( osgDB::findDataFile(fileTexture.c_str()) );

	float x = 0.0f;
	float y = 0.0f;
	float w = image->s();
	float h = image->t();

	m_rGeomQuad = new CSulGeomQuad( osg::Vec3( x + w/2.0f, fViewH - (y+h/2.0f) , 0 ), w, h, CSulGeomQuad::PLANE_XY );
	m_rGeomQuad->setTexture( image );

	m_geodeQuad = new CSulGeode;
	m_geodeQuad->addDrawable( m_rGeomQuad );

	osg::Matrixd mOrtho = osg::Matrix::ortho2D( 0, fViewW, 0, fViewH );
	setMatrix( mOrtho );

	initConstructor();
}

CSulScreenAlignedQuad::CSulScreenAlignedQuad( float fViewW, float fViewH, osg::Texture2D* tex, sigma::int32 x, sigma::int32 y, sigma::uint32 w, sigma::uint32 h )
{
	// create geometry quad
	m_rGeomQuad = new CSulGeomQuad( osg::Vec3( x + w/2.0f, fViewH - (y+h/2.0f) , 0 ), w, h, CSulGeomQuad::PLANE_XY );
	m_rGeomQuad->setTexture( tex );

	m_geodeQuad = new CSulGeode;
	m_geodeQuad->addDrawable( m_rGeomQuad );

	osg::Matrixd mOrtho = osg::Matrix::ortho2D( 0, fViewW, 0, fViewH );
	setMatrix( mOrtho );

	initConstructor();
}

CSulScreenAlignedQuad::CSulScreenAlignedQuad( float fViewW, float fViewH, osg::Texture2D* tex, const osg::Vec3& pos, sigma::uint32 w, sigma::uint32 h )
{
	float x = pos.x();
	float y = pos.y();
	float z = pos.z();

	// create geometry quad
	m_rGeomQuad = new CSulGeomQuad( osg::Vec3( x + w/2.0f, fViewH - (y+h/2.0f) , z ), w, h, CSulGeomQuad::PLANE_XY );
	m_rGeomQuad->setTexture( tex );

	m_geodeQuad = new CSulGeode;
	m_geodeQuad->addDrawable( m_rGeomQuad );

	osg::Matrixd mOrtho = osg::Matrix::ortho2D( 0, fViewW, 0, fViewH );
	setMatrix( mOrtho );

	initConstructor();
}


CSulScreenAlignedQuad::CSulScreenAlignedQuad( osgViewer::Viewer* viewer, osg::Texture2D* tex, sigma::int32 x, sigma::int32 y, sigma::uint32 w, sigma::uint32 h )
{
	setName( "CSulScreenAlignedQuad" );

	float fViewW = viewer->getCamera()->getViewport()->width();
	float fViewH = viewer->getCamera()->getViewport()->height();

	m_geodeQuad = new CSulGeode;

	// create geometry quad
	m_rGeomQuad = new CSulGeomQuad( osg::Vec3( x + w/2.0f, fViewH - (y+h/2.0f) , 0 ), w, h, CSulGeomQuad::PLANE_XY );
	m_geodeQuad->addDrawable( m_rGeomQuad );

	m_rGeomQuad->setTexture( tex );

    // create geometry node that will contain all our drawables
    m_rGroup = new osg::Group;
    osg::StateSet* pStateSet = m_rGroup->getOrCreateStateSet();
    pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    m_rGroup->addChild( m_geodeQuad );
	
	m_rMT = new osg::MatrixTransform;
	m_rMT->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	m_rMT->setMatrix( osg::Matrix::identity() );
	m_rMT->addChild( m_rGroup );
	
	osg::Matrixd mOrtho = osg::Matrix::ortho2D( 0, fViewW, 0, fViewH );
	addChild( m_rMT );
	setMatrix( mOrtho );
}

CSulScreenAlignedQuad::CSulScreenAlignedQuad( const osg::Vec3& vPos, float w, float h, float fViewW, float fViewH )
{
	setName( "CSulScreenAlignedQuad" );

	// create geometry quad
	m_rGeomQuad = new CSulGeomQuad( vPos, w, h, CSulGeomQuad::PLANE_XY );

	m_geodeQuad = new CSulGeode;
	m_geodeQuad->addDrawable( m_rGeomQuad );

	m_rGeomQuad->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	
    // create geometry node that will contain all our drawables
    m_rGroup = new osg::Group;
	m_rGroup->setName( "CSulScreenAlignedQuad -> osg::Group" );

    osg::StateSet* pStateSet = m_rGroup->getOrCreateStateSet();
    pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    m_rGroup->addChild( m_geodeQuad );
	
	m_rMT = new osg::MatrixTransform;
	m_rMT->setName( "CSulScreenAlignedQuad -> osg::MatrixTransform" );
	m_rMT->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	m_rMT->setMatrix( osg::Matrix::identity() );
	m_rMT->addChild( m_rGroup );
	
	osg::Matrixd mOrtho = osg::Matrix::ortho2D( 0, fViewW, 0, fViewH );
	addChild( m_rMT );
	setMatrix( mOrtho );
}

CSulScreenAlignedQuad::CSulScreenAlignedQuad( osgViewer::Viewer* viewer )
{
	initConstructor();
}

void CSulScreenAlignedQuad::initConstructor()
{
	setName( "CSulScreenAlignedQuad" );

    // create geometry node that will contain all our drawables
    m_rGroup = new osg::Group;

	m_rGeomQuad->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );

    osg::StateSet* pStateSet = m_rGroup->getOrCreateStateSet();
    pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    m_rGroup->addChild( m_geodeQuad );
	
	m_rMT = new osg::MatrixTransform;
	m_rMT->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	m_rMT->setMatrix( osg::Matrix::identity() );
	m_rMT->addChild( m_rGroup );

	addChild( m_rMT );
}

osg::Group* CSulScreenAlignedQuad::getGroup()
{
	return m_rGroup;
}

CSulGeode* CSulScreenAlignedQuad::getGeode()
{
	return m_geodeQuad;
}

CSulGeomQuad* CSulScreenAlignedQuad::getGeom()
{
	return m_rGeomQuad;
}

void CSulScreenAlignedQuad::setTexture( const std::string& sFile, GLint internalFormat )
{
	osg::Image* pImage = osgDB::readImageFile( osgDB::findDataFile(sFile) );
	if ( !pImage )
	{
		osg::notify(osg::ALWAYS) << "ERROR: CSulScreenAlignedQuad -> could not load texture: " << sFile << std::endl;
		return;
	}

	setTexture( pImage, internalFormat );
}

void CSulScreenAlignedQuad::setTexture( osg::Texture* pTex, sigma::uint32 unit, const CSulString& uniformName )
{
	m_rGeomQuad->setTexture( pTex, unit, uniformName );
}

void CSulScreenAlignedQuad::setTexture( osg::Image* pImage, GLint internalFormat, sigma::uint32 unit )
{
	m_rGeomQuad->setTexture( pImage, internalFormat, unit );
}