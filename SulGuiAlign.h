// SulGuiAlign.h

#ifndef __SULGUIALIGN_H__
#define __SULGUIALIGN_H__

#include "SulGuiCanvas.h"

class CSulGuiAlign : public CSulGuiCanvas
{
public:
	enum EALIGNV
	{
		TOP,
		BOTTOM
	};

	enum EALIGNH
	{
		LEFT,
		RIGHT
	};

public:
					CSulGuiAlign();

	virtual void	setupEventHandler( CSulGuiEventHandler* pEventHandler );
	virtual void	setupAttr( CSulXmlAttr* pAttr );
	virtual void	init();

private:
	void			onViewResize( float w, float h );

private:
	EALIGNH			m_eAlignH;
	EALIGNV			m_eAlignV;
	float			m_xOfs;
	float			m_yOfs;
};

#endif // __SULGUIALIGN_H__