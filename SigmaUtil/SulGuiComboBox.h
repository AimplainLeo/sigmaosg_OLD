// SulGuiComboBox.h

#ifndef __SULGUICOMBOBOX_H__
#define __SULGUICOMBOBOX_H__

#include "SulGuiCanvas.h"
#include "SulGuiTextBox.h"
#include "SulGuiButton.h"

class CSulGuiComboBox : public CSulGuiCanvas
{
public:
					CSulGuiComboBox( float x, float y, float w, float h );

	virtual void	setupTheme( CSulGuiThemeXml* pThemeXml );
	virtual void	setupAttr( CSulXmlAttr* pAttr );
	

	virtual void	init();

private:
	osg::ref_ptr< CSulGuiTextBox >	m_rTextBox;
	osg::ref_ptr< CSulGuiButton >	m_rButton;
};

#endif // __SULGUICOMBOBOX_H__