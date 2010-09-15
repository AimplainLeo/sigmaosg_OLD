// SulDebugValueFloat.cpp

#include "stdafx.h"
#include "SulDebugValueFloat.h"
#include "SulString.h"

CSulDebugValueFloat::CSulDebugValueFloat( const std::string& sTitle, float fMin, float fMax, float fVal, float fStep ) :
CSulDebugValueBase( sTitle )
{
	m_minVal = fMin;
	m_maxVal = fMax;
	m_stepVal = fStep;
	m_val = fVal;
	setTextValue( CSulString(m_val) );
}

void CSulDebugValueFloat::inc()
{
	m_val += m_stepVal;
	if ( m_val>m_maxVal )
	{
		m_val = m_maxVal;
	}

	setTextValue( CSulString(m_val) );
}

void CSulDebugValueFloat::dec()
{
	m_val -= m_stepVal;
	if ( m_val<m_minVal )
	{
		m_val = m_minVal;
	}

	setTextValue( CSulString(m_val) );
}

float CSulDebugValueFloat::getVal()
{
	return m_val;
}
