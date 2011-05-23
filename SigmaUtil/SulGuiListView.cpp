// SulGuiListView.cpp

#include "stdafx.h"
#include "SulGuiListView.h"
#include "SulStringList.h"

CSulGuiListView::CSulGuiListView() :
CSulGuiTable( "LISTVIEW" )
{
	m_header = "";
}

void CSulGuiListView::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiTable::setupAttr( pAttr );

	if ( pAttr->exist( "header" ) )	m_header = pAttr->get( "header" );
}

void CSulGuiListView::init()
{
	CSulGuiTable::init();

	// setup header
	if ( !m_header.empty() )
	{
		addRow();

		CSulStringList list;

		list.Tokenize( m_header );
		sigma::uint32 col = getColCount();
		for ( sigma::uint32 x=0; x<col; x++ )
		{
			set( 0, x, list.getString(x) );
		}
	}
}

void CSulGuiListView::set( sigma::uint32 row, sigma::uint32 col, const CSulString& s )
{
	CSulString::VEC_STRING& vecRow = m_vecRow[row];
	vecRow[col] = s;
	updateView();
}

sigma::uint32 CSulGuiListView::addRow()
{
	CSulString::VEC_STRING row;
	row.resize( getColCount() );
	m_vecRow.push_back( row );
	return m_vecRow.size()-1;
}

CSulString CSulGuiListView::getText( sigma::uint32 row, sigma::uint32 col )
{
	// check out of bounds
	if ( row >= m_vecRow.size() )
		return "";

	// check out of bounds
	if ( col >= getColCount() )
		return "";

	return m_vecRow[row][col];
}

void CSulGuiListView::updateView()
{
	sigma::uint32 row = getRowCount();
	sigma::uint32 col = getColCount();

	// update header
	for ( sigma::uint32 x=0; x<col; x++ )
	{
		CSulString s = getText( 0, x );
		setText( 0, x, s );
	}

	// update content
	for ( sigma::uint32 y=1; y<row; y++ )
		for ( sigma::uint32 x=0; x<col; x++ )
		{
			CSulString s = getText( y, x );
			setText( y, x, s );
		}
}


