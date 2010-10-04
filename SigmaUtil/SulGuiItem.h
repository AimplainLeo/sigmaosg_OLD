// SulGuiItem.h

#ifndef __SULGUIITEM_H__
#define __SULGUIITEM_H__

#include "SulGuiCanvas.h"

class CSulGuiItem : public osg::Referenced
{
public:
					CSulGuiItem( CSulGuiCanvas* pItem );

	CSulGuiCanvas*	getCanvas();

	void			toggleSelect();
	void			setSelect( bool bSelect );

private:
	osg::ref_ptr<CSulGuiCanvas>	m_rCanvas;
	bool						m_bSelected;
};

typedef std::vector< osg::ref_ptr<CSulGuiItem> >				VEC_GUIITEM;
typedef std::map< CSulGuiCanvas*, osg::ref_ptr<CSulGuiItem> >	MAP_GUIITEM;

#endif // __SULGUIITEM_H__