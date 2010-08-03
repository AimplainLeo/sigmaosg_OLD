// SulGuiRadioButtonGroup.h

#ifndef __SULGUIRADIOBUTTONGROUP_H__
#define __SULGUIRADIOBUTTONGROUP_H__

#include "SulGuiComp.h"
#include "SulGuiRadioButton.h"

class CSulGuiRadioButtonGroup : public CSulGuiComp
{
public:
				CSulGuiRadioButtonGroup( float x, float y );

	void		addRadioButton( CSulGuiRadioButton* pRadioButton );

private:
	VEC_RADIOBUTTON	m_vecRadioButton;
};

#endif // __SULGUIRADIOBUTTONGROUP_H__