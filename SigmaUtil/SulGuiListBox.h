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

	virtual void	setBgColor( const osg::Vec4& c );

	float			getTotalItemsHeight();

	virtual bool	addChild( Node *child );

	// osg::Group removeChild is not a virtual :(
	void			removeChildItem( osg::Node* child );

	bool			addTextItem( const CSulString& s );

	// returns the first selected (only text -> if any)
	CSulString		getSelectedText();

	void				setSelectedIndex( sigma::uint32 index );
	sigma::int32		getSelectedIndex();

	CSulGuiItem*		getSelectedItem();

	virtual class CSulGuiListBox*	asListBox() { return this; }

	sigma::signal1<CSulGuiItem*>	signalItemClicked;
	sigma::signal1<CSulGuiItem*>	signalItemChanged;

private:
	void			initConstructor();

	void			onViewResize( float w, float h );
	void			onScrollBarChanged( float val );

	void			onItemClicked( CSulGuiCanvas* pCanvas, float local_x, float local_y, float x, float y );

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

