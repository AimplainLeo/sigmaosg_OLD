// SulGuiRadioButtonGroup.cpp

#include "stdafx.h"
#include "SulGuiRadioButtonGroup.h"

CSulGuiRadioButtonGroup::CSulGuiRadioButtonGroup( float x, float y ) :
CSulGuiComp( x, y )
{
}

void CSulGuiRadioButtonGroup::addRadioButton( CSulGuiRadioButton* pRadioButton )
{
	m_vecRadioButton.push_back( pRadioButton );
}