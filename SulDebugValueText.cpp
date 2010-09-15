// SulDebugValueText.cpp

#include "stdafx.h"
#include "SulDebugValueText.h"

CSulDebugValueText::CSulDebugValueText( const std::string& sTitle, const std::string& sTextArray, const std::string& sTextDefault ) :
CSulDebugValueBase( sTitle )
{
	m_listText = new CSulStringList( sTextArray );
	m_index = m_listText->getIndex( sTextDefault );
	setTextValue( m_listText->getString(m_index) );
}

void CSulDebugValueText::inc()
{
	m_index++;
	if ( m_index>=m_listText->GetList().size() )
	{	
		m_index = 0;
	}

	setTextValue( m_listText->getString(m_index) );
}

void CSulDebugValueText::dec()
{
	if ( m_index==0 )
	{
		m_index = m_listText->GetList().size()-1;
	}
	else
	{
		m_index--;
	}

	setTextValue( m_listText->getString(m_index) );
}

CSulString CSulDebugValueText::getText()
{
	return m_listText->getString( m_index );
}
