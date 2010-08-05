// SulGuiListBox.h

#ifndef __SULGUILISTBOX_H__
#define __SULGUILISTBOX_H__

#include "SulGuiCanvas.h"
#include "SulGuiItem.h"

class CSulGuiListBox : public CSulGuiCanvas
{
public:
					CSulGuiListBox( float x, float y, float w, float h );

	void			addItem( CSulGuiItem* pItem );

private:
	VEC_GUIITEM		m_vecItem;
}; 

#endif // __SULGUILISTBOX_H__

