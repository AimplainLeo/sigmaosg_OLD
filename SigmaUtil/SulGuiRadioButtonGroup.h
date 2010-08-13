// SulGuiRadioButtonGroup.h

#ifndef __SULGUIRADIOBUTTONGROUP_H__
#define __SULGUIRADIOBUTTONGROUP_H__

#include "SulGuiComp.h"
#include "SulGuiRadioButton.h"

class CSulGuiRadioButtonGroup : public CSulGuiComp
{
public:
				CSulGuiRadioButtonGroup( float x, float y );

	virtual void setupAttr( CSulXmlAttr* pAttr );

	void		addRadioButton( CSulGuiRadioButton* pRadioButton );

	void		removeSelect();

private:
	VEC_RADIOBUTTON	m_vecRadioButton;
	float			m_spacingY;
};

#endif // __SULGUIRADIOBUTTONGROUP_H__