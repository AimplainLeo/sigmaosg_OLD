// SulJsonString.cpp

#include "stdafx.h"
#include "SulJsonString.h"

CSulJsonString::CSulJsonString( const CSulString& s ) :
CSulJson()
{
	m_s = s;
}

CSulString CSulJsonString::get() const
{
	return m_s;
}