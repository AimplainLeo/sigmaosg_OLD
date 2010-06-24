// SulSceneManagerXml.h

#ifndef __SULSCENEMANAGERXML_H__
#define __SULSCENEMANAGERXML_H__

#include "SulExport.h"
#include "SulTypes.h"
#include "SulSceneManager.h"
#include "SulXmlParser.h"
#include <osg/AlphaFunc>

class SUL_EXPORT CSulSceneManagerXml : public CSulXmlParser
{
private:
	typedef std::vector< osg::Node* >	VECTOR_NODESTACK;

public:
										CSulSceneManagerXml( CSulSceneManager* pSceneManager );

	virtual void						ElementStart( const CSulString& sName, CSulXmlAttr* pAttr );
	virtual void						ElementEnd( const CSulString& sName );

protected:
	osg::StateAttribute::Values			GetStateAttribute( const CSulString& s );
	osg::AlphaFunc::ComparisonFunction	GetAlphaFuncFunction(const CSulString& s );
	osg::StateAttribute::GLMode			GetStateMode( const CSulString& s );

	osg::Node*							GetLastNode();

protected:
	osg::ref_ptr<CSulSceneManager>		m_rSceneManager;

	VECTOR_NODESTACK					m_vecNodeStack;
	CSulString							m_sLastNodeName;
	osg::StateSet*						m_pCurrentStateSet;
};

#endif // __SULSCENEMANAGERXML_H__