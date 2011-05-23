// SulGuiListBox.cpp

#include "stdafx.h"
#include "SulGuiListBox.h"
#include "SulGuiEventHandler.h"
#include "SulGuiTextBox.h"
#include "SulNodePath.h"
#include <osg/scissor>
#include <osgManipulator/Selection>

CSulGuiListBox::CSulGuiListBox() :
CSulGuiCanvas( "LISTBOX" )
{
	initConstructor();
}

CSulGuiListBox::CSulGuiListBox( float x, float y, float w, float h ) :
CSulGuiCanvas( "LISTBOX", x, y, w, h )
{
	initConstructor();
}

void CSulGuiListBox::initConstructor()
{
	m_clipPadding = 2.0f;
	m_bMultiSelect = false;
}

void CSulGuiListBox::setupTheme( CSulGuiThemeXml* pThemeXml )
{
	CSulGuiCanvas::setupTheme( pThemeXml );

	m_clipPadding = getThemeValue( "clip_padding" ).asFloat();
	m_itemOfsX = getThemeValue( "item_ofs_x" ).asFloat();

}

void CSulGuiListBox::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );

	if ( pAttr->exist( "clip_padding" ) )	m_clipPadding = pAttr->get( "clip_padding" ).asFloat();
	if ( pAttr->exist( "item_ofs_x" ) )		m_itemOfsX = pAttr->get( "item_ofs_x" ).asFloat();
	if ( pAttr->exist( "multiselect" ) )	m_bMultiSelect = pAttr->get( "multiselect" ).asBool();
}

void CSulGuiListBox::setupEventHandler( class CSulGuiEventHandler* pEventHandler )
{
	CSulGuiCanvas::setupEventHandler( pEventHandler );

	pEventHandler->signalViewResize.connect( this, &CSulGuiListBox::onViewResize );
}

void CSulGuiListBox::init()
{
	CSulGuiCanvas::init();

	float w = getW();
	float h = getH();

	// add a scrollbar
	m_rScrollBar = new CSulGuiScrollBar( w, 0, 32, h );
	m_rScrollBar->setBgColor( getBgColor() );
	m_rScrollBar->setupEventHandler( getEventHandler() );
	m_rScrollBar->init();
	//osg::MatrixTransform::addChild( m_rScrollBar );
	CSulGuiCanvas::addChild( m_rScrollBar );

	m_rScrollBar->signalChanged.connect( this, &CSulGuiListBox::onScrollBarChanged );
}

void CSulGuiListBox::setBgColor( const osg::Vec4& c )
{
	CSulGuiCanvas::setBgColor( c );

	if ( m_rScrollBar.valid() )
	{
		m_rScrollBar->setBgColor( c );
	}
}

float CSulGuiListBox::getTotalItemsHeight()
{
	VEC_GUIITEM::iterator i, e;
	i = m_vecItem.begin();	
	e = m_vecItem.end();
	float h = 0.0f;

	while( i!=e )
	{
		h += (*i)->getCanvas()->getH();		
		++i;
	}

	return h;
}

void CSulGuiListBox::onScrollBarChanged( float val )
{
	float totalHeight = getTotalItemsHeight()-getH();
	
	float realPosition = totalHeight*val;

	// reposition items
	VEC_GUIITEM::iterator i, e;
	i = m_vecItem.begin();	
	e = m_vecItem.end();

	float h = 0.0f;

	while( i!=e )
	{
		(*i)->getCanvas()->setY( -realPosition+h );
		h += (*i)->getCanvas()->getH();
		++i;
	}
}

CSulGuiItem* CSulGuiListBox::getItem( CSulGuiCanvas* pCanvas )
{
	VEC_GUIITEM::iterator i = m_vecItem.begin();
	VEC_GUIITEM::iterator ie = m_vecItem.end();

	while ( i!=ie )
	{
		if ( (*i)->getCanvas()==pCanvas )
			return (*i);

		++i;
	}

	return 0;
}

// fixme: local is to pCanvas and not the listbox
void CSulGuiListBox::onItemClicked( CSulGuiCanvas* pCanvas, float local_x, float local_y, float x, float y )
{
	if ( !isVisible() )
	{
		return;
	}

	// we need to test that the mouse_local_x and mouse_local_y are inside the listbox itself
	// because the local coordinates are for the pCanvas and not the listbox we need to calculate
	// the local mouse coordinates for the listbox
	osg::NodePath path;
	sulNodePath( *this, path, 0, true );
	osg::Matrix m = osg::computeLocalToWorld( path );
	float mouse_local_x = x-getWorldX();
	float mouse_local_y = y-getWorldY();
	if ( !isInside( mouse_local_x, mouse_local_y ) )
	{
		return;
	}

	if ( m_bMultiSelect )
	{
		getItem( pCanvas )->toggleSelect();
		return;
	}

	VEC_GUIITEM::iterator i,ie;
	i = m_vecItem.begin();
	ie = m_vecItem.end();
	while ( i!=ie )
	{
		(*i)->setSelect( (*i)->getCanvas()==pCanvas?true:false );	

		// signal that an item has been clicked on
		if ( (*i)->getCanvas()==pCanvas )
		{
			signalItemClicked( (*i) );
			signalItemChanged( (*i) );
		}

		++i;
	}
}

void CSulGuiListBox::show( bool bShow )
{
	CSulGuiCanvas::show( bShow );
	updateClipping();
}

void CSulGuiListBox::toggleShow()
{
	CSulGuiCanvas::toggleShow();
	updateClipping();
}

void CSulGuiListBox::updateClipping()
{
	float x = getWorldX()+m_clipPadding;
	float y = getWorldY()+m_clipPadding;
	float ww = getW()-m_clipPadding*2.0f;
	float hh = getH()-m_clipPadding*2.0f;

	osg::Scissor* pScissor = new osg::Scissor( x, m_viewH-(y+hh), ww+32, hh );

	VEC_GUIITEM::iterator i, e;
	i = m_vecItem.begin();	
	e = m_vecItem.end();

	while( i!=e )
	{
		(*i)->getCanvas()->getOrCreateStateSet()->setAttributeAndModes( pScissor, osg::StateAttribute::ON );
		++i;
	}
}

void CSulGuiListBox::removeChildItem( osg::Node* child )
{
	CSulGuiCanvas* pCanvas = dynamic_cast<CSulGuiCanvas*>(child);
	CSulGuiItem* item = getItem( pCanvas );

	VEC_GUIITEM::iterator i = std::find( m_vecItem.begin(), m_vecItem.end(), item );
	m_vecItem.erase( i );

	CSulGuiCanvas::removeChild( child );
}

bool CSulGuiListBox::addChild( Node* child )
{
	CSulGuiCanvas* pCanvas = dynamic_cast<CSulGuiCanvas*>(child);
	if ( !pCanvas )
	{
		osg::notify(osg::WARN) << "WARNING: CSulGuiListBox::addChild, only CSulGuiCanvas objects can be added as children" << std::endl;
		return false;
	}

	CSulGuiCanvas::addChild( pCanvas );

	CSulGuiItem* pItem = new CSulGuiItem( pCanvas );

	float h = getTotalItemsHeight();

	m_vecItem.push_back( pItem );

	// we need to set the position of the item in the listbox
	pCanvas->setXY( m_itemOfsX, h );

	//getEventHandler()->wantEvent( this, pCanvas, CSulGuiEventHandler::EVENT_MOUSERELEASE ); 
	pCanvas->signalClickedExt.connect( this, &CSulGuiListBox::onItemClicked );

	pCanvas->setActive( false );

	updateClipping();

	return true;
}

void CSulGuiListBox::onViewResize( float w, float h )
{
	m_viewW = w;
	m_viewH = h;

	updateClipping();
}

bool CSulGuiListBox::addTextItem( const CSulString& s )
{
	CSulGuiTextBox* p = new CSulGuiTextBox( s, 0, 0, 128, 32, 20 );
	p->setupEventHandler( getEventHandler() );
	p->setupTheme( getTheme() );
	p->init();
	p->useShaderBorder( false );
	p->useShaderBackground( false );
	return addChild( p );
}

CSulString CSulGuiListBox::getSelectedText()
{
	VEC_GUIITEM::iterator i, e;
	i = m_vecItem.begin();	
	e = m_vecItem.end();

	while( i!=e )
	{
		if ( (*i)->isSelected() )
		{
			CSulGuiTextBox* pTextBox = dynamic_cast<CSulGuiTextBox*>((*i)->getCanvas());
			if ( pTextBox )
			{
				return pTextBox->getText();
			}
		}

		++i;
	}

	return "";
}

void CSulGuiListBox::setSelectedIndex( sigma::uint32 index )
{
	VEC_GUIITEM::iterator i, e;
	i = m_vecItem.begin();	
	e = m_vecItem.end();

	sigma::int32 count = 0;
	while( i!=e )
	{
		(*i)->setSelect( count==index?true:false );

		if ( count==index )
			signalItemChanged( (*i) );

		++count;
		++i;
	}
}

sigma::int32 CSulGuiListBox::getSelectedIndex()
{
	VEC_GUIITEM::iterator i, e;
	i = m_vecItem.begin();	
	e = m_vecItem.end();

	sigma::int32 count = 0;
	while( i!=e )
	{
		if ( (*i)->isSelected() )
		{
			return count;
		}

		++count;
		++i;
	}

	return -1;
}

CSulGuiItem* CSulGuiListBox::getSelectedItem()
{
	sigma::int32 i = getSelectedIndex();
	return m_vecItem[i];
}