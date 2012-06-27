// SulSceneManagerXml.h

#ifndef __SULSCENEMANAGERXML_H__
#define __SULSCENEMANAGERXML_H__

#include "SulExport.h"
#include "SulTypes.h"
#include "SulSceneManager.h"
#include "SulXmlParser.h"
#include "SulProgramShaders.h"
#include <osg/AlphaFunc>

class SUL_EXPORT CSulSceneManagerXml : public CSulXmlParser
{
private:
	typedef std::vector< osg::Node* >								VECTOR_NODESTACK;
	typedef std::map< CSulString, osg::ref_ptr<osg::Texture2D> >	MAP_TEXTURE2D;
	typedef std::map< CSulString, osg::ref_ptr<osg::Node> >			MAP_NODE;
	typedef std::map< CSulString, osg::ref_ptr<CSulProgramShaders> >	MAP_PROGRAMSHADERS;

public:
										CSulSceneManagerXml( CSulSceneManager* pSceneManager );

	virtual void						elementStart( const CSulString& sName, CSulXmlAttr* pAttr, CSulString sData );
	virtual void						elementEnd( const CSulString& sName );

	void								addTexture2D( const CSulString& sName, osg::Texture2D* tex );
	osg::Texture2D*						getTexture2D( const CSulString& sName );

	void								addProgramShaders( const CSulString& sName, CSulProgramShaders* programShaders );
	CSulProgramShaders*					getProgramShaders( const CSulString& sName );
	
	void								add( CSulString sName, osg::Node* node );
	osg::Node*							get( const CSulString& sName );

protected:
	osg::StateAttribute::Values			getStateAttribute( const CSulString& s );
	osg::AlphaFunc::ComparisonFunction	GetAlphaFuncFunction(const CSulString& s );
	osg::StateAttribute::GLMode			GetStateMode( const CSulString& s );

	osg::Node*							getLastNode();

protected:
	osg::ref_ptr<CSulSceneManager>		m_rSceneManager;

	VECTOR_NODESTACK					m_vecNodeStack;
	CSulString							m_sLastNodeName;
	osg::StateSet*						m_pCurrentStateSet;

	MAP_TEXTURE2D						m_mapTexture2D;
	MAP_NODE							m_mapNode;
	MAP_PROGRAMSHADERS					m_mapProgramShaders;
};

#endif // __SULSCENEMANAGERXML_H__