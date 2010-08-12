// SulSceneManagerXml.cpp

#include "stdafx.h"
#include "SulSceneManagerXml.h"
#include "SulXmlAttr.h"
#include "SulParser.h"
#include <osg/Fog>

CSulSceneManagerXml::CSulSceneManagerXml( CSulSceneManager* pSceneManager ):
m_pCurrentStateSet(0)
{
	m_rSceneManager = pSceneManager;
	m_vecNodeStack.push_back( 0 );
}

osg::StateAttribute::Values CSulSceneManagerXml::GetStateAttribute( const CSulString& s )
{
	osg::ref_ptr<CSulParser> rParser = new CSulParser;
	rParser->AddCharDelimiter( '|' );
	rParser->InitStringParse( s );

	sigma::uint32 v = osg::StateAttribute::OFF;

	sigma::int8* pToken = 0;
	while ( pToken = rParser->GetToken() )
	{
		if ( !strcmp( pToken, "on" ) )			v|= osg::StateAttribute::ON;	
		if ( !strcmp( pToken, "override" ) )	v|= osg::StateAttribute::OVERRIDE;
		if ( !strcmp( pToken, "protected" ) )	v|= osg::StateAttribute::PROTECTED;
		if ( !strcmp( pToken, "inherit" ) )		v|= osg::StateAttribute::INHERIT;
	}

	return (osg::StateAttribute::Values)v;
}

osg::StateAttribute::GLMode CSulSceneManagerXml::GetStateMode( const CSulString& s )
{
	if ( s == "fog" )
	{
		return GL_FOG;
	}
	else if ( s == "alpha_test" )
	{
		return GL_ALPHA_TEST;
	}
	else if ( s == "blend" )
	{
		return GL_BLEND;
	}
	else if ( s == "lighting" )
	{
		return GL_LIGHTING;
	}
	else if ( s.find("light") != std::string::npos )
	{
		int lightNum = atoi(&s[5]);
		if ( lightNum >= 0 && lightNum <=7 )
		{
			return GL_LIGHT0 + lightNum;
		}
	}
	return 0;
}

osg::AlphaFunc::ComparisonFunction CSulSceneManagerXml::GetAlphaFuncFunction( const CSulString& s )
{
	if ( s == "never" )
	{
		return osg::AlphaFunc::NEVER;
	}
	else if ( s == "less" )
	{
		return osg::AlphaFunc::LESS;
	}
	else if ( s == "equal" )
	{
		return osg::AlphaFunc::EQUAL;
	}
	else if ( s == "lequal" )
	{
		return osg::AlphaFunc::LEQUAL;
	}
	else if ( s == "greater" )
	{
		return osg::AlphaFunc::GREATER;
	}
	else if ( s == "notequal" )
	{
		return osg::AlphaFunc::NOTEQUAL;
	}
	else if ( s == "gequal" )
	{
		return osg::AlphaFunc::GEQUAL;
	}
	else if ( s == "always" )
	{
		return osg::AlphaFunc::ALWAYS;
	}
	else
	{
		osg::notify(osg::WARN) << "WARNING: CSulSceneManagerXml::GetAlphaFuncFunction -> unknown alphafunc comparison function: ["<<s<<"] Choosing [greater]" << std::endl;
		return osg::AlphaFunc::GREATER;
	}
}

osg::Node* CSulSceneManagerXml::GetLastNode()
{
	if ( !m_vecNodeStack.size() )
	{
		return 0;
	}

	return m_vecNodeStack[m_vecNodeStack.size()-1];
}

void CSulSceneManagerXml::elementStart( const CSulString& sName, CSulXmlAttr* pAttr )
{
	if ( sName=="NODE" )
	{
		// create group
		m_sLastNodeName = pAttr->get( "name" );
		osg::Group* pParentGroup = dynamic_cast<osg::Group*>(m_vecNodeStack[m_vecNodeStack.size()-1]);
		m_rSceneManager->Create( m_sLastNodeName, pParentGroup );
		osg::Node* pNode = m_rSceneManager->Get( m_sLastNodeName );
		m_vecNodeStack.push_back( pNode );

		// StateSet
		if ( pAttr->exist("stateset") )
		{
			CSulString name = pAttr->get("stateset");
			if ( pNode->getStateSet() )
			{
				osg::notify(osg::WARN) << "WARNING: CSulSceneManagerXml::elementStart -> non-empty stateset on node ["<<pNode->getName()<<"] will be overridden!" << std::endl;
			}
			pNode->setStateSet( m_rSceneManager->GetStateSet(name) );
		}
	}

	if ( sName=="ADD" )
	{
		// get the name of the object group we want to add
		CSulString s = pAttr->get( "name" );
		osg::Node* pNode = m_rSceneManager->Get( s );
		if ( !pNode )
		{
			osg::notify(osg::WARN)<<"WARNING: CSulSceneManagerXml::elementStart -> ADD pNode ["<<s<<"] does not exist"<<std::endl;
		}

		// @PWM the m_sLastNodeName is not the current parent node! it's the name of the node last created!
		//m_rSceneManager->AddToGroup( m_sLastNodeName, pNode );
		osg::Group* pGroup = dynamic_cast<osg::Group*>(GetLastNode());
		pGroup->addChild( pNode );
	}

	if ( sName=="ADD_ATTR" )
	{
		if ( !m_pCurrentStateSet )
		{
			osg::notify(osg::WARN) << "WARNING: CSulSceneManagerXml::elementStart -> ADD_ATTR: can only be added inside a stateset" << std::endl;
		}
		else
		{
			CSulString sAttrName = pAttr->get( "name" );
			m_pCurrentStateSet->setAttributeAndModes( m_rSceneManager->getAttribute( sAttrName ) );
		}
	}

	if ( sName=="STATESET" )
	{
		CSulString s = pAttr->get( "name" );
		m_rSceneManager->AddStateSet( s, m_pCurrentStateSet = new osg::StateSet );
	}

	if ( sName=="FOG" )
	{
		osg::Fog* pFog = new osg::Fog;
		
		// @PWM: currentstateset is a bad design.. i'm changing this so statesets can add attributes instead
		//m_pCurrentStateSet->setAttributeAndModes(pFog);
		
		pFog->setColor( osg::Vec4(1,1,1,1) );

		CSulString sMode;
		if ( pAttr->get( "mode", sMode ) )
		{
			if ( sMode=="linear" )
			{
				pFog->setMode( osg::Fog::LINEAR );
			}
			else if ( sMode=="exp" )
			{
				pFog->setMode( osg::Fog::EXP );
			}
			else if ( sMode=="exp2" )
			{
				pFog->setMode( osg::Fog::EXP2 );
			}
			else
			{
				osg::notify(osg::WARN) << "WARNING: CSulSceneManagerXml::elementStart -> FOG: Unknown mode" << std::endl;
			}
		}

		m_rSceneManager->addAttribute( pAttr->get("name"), pFog );
	}

	if ( m_pCurrentStateSet && sName=="MODE" )
	{
		CSulString sMode, sValue;
		if( pAttr->get("mode", sMode) && pAttr->get("value", sValue) )
		{
			osg::StateAttribute::GLMode mode = GetStateMode(sMode);
			if ( mode == 0 )
			{
				osg::notify(osg::WARN) << "WARNING: CSulSceneManagerXml::elementStart -> MODE: Unknown mode" << std::endl;
			}
			else
			{
				m_pCurrentStateSet->setMode( mode, GetStateAttribute(sValue) );
			}
		}
	}

	if ( m_pCurrentStateSet && sName=="ALPHAFUNC" )
	{
		CSulString sComparisonFunction;
		float fValue;
		if ( pAttr->get("comparisonfunction", sComparisonFunction) && pAttr->get("value", fValue) )
		{
			osg::AlphaFunc* pAlphaFunc = new osg::AlphaFunc;
			pAlphaFunc->setFunction( GetAlphaFuncFunction(sComparisonFunction) );
			pAlphaFunc->setReferenceValue( fValue );
			m_pCurrentStateSet->setAttribute( pAlphaFunc, osg::StateAttribute::PROTECTED | osg::StateAttribute::ON );
		}
	}

	if ( m_pCurrentStateSet && sName=="RENDERBIN" )
	{
		sigma::uint32 binNum;
		CSulString sBinName;
		if( pAttr->get("number", binNum) && pAttr->get("name", sBinName) )
		{
			m_pCurrentStateSet->setRenderBinDetails(binNum, sBinName);
		}
	}

	if ( m_pCurrentStateSet && sName=="UNIFORM" )
	{
		osg::Uniform* uniform = 0;

		CSulString sUniformName = pAttr->get( "name" );
		
		CSulString type = pAttr->get( "type" );
		
		if ( type=="INT" )
		{
			uniform = new osg::Uniform( sUniformName.c_str(), pAttr->get( "value" ).asInt32() );
		}

		m_pCurrentStateSet->addUniform( uniform );
	}
}

void CSulSceneManagerXml::elementEnd( const CSulString& sName )
{
	if ( sName=="NODE" )
	{
		m_vecNodeStack.pop_back();
	}

	if ( sName=="STATESET" )
	{
		m_pCurrentStateSet = 0;
	}
}

