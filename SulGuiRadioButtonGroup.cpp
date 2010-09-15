// SulGuiRadioButtonGroup.cpp

#include "stdafx.h"
#include "SulGuiRadioButtonGroup.h"

CSulGuiRadioButtonGroup::CSulGuiRadioButtonGroup( float x, float y ) :
CSulGuiComp( "RADIOBUTTON_GROUP", x, y )
{
}

void CSulGuiRadioButtonGroup::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiComp::setupAttr( pAttr );

	m_spacingY = getThemeValue( "spacing_y" ).asFloat();

	if ( pAttr->exist( "spacing_y" ) ) m_spacingY = pAttr->get( "spacing_y" ).asFloat();
}

void CSulGuiRadioButtonGroup::addRadioButton( CSulGuiRadioButton* pRadioButton )
{
	pRadioButton->setY( m_spacingY*m_vecRadioButton.size() );

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