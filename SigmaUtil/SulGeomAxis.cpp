// SulGeomAxis.cpp

#include "stdafx.h"
#include "SulGeomAxis.h"
#include <osg/matrixtransform>

CSulGeomAxis::CSulGeomAxis( float len ) :
m_len( len )
{
	createDrawable();
}

void CSulGeomAxis::createDrawable()
{
	m_arrowRight	= new CSulGeomArrow3D( m_len, osg::Vec4(1,0,0,1), osg::Vec4(1,0,0,1) );
	m_arrowForward	= new CSulGeomArrow3D( m_len, osg::Vec4(0,1,0,1), osg::Vec4(0,1,0,1) );
	m_arrowUp		= new CSulGeomArrow3D( m_len, osg::Vec4(0,0,1,1), osg::Vec4(0,0,1,1) );

	// up arrow needs no transformation, the arrow is drawn upwards by default
	addChild( m_arrowUp );
	
	osg::ref_ptr<osg::MatrixTransform> rTransForward = new osg::MatrixTransform( osg::Matrix( osg::Quat( osg::DegreesToRadians(-90.0f), osg::X_AXIS ) ) );
	addChild( rTransForward );
	rTransForward->addChild( m_arrowForward );

	osg::ref_ptr<osg::MatrixTransform> rTransRight = new osg::MatrixTransform( osg::Matrix( osg::Quat( osg::DegreesToRadians(90.0f), osg::Y_AXIS ) ) );
	addChild( rTransRight );
	rTransRight->addChild( m_arrowRight );

}