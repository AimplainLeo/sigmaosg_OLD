// SulGuiListBox.h

#ifndef __SULGUILISTBOX_H__
#define __SULGUILISTBOX_H__

#include "SulGuiCanvas.h"
#include "SulGuiItem.h"
#include "SulGuiScrollBar.h"
#include "SulSigSlots.h"
#include "SulExport.h"

class SUL_EXPORT CSulGuiListBox : public CSulGuiCanvas
{
public:
					CSulGuiListBox();
					CSulGuiListBox( float x, float y, float w, float h );

	virtual void	show( bool bShow );
	virtual void	toggleShow();

	virtual void	setupTheme( CSulGuiThemeXml* pThemeXml );
	virtual	void	setupAttr( CSulXmlAttr* pAttr );
	virtual void	setupEventHandler( class CSulGuiEventHandler* pEventHandler );

	virtual void	init();

	float			getTotalItemsHeight();

	virtual bool	addChild( Node *child );

	bool			addTextItem( const CSulString& s );

	// returns the first selected (only text -> if any)
	CSulString		getSelectedText();

	sigma::int32		getSelectedIndex();

	virtual class CSulGuiListBox*	asListBox() { return this; }

	virtual void	eventMouseRelease( CSulGuiCanvas* pCanvas, float local_x, float local_y, float x, float y );

	sigma::signal1<CSulGuiItem*>	signalItemClicked;

private:
	void			initConstructor();

	void			onViewResize( float w, float h );
	void			onScrollBarChanged( float val );

	void			updateClipping();

	CSulGuiItem*	getItem( CSulGuiCanvas* pCanvas );

private:
	VEC_GUIITEM						m_vecItem;
	osg::ref_ptr<CSulGuiScrollBar>	m_rScrollBar;
	float							m_clipPadding;
	float							m_viewW;
	float							m_viewH;
	float							m_itemOfsX;
	bool							m_bMultiSelect;
}; 

#endif // __SULGUILISTBOX_H__

