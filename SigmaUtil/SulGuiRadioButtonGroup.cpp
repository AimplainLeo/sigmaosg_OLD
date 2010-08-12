// SulGuiRadioButtonGroup.cpp

#include "stdafx.h"
#include "SulGuiRadioButtonGroup.h"

CSulGuiRadioButtonGroup::CSulGuiRadioButtonGroup( float x, float y ) :
CSulGuiComp( "RADIOBUTTON_GROUP", x, y )
{
}

void CSulGuiRadioButtonGroup::addRadioButton( CSulGuiRadioButton* pRadioButton )
{
	m_vecRadioButton.push_back( pRadioButton );
}

void CSulGuiRadioButtonGroup::removeSelect()
{
	VEC_RADIOBUTTON::iterator i;

	i = m_vecRadioButton.begin();
	while ( i!=m_vecRadioButton.end() )
	{
		(*i)->removeSelect();
		++i;
	}
}