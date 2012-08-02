// SulGeomLines.cpp

#include "stdafx.h"
#include "SulGeomLines.h"

CSulGeomLines::CSulGeomLines( const osg::Vec4& color ) :
CSulGeom( color )
{
	initConstructor();
}

CSulGeomLines::CSulGeomLines( const sigma::VEC_LINESEGMENT& vecLines ) :
CSulGeom(),
m_vecLines( vecLines )
{
	initConstructor();
}

void CSulGeomLines::initConstructor()
{
	createDrawable();
}

void CSulGeomLines::createDrawable()
{
	// we can't create if we don't have any lines
	if ( m_vecLines.size()==0 )
		return;

	osg::ref_ptr<osg::UIntArray> indices = new osg::UIntArray;	

	sigma::uint32 count = 0;
	sigma::VEC_LINESEGMENT::const_iterator i;
	i = m_vecLines.begin();
	while ( i!=m_vecLines.end() )
	{
		m_verts->push_back( (*i)->start() );
		m_verts->push_back( (*i)->end() );

		indices->push_back( count++ );
		indices->push_back( count++ );

		m_colors->push_back( m_color );
		m_colors->push_back( m_color );

		++i;
	}

	setColorBinding( osg::Geometry::BIND_PER_VERTEX );

    addPrimitiveSet(
        new osg::DrawElementsUInt( 
			GL_LINES,
            indices->size(), 
			&(indices->front())
		)
	);
}

void CSulGeomLines::setLines( const sigma::VEC_LINESEGMENT& vecLines )
{
	m_vecLines = vecLines;
}