// SulGuiItem.cpp

#include "stdafx.h"
#include "SulGuiItem.h"
#include "SulGuiText.h"

CSulGuiItem::CSulGuiItem( const CSulString& sText) :
CSulGuiComp( 0, 0 )
{
	CSulGuiText* pGuiText = new CSulGuiText( sText, 0, 0, 32.0f );
	addChild( pGuiText );
}