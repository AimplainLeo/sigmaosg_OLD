// SulXmlReader.h

#ifndef __SULXMLREADER_H__
#define __SULXMLREADER_H__

#include "SulXmlParser.h"
#include "SulXmlNodeTag.h"

class CSulXmlReader : public CSulXmlParser
{
public:
	typedef std::vector< osg::ref_ptr<CSulXmlNodeTag> > VEC_XMLNODE;
	
public:
	CSulXmlReader();

	CSulXmlNodeTag* findTag( const CSulString& tag, CSulXmlNodeTag* searchTag=0 );

	void findTags( const CSulString& tag, VEC_XMLNODE& vecXmlNodes, CSulXmlNodeTag* searchTag=0 );

protected:
	virtual void	elementStart( const CSulString& sName, CSulXmlAttr* pAttr, CSulString sData );
	virtual void	elementEnd( const CSulString& sName, CSulXmlAttr* pAttr );

private:
	osg::ref_ptr<CSulXmlNodeTag>	m_root;
	CSulXmlNodeTag*					m_curTag;
};

#endif // __SULXMLREADER_H_