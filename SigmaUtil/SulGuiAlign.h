// SulGuiAlign.h

#ifndef __SULGUIALIGN_H__
#define __SULGUIALIGN_H__

#include "SulGuiCanvas.h"
#include "SulExport.h"

class SUL_EXPORT CSulGuiAlign : public CSulGuiCanvas
{
public:
	enum EALIGNV
	{
		V_TOP,
		V_BOTTOM,
		V_CENTER
	};

	enum EALIGNH
	{
		H_LEFT,
		H_RIGHT,
		H_CENTER
	};

public:
					CSulGuiAlign();

	virtual void	setupEventHandler( CSulGuiEventHandler* pEventHandler );
	virtual void	setupAttr( CSulXmlAttr* pAttr );
	virtual void	init();

	void			update();

	CSulGuiAlign*	asAlign()		{ return this; }

private:
	void			onViewResize( float w, float h );

private:
	EALIGNH			m_eAlignH;
	EALIGNV			m_eAlignV;
	float			m_xOfs;
	float			m_yOfs;
};

#endif // __SULGUIALIGN_H__