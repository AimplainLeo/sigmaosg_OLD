// SulXmlReader.cpp

#include "stdafx.h"
#include "SulXmlReader.h"

CSulXmlReader::CSulXmlReader()
{
	m_curTag = 0;
}

CSulXmlNodeTag* CSulXmlReader::findTag( const CSulString& tag, CSulXmlNodeTag* searchTag )
{
	if ( !searchTag )
		searchTag = m_root;

	if ( searchTag->getName()==tag )
		return searchTag;

	const CSulNode::VEC_NODE& vecChildren = searchTag->getChildList();

	CSulNode::VEC_NODE::const_iterator i = vecChildren.begin();
	CSulNode::VEC_NODE::const_iterator ie = vecChildren.end();
	while ( i!=ie )
	{
		CSulXmlNodeTag* childNode = dynamic_cast<CSulXmlNodeTag*>((*i).get());

		CSulXmlNodeTag* found = findTag( tag, childNode );
		if ( found )
			return found;

		++i;
	}

	return 0;
}

void CSulXmlReader::elementStart( const CSulString& sName, CSulXmlAttr* pAttr, CSulString sData ) 
{
	CSulXmlNodeTag* tag = new CSulXmlNodeTag( sName );
	tag->setAttr( *pAttr );

	if ( !m_root.valid() )
		m_root = tag;
	else
		m_curTag->addChild( tag );

	m_curTag = tag;
}

void CSulXmlReader::elementEnd( const CSulString& sName, CSulXmlAttr* pAttr )
{
	m_curTag = dynamic_cast<CSulXmlNodeTag*>(m_curTag->getParent());
}


