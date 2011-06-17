// SulScreenAlignedQuad.cpp

#include "stdafx.h"
#include "SulScreenAlignedQuad.h"
#include <osgDB/ReadFile>

CSulScreenAlignedQuad::CSulScreenAlignedQuad( const osg::Vec3& vPos, float w, float h, float fViewW, float fViewH )
{
	// create geometry quad
	m_rGeomQuad = new CSulGeomQuad( vPos, w, h, CSulGeomQuad::PLANE_XY );

	m_rGeomQuad->getOrCreateStateSet()->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF );
	
    // create geometry node that will contain all our drawables
    m_rGroup = new osg::Group;
    osg::StateSet* pStateSet = m_rGroup->getOrCreateStateSet();
    pStateSet->setMode( GL_LIGHTING, osg::StateAttribute::OFF );
    m_rGroup->addChild( m_rGeomQuad );
	
	m_rMT = new osg::MatrixTransform;
	m_rMT->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	m_rMT->setMatrix( osg::Matrix::identity() );
	m_rMT->addChild( m_rGroup );
	
	m_rProj = new osg::Projection();
	osg::Matrixd mOrtho = osg::Matrix::ortho2D( 0, fViewW, 0, fViewH );
	m_rProj->addChild( m_rMT.get() );
	m_rProj->setMatrix( mOrtho );
}

osg::Projection* CSulScreenAlignedQuad::getProjection()
{
	return m_rProj;
}

osg::Group* CSulScreenAlignedQuad::getGroup()
{
	return m_rGroup;
}

CSulGeomQuad* CSulScreenAlignedQuad::getQuad()
{
	return m_rGeomQuad;
}

void CSulScreenAlignedQuad::setTexture( const std::string& sFile, GLint internalFormat )
{
	osg::Image* pImage = osgDB::readImageFile( sFile );
	setTexture( pImage, internalFormat );
}

void CSulScreenAlignedQuad::setTexture( osg::Texture* pTex )
{
	m_rGeomQuad->setTexture( pTex );
}

void CSulScreenAlignedQuad::setTexture( osg::Image* pImage, GLint internalFormat )
{
	m_rGeomQuad->setTexture( pImage, internalFormat );
}