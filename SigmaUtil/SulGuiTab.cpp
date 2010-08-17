// SulGuiTab.cpp

#include "stdafx.h"
#include "SulGuiTab.h"
#include "SulGuiTabPage.h"

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

bool CSulGuiTab::addChild( Node *child )
{
	//  TABPAGE expected
	CSulGuiTabPage* page = dynamic_cast<CSulGuiTabPage*>(child);
	if ( !page )
	{
		osg::notify(osg::WARN) << "WARNING: CSulGuiTab::addChild, only expects CSulTabPage objects." << std::endl;
		return false;
	}

	page->setXY( 0.0f, getH() );
	page->setWH( getW(), 256.0f );

	return CSulGuiCanvas::addChild( page );
}