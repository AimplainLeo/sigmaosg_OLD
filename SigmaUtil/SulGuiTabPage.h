// SulGuiTabPage.h

#ifndef __SULGUITABPAGE_H__
#define __SULGUITABPAGE_H__

#include "SulGuiCanvas.h"

class CSulGuiTabPage : public CSulGuiCanvas
{
public:
					CSulGuiTabPage();

	virtual void	setupAttr( CSulXmlAttr* pAttr );

	CSulString		getTitle();

private:
	CSulString		m_sTitle;
};

#endif // __SULGUITABPAGE_H__