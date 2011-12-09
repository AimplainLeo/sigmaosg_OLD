// SulJsonArray.cpp

#include "stdafx.h"
#include "SulJsonArray.h"
#include <assert.h>

CSulJsonArray::CSulJsonArray() :
CSulJson()
{
}

CSulJsonArray::CSulJsonArray( CSulJson* pParent ) :
CSulJson( pParent )
{
}

void CSulJsonArray::add( CSulJson* p )
{
	m_vecJson.push_back( p );
}

sigma::uint32 CSulJsonArray::getCount()
{
	return m_vecJson.size();
}

CSulJson* CSulJsonArray::get( sigma::uint32 index )
{
	return m_vecJson[index];
}