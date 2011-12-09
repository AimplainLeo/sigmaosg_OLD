// SulJsonObject.cpp

#include "stdafx.h"
#include "SulJsonObject.h"

CSulJsonObject::CSulJsonObject() :
CSulJson()
{
}

CSulJsonObject::CSulJsonObject( CSulJson* pParent ) :
CSulJson( pParent )
{
}

void CSulJsonObject::add( const CSulString& key, CSulJson* p )
{
	m_mapKeyVal[key] = p;
}

CSulJson* CSulJsonObject::get( const CSulString& key )
{
	return m_mapKeyVal[key];
}
