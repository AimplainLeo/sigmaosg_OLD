// SulGuiItem.cpp

#include "stdafx.h"
#include "SulGuiItem.h"
#include "SulGuiText.h"

CSulGuiItem::CSulGuiItem( CSulGuiCanvas* pCanvas ) 
{
	m_rCanvas = pCanvas;
	m_bSelected = false;
}

CSulGuiCanvas* CSulGuiItem::getCanvas()
{
	return m_rCanvas;
}

void CSulGuiItem::toggleSelect()
{
	m_bSelected = !m_bSelected;
	m_rCanvas->m_rGeodeQuad->getOrCreateStateSet()->addUniform( new osg::Uniform( "cover", m_bSelected?1:0 ) );
}

void CSulGuiItem::setSelect( bool bSelect )
{
	m_bSelected = bSelect;
	m_rCanvas->m_rGeodeQuad->getOrCreateStateSet()->addUniform( new osg::Uniform( "cover", m_bSelected?1:0 ) );
}