	// SulGuiTab.h

#ifndef __SULGUITAB_H__
#define __SULGUITAB_H__

#include "SulGuiCanvas.h"
#include "SulGuiItem.h"

class CSulGuiTab : public CSulGuiCanvas
{
public:
	CSulGuiTab();

	virtual void	setupTheme( CSulGuiThemeXml* pThemeXml );
	virtual void	setupAttr( CSulXmlAttr* pAttr );
	virtual void	setupEventHandler( CSulGuiEventHandler* pEventHandler );

	virtual bool	addChild( Node *child );

private:
	void			onTabClick( CSulGuiCanvas* pCanvas );

	void			hideAllPages();

private:
	MAP_GUIITEM		m_mapButton;	// buttons in tab
};

#endif // __SULGUITAB_H__