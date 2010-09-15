// SulGuiCounter.h

#ifndef __SULGUICOUNTER_H__
#define __SULGUICOUNTER_H__

#include "SulGuiCanvas.h"

class SUL_EXPORT CSulGuiCounter : public CSulGuiCanvas
{
public:
					CSulGuiCounter();

	virtual void	setupTheme( CSulGuiThemeXml* pThemeXml );
	virtual void	setupAttr( CSulXmlAttr* pAttr );

	virtual void	init();

	sigma::uint32	getValue();

	CSulGuiCounter*	asCounter() { return this; }

private:
	void			onClick( CSulGuiCanvas* pCanvas );

private:
	sigma::uint32	m_digits;
};

#endif // __SULGUICOUNTER_H__