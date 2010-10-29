// SulGuiComboBox.h

#ifndef __SULGUICOMBOBOX_H__
#define __SULGUICOMBOBOX_H__

#include "SulGuiCanvas.h"
#include "SulGuiTextBox.h"
#include "SulGuiButton.h"
#include "SulGuiListBox.h"
#include "SulSigSlots.h"

class CSulGuiComboBox : public CSulGuiCanvas
{
public:
					CSulGuiComboBox( float x, float y, float w, float h );

	virtual void	setupEventHandler( CSulGuiEventHandler* pEventHandler );
	virtual void	setupTheme( CSulGuiThemeXml* pThemeXml );
	virtual void	setupAttr( CSulXmlAttr* pAttr );
	
	virtual void	init();

	virtual bool	addChild( Node *child );

	virtual class CSulGuiComboBox*		asComboBox()	{ return this; }

private:
	void			onDropDownClick( CSulGuiCanvas* );
	
private:
	osg::ref_ptr< CSulGuiTextBox >	m_rTextBox;
	osg::ref_ptr< CSulGuiButton >	m_rButton;
	osg::ref_ptr< CSulGuiListBox >	m_rListBox;
};

#endif // __SULGUICOMBOBOX_H__