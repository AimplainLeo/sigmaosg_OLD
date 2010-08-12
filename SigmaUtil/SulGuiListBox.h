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

	void			setupView( float w, float h );
	void			setupEventHandler( class CSulGuiEventHandler* pEventHandler );

	void			addItem( CSulGuiCanvas* pCanvas );

	float			getTotalItemsHeight();

private:
	void			onClick( CSulGuiCanvas* pItem );
	void			onScrollBarChanged( float val );

private:
	MAP_GUIITEM						m_mapItem;
	osg::ref_ptr<CSulGuiScrollBar>	m_rScrollBar;
}; 

#endif // __SULGUILISTBOX_H__

