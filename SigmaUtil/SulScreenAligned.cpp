// SulScreenAligned.cpp

#include "stdafx.h"
#include "SulScreenAligned.h"

CSulScreenAligned::CSulScreenAligned( sigma::uint32 viewW, sigma::uint32 viewH ) :
osg::Projection(),
m_viewW( viewW ),
m_viewH( viewH ),
m_alignment( CSulScreenAligned::CENTER_CENTER )
{
	initConstructor();
}

void CSulScreenAligned::initConstructor()
{
	m_mt = new osg::MatrixTransform;
	m_mt->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	setPosition( osg::Vec3( 0, 0, 0 ) );
	
	osg::Matrixd mOrtho = osg::Matrix::ortho2D( 0, m_viewW, 0, m_viewH );
	setMatrix( mOrtho );
	addChild( m_mt );
}

osg::MatrixTransform* CSulScreenAligned::getMatrixTransform()
{
	return m_mt;
}

void CSulScreenAligned::setPosition( const osg::Vec3& pos )
{
	osg::Vec3 newPos = pos;

	switch ( m_alignment )
	{
		case CENTER_CENTER:
			newPos.x() += m_viewW/2;
			newPos.y() += m_viewH/2;
			break;

		default:
			assert( 0 ); // alignment not support yet
	}

	osg::Matrix m;
	m.setTrans( newPos );
	m_mt->setMatrix( m );
}


