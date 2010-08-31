// SulGuiAlignH.h

#ifndef __SULGUIALIGNH_H__
#define __SULGUIALIGNH_H__

#include "SulGuiCanvas.h"

class CSulGuiAlignH : public CSulGuiCanvas
{
public:
	enum EALIGN
	{
		LEFT,
		RIGHT
	};

public:
					CSulGuiAlignH();

	virtual void	setupEventHandler( CSulGuiEventHandler* pEventHandler );

	void			setupAttr( CSulXmlAttr* pAttr );

	virtual void	init();

private:
	void			onViewResize( float w, float h );

private:
	EALIGN			m_eAlign;
	float			m_xOfs;
};

#endif // __SULGUIALIGNH_H__