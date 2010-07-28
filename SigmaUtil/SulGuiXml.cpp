// SulGuiXml.cpp

#include "stdafx.h"
#include "SulGuiXml.h"
#include "SulXmlAttr.h"
#include "SulGuiCanvas.h"
#include "SulGuiButton.h"
#include "SulGuiText.h"

CSulGuiXml::CSulGuiXml( osg::Group* pRootGroup, CSulGuiEventHandler* pEventHandler )
{
	m_rEventHandler = pEventHandler;
	m_rRootGroup = pRootGroup;
	m_curGroup = pRootGroup;
}

void CSulGuiXml::elementStart( const CSulString& sName, CSulXmlAttr* pAttr )
{
	if ( sName=="CANVAS" )
	{
		CSulGuiComp* p = new CSulGuiCanvas(
			pAttr->get( "name" ),
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat()
		);

		p->setRenderQuad( pAttr->get( "renderQuad" ).asBool() );

		m_curGroup->addChild( p );
		m_curGroup = p;
	}

	if ( sName=="BUTTON" )
	{
		CSulGuiComp* p = new CSulGuiButton(
			pAttr->get( "name" ),
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat()
		);

		m_rEventHandler->addComp( p );

		p->setRenderQuad( pAttr->get( "renderQuad" ).asBool() );

		m_curGroup->addChild( p );
		m_curGroup = p;
	}

	if ( sName=="TEXT" )
	{
		CSulGuiComp* p = new CSulGuiText(
			pAttr->get( "name" ),
			pAttr->get( "x" ).asFloat(),
			pAttr->get( "y" ).asFloat(),
			pAttr->get( "w" ).asFloat(),
			pAttr->get( "h" ).asFloat(),
			pAttr->get( "size" ).asFloat()
		);

		p->setRenderQuad( pAttr->get( "renderQuad" ).asBool() );

		m_curGroup->addChild( p );
		m_curGroup = p;
	}


	// we assume only element tags from now on (there is no smart error checking here yet!)
	m_mapAttr[sName] = pAttr;
}

void CSulGuiXml::elementEnd( const CSulString& sName )
{
	if ( sName=="CANVAS" || sName=="BUTTON" || sName=="TEXT" )
	{
		 m_curGroup = m_curGroup->getParent( 0 );
	}
}

CSulXmlAttr* CSulGuiXml::get( const CSulString& sTag ) 
{
	MAP_ATTR::const_iterator i;

	i = m_mapAttr.find( sTag );
	if ( i!=m_mapAttr.end() )
	{
		return m_mapAttr[sTag].get();
	}

	return 0;
}

