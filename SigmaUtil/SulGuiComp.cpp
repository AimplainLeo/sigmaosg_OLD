// SulGuiComp.h

#include "stdafx.h"
#include "SulGuiComp.h"
#include "SulShaderGuiFrame.h"
#include <osg/matrix>

CSulGuiComp::CSulGuiComp( const CSulString& sName, float x, float y, float w, float h )
{
	m_events = 0;

	m_rQuadColor.set( 1.0f, 1.0f, 1.0f, 1.0f );

	m_sName;
	m_w = w;
	m_h = h;

	osg::Matrix m;
	m.setTrans( x, y, 0.0f );
	setMatrix( m );	
}

// FIXME
float CSulGuiComp::getX()
{
	osg::Matrix m;
	computeLocalToWorldMatrix( m, 0 );
	return m.getTrans().x();
}

// FIXME
float CSulGuiComp::getY()
{
	return getMatrix().getTrans().y();
}

void CSulGuiComp::addEvents( Sigma::uint32 events )
{
	m_events |= events;
}

Sigma::uint32 CSulGuiComp::getEvents()
{
	return m_events;
}

void CSulGuiComp::setQuadColor( osg::Vec4& c )
{
	m_rQuadColor = c;
}

void CSulGuiComp::setRenderQuad( bool bRenderQuad )
{
	if ( !bRenderQuad )
	{
		if ( m_rGeodeQuad.valid() )
		{
			removeChild( m_rGeodeQuad );
		}

		return;
	}

	const osg::Matrix& m = getMatrix();
	osg::Vec3 pos = m.getTrans();
	
	m_rQuad = new CSulGeomQuad(
		osg::Vec3( m_w/2.0f, m_h/2.0f, 0.0f ),
		m_w, m_h );
	m_rQuad->createUV();
	m_rQuad->setColor( m_rQuadColor );
	m_rGeodeQuad = new osg::Geode;
	m_rGeodeQuad->addDrawable( m_rQuad->getDrawable() );

	addChild( m_rGeodeQuad );

	// add a shader
	new CSulShaderGuiFrame( m_rGeodeQuad );

	m_rGeodeQuad->getOrCreateStateSet()->addUniform( new osg::Uniform( "w", m_w ) );
	m_rGeodeQuad->getOrCreateStateSet()->addUniform( new osg::Uniform( "h", m_h ) );
	m_rGeodeQuad->getOrCreateStateSet()->addUniform( new osg::Uniform( "border", 4.0f ) );
}


