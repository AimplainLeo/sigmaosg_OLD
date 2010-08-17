	// SulGuiTab.h

#ifndef __SULGUITAB_H__
#define __SULGUITAB_H__

#include "SulGuiCanvas.h"

class CSulGuiTab : public CSulGuiCanvas
{
public:
	CSulGuiTab();

	virtual void	setupTheme( CSulGuiThemeXml* pThemeXml );
	virtual void	setupAttr( CSulXmlAttr* pAttr );

	virtual bool	addChild( Node *child );
};

#endif // __SULGUITAB_H__