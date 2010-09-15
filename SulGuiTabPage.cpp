// SulGuiTabPage.cpp

#include "stdafx.h"
#include "SulGuiTabPage.h"

CSulGuiTabPage::CSulGuiTabPage() :
CSulGuiCanvas( "TABPAGE" )
{
}

void CSulGuiTabPage::setupAttr( CSulXmlAttr* pAttr )
{
	CSulGuiCanvas::setupAttr( pAttr );

	if ( pAttr->exist( "title" ) ) m_sTitle = pAttr->get( "title" );
}

CSulString CSulGuiTabPage::getTitle()
{
	return m_sTitle;
}