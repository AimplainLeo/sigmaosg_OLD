// SulGuiTable.cpp

#include "stdafx.h"
#include "SulGuiTable.h"
#include "SulGuiTextBox.h"

CSulGuiTable::CSulGuiTable( const CSulString& sCompName ) :
CSulGuiCanvas( sCompName )
{
	initConstructor();
}

CSulGuiTable::CSulGuiTable() :
CSulGuiCanvas( "TABLE" )
{
	initConstructor();
}

void CSulGuiTable::initConstructor()
{
	m_rows = 2;
	m_cols = 2;
	m_padding = 4;
}

void CSulGuiTable::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );

	if ( pAttr->exist( "rows" ) )	m_rows = pAttr->get( "rows" ).asFloat();
	if ( pAttr->exist( "cols" ) )	m_cols = pAttr->get( "cols" ).asFloat();
}

void CSulGuiTable::init()
{
	CSulGuiCanvas::init();

	sigma::uint32 w = getW()-m_padding*2;
	sigma::uint32 h = getH()-m_padding*2;

	m_rCellGroup = new osg::Group;
	addChild( m_rCellGroup );

	// create text object for each cell
	for ( sigma::uint32 y=0; y<m_rows; y++ )
		for ( sigma::uint32 x=0; x<m_cols; x++ )
		{
			sigma::uint32 xx = m_padding + x * (w/m_cols);
			sigma::uint32 yy = m_padding + y * (h/m_rows);
			CSulGuiTextBox* p = new CSulGuiTextBox( xx, yy );
			p->setupTheme( getTheme() );
			p->setupEventHandler( getEventHandler() );
			p->init();
			p->setWH( w/m_cols, h/m_rows );
			p->setTextOffset( 4, 4 );
			p->setText( "hello" );
			p->showCanvas( true );
			p->useShaderBorder( true );
			m_rCellGroup->addChild( p );
		}
}

void CSulGuiTable::setText( sigma::uint32 row, sigma::uint32 col, const CSulString& text )
{
	osg::Node* pNode = m_rCellGroup->getChild( col + row*m_cols );
	CSulGuiTextBox* p = dynamic_cast<CSulGuiTextBox*>( pNode );
	p->setText( text );
}

sigma::uint32 CSulGuiTable::getRowCount()
{
	return m_rows;
}

sigma::uint32 CSulGuiTable::getColCount()
{
	return m_cols;
}
