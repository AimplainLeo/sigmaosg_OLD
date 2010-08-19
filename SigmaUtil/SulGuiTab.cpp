// SulGuiTab.cpp

#include "stdafx.h"
#include "SulGuiTab.h"
#include "SulGuiTabPage.h"
#include "SulGuiButton.h"

CSulGuiTab::CSulGuiTab( ) :
CSulGuiCanvas( "TAB" )
{
	
}

void CSulGuiTab::setupTheme( CSulGuiThemeXml* pThemeXml )
{
	CSulGuiCanvas::setupTheme( pThemeXml );
}

void CSulGuiTab::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );
}

void CSulGuiTab::setupEventHandler( CSulGuiEventHandler* pEventHandler )
{
	CSulGuiCanvas::setupEventHandler( pEventHandler );
}

bool CSulGuiTab::addChild( Node *child )
{
	//  TABPAGE expected
	CSulGuiTabPage* page = dynamic_cast<CSulGuiTabPage*>(child);
	if ( !page )
	{
		osg::notify(osg::WARN) << "WARNING: CSulGuiTab::addChild, only expects CSulTabPage objects." << std::endl;
		return false;
	}

	// create a button for the tabpage in the tab itself
	float ofsx = m_mapButton.size()*48.0f;
	CSulGuiButton* pBut = new CSulGuiButton( ofsx, 0, 48, 32 );
	pBut->setText( page->getTitle() );
	pBut->setFontSize( 20.0f );
	pBut->setupEventHandler( getEventHandler() );
	pBut->init();
	osg::MatrixTransform::addChild( pBut );
	CSulGuiItem* pItem = new CSulGuiItem( page );
	m_mapButton[pBut] = pItem;

	// position and size tabpage
	page->setXY( 0.0f, getH() );
	page->setWH( getW(), 256.0f );

	// hide/show page (first one by default is visible )
	page->show( m_mapButton.size()==1?true:false );

	// set connection for clicking on button to page
	pBut->signalClicked.connect( this, &CSulGuiTab::onTabClick );

	return CSulGuiCanvas::addChild( page );
}

void CSulGuiTab::hideAllPages()
{
	MAP_GUIITEM::iterator i, iEnd;
	
	i = m_mapButton.begin();
	iEnd = m_mapButton.end();

	while ( i!=iEnd )
	{
		i->second->getCanvas()->show( false );
		++i;
	}
}

void CSulGuiTab::onTabClick( CSulGuiCanvas* pCanvas )
{
	hideAllPages();

	m_mapButton[pCanvas]->getCanvas()->show( true );
}