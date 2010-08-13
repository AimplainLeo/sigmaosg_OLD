// SulGuiListBox.h

#ifndef __SULGUILISTBOX_H__
#define __SULGUILISTBOX_H__

#include "SulGuiCanvas.h"
#include "SulGuiItem.h"
#include "SulGuiScrollBar.h"
#include "SulSigSlots.h"

class CSulGuiListBox : public CSulGuiCanvas, public sigma::has_slots<>
{
public:
					CSulGuiListBox( float x, float y, float w, float h );

	virtual	void	setupAttr( CSulXmlAttr* pAttr );
	virtual void	setupView( float w, float h );
	virtual void	setupEventHandler( class CSulGuiEventHandler* pEventHandler );

	virtual void	init();

	void			addItem( CSulGuiCanvas* pCanvas );

	float			getTotalItemsHeight();

private:
	void			onClick( CSulGuiCanvas* pItem );
	void			onScrollBarChanged( float val );

	void			updateClipping();

private:
	MAP_GUIITEM						m_mapItem;
	osg::ref_ptr<CSulGuiScrollBar>	m_rScrollBar;
	float							m_clipPadding;
	float							m_viewW;
	float							m_viewH;
	float							m_itemOfsX;
}; 

#endif // __SULGUILISTBOX_H__

