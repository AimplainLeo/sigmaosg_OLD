// SulSceneManagerXml.cpp

#include "stdafx.h"
#include "SulSceneManagerXml.h"
#include "SulXmlAttr.h"
#include "SulParser.h"
#include "SulStringList.h"
#include "SulTexCam.h"
#include "SulScreenAlignedQuad.h"
#include "SulDeferredCamera.h"
#include "SulTestGeoms.h"
#include "SulGeodeSphere.h"
#include <osg/Fog>
#include <osgText/Text>
#include <iostream>

CSulSceneManagerXml::CSulSceneManagerXml( CSulSceneManager* pSceneManager ):
m_pCurrentStateSet(0)
{
	m_rSceneManager = pSceneManager;
	m_vecNodeStack.push_back( 0 );
}

osg::StateAttribute::Values CSulSceneManagerXml::getStateAttribute( const CSulString& s )
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

osg::Node* CSulSceneManagerXml::getLastNode()
{
	if ( !m_vecNodeStack.size() )
	{
		return 0;
	}

	return m_vecNodeStack[m_vecNodeStack.size()-1];
}

void CSulSceneManagerXml::addTexture2D( const CSulString& sName, osg::Texture2D* tex )
{
	m_mapTexture2D[sName] = tex;
}

osg::Texture2D* CSulSceneManagerXml::getTexture2D( const CSulString& sName )
{
	if ( m_mapTexture2D.find( sName )==m_mapTexture2D.end() )
	{
		return 0;
	}

	return m_mapTexture2D[sName];
}

void CSulSceneManagerXml::add( CSulString sName, osg::Node* node )
{
	// make sure the name does not already exist
	if ( m_mapNode.find( sName )!=m_mapNode.end() )
	{
		std::cout << "ERROR: CSulSceneManager::add -> name given already exists: " << sName << std::endl;
		return;
	}

	m_sLastNodeName = sName;
	osg::Group* pParentGroup = dynamic_cast<osg::Group*>(m_vecNodeStack[m_vecNodeStack.size()-1]);
	m_rSceneManager->Create( m_sLastNodeName, pParentGroup, node );
	m_vecNodeStack.push_back( node );
	m_mapNode[sName] = node;
}

osg::Node* CSulSceneManagerXml::get( const CSulString& sName )
{
	return m_mapNode[sName];
}

void CSulSceneManagerXml::addProgramShaders( const CSulString& sName, CSulProgramShaders* programShaders )
{
	m_mapProgramShaders[sName] = programShaders;
}

CSulProgramShaders* CSulSceneManagerXml::getProgramShaders( const CSulString& sName )
{
	return m_mapProgramShaders[sName];
}

void CSulSceneManagerXml::elementStart( const CSulString& sName, CSulXmlAttr* pAttr, CSulString sData )
{
	if ( sName=="CAMERA" )
	{
		CSulString sN = pAttr->get( "name" );

		osg::Camera* camera = new osg::Camera;
		add( sN, camera );

		bool bAddToRoot = pAttr->getBool( "add_to_root", false );
		if ( bAddToRoot )
		{
			m_vecRootNodes.push_back( camera );
		}

		// StateSet
		if ( pAttr->exist("stateset") )
		{
			CSulString name = pAttr->get("stateset");
			if ( camera->getStateSet() )
			{
				osg::notify(osg::WARN) << "WARNING: CSulSceneManagerXml::elementStart -> non-empty stateset on node ["<<camera->getName()<<"] will be overridden!" << std::endl;
			}
			camera->setStateSet( m_rSceneManager->GetStateSet(name) );
		}

		//////////////////////////////////////////////
		// background color
		//////////////////////////////////////////////

		osg::Vec4 bgcolor = pAttr->getVec4( "bgcolor", osg::Vec4(0,0,0,0) );
		camera->setClearColor( bgcolor );

		//////////////////////////////////////////////
		// compute near far mode
		//////////////////////////////////////////////

		CSulString sNearFar = pAttr->getString( "computer_near_far_mode", "COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES" );
		if ( sNearFar=="COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES" )
			camera->setComputeNearFarMode( osg::CullSettings::COMPUTE_NEAR_FAR_USING_BOUNDING_VOLUMES );
		else if ( sNearFar=="COMPUTE_NEAR_FAR_USING_PRIMITIVES" )
			camera->setComputeNearFarMode( osg::CullSettings::COMPUTE_NEAR_FAR_USING_PRIMITIVES );
		else
			camera->setComputeNearFarMode( osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR );

		//////////////////////////////////////////////
		// render order
		//////////////////////////////////////////////

		CSulString sRenderOrder = pAttr->getString( "render_order", "POST_RENDER" );
		sigma::int32 ron		= pAttr->getInt32( "render_order_num", 0 );
		osg::Camera::RenderOrder ro = osg::Camera::POST_RENDER;
		if ( sRenderOrder=="PRE_RENDER" )
			ro = osg::Camera::PRE_RENDER;
		else if ( sRenderOrder=="NESTED_RENDER" )
			ro = osg::Camera::NESTED_RENDER;
		camera->setRenderOrder( ro, ron );

		//////////////////////////////////////////////
		// clear masks
		//////////////////////////////////////////////

		sigma::uint32 clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

		// clear z buffer
		bool clearz = pAttr->getBool( "clearz", true );
		clearMask = clearMask & (~GL_DEPTH_BUFFER_BIT);
		if ( clearz )
			clearMask |= GL_DEPTH_BUFFER_BIT;

		// clear background color
		bool clearbg = pAttr->getBool( "clearbg", true );
		clearMask = clearMask & (~GL_COLOR_BUFFER_BIT);
		if ( clearbg )
			clearMask |= GL_COLOR_BUFFER_BIT;

		camera->setClearMask( clearMask );

		//////////////////////////////////////////////
		// projection
		//////////////////////////////////////////////

		double fovy = pAttr->getDouble( "fovy", 45.0 );
		double near = pAttr->getDouble( "near", 0.1 );
		double far = pAttr->getDouble( "far", 10000 );
		double aspect = pAttr->getDouble( "aspect", 4.0/3.0 );
		camera->setProjectionMatrixAsPerspective( fovy, aspect, near, far );

		//////////////////////////////////////////////
		// viewmatrix
		//////////////////////////////////////////////

		osg::Vec3 eye = pAttr->getVec3( "eye", osg::Vec3(0,0,0) );
		osg::Vec3 center = pAttr->getVec3( "center", osg::Vec3(0,10,0.1) );
		osg::Vec3 up = pAttr->getVec3( "up", osg::Vec3(0,0,1) );
		camera->setViewMatrixAsLookAt( eye, center, up );

		//////////////////////////////////////////////
		// reference frame
		//////////////////////////////////////////////

		CSulString sRefFrame = pAttr->getString( "reference_frame", "RELATIVE_RF" );
		osg::Transform::ReferenceFrame rf = osg::Transform::RELATIVE_RF;
		if ( sRefFrame=="ABSOLUTE_RF" )
			rf = osg::Transform::ABSOLUTE_RF;
		else if ( sRefFrame=="ABSOLUTE_RF_INHERIT_VIEWPOINT" )
			rf = osg::Transform::ABSOLUTE_RF_INHERIT_VIEWPOINT;
		camera->setReferenceFrame( rf );
	}

	if ( sName=="SCREENLABEL" )
	{
		osg::Camera* camera = new osg::Camera;
		camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		camera->setProjectionMatrixAsOrtho2D(0,800,0,600);
		camera->setViewMatrix(osg::Matrix::identity());
		camera->setClearMask(GL_DEPTH_BUFFER_BIT);
		camera->getOrCreateStateSet()->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
		
		osg::Geode* geode = new osg::Geode;
		camera->addChild( geode );

		CSulString sN = pAttr->get( "name" );
		CSulString sT = pAttr->get( "text" );

		add( sN, camera );

		osgText::Text* t = new  osgText::Text;
		t->setColor( osg::Vec4( 1.0f, 1.0f, 0.0f, 1.0f ) );
		t->setText( sT );
		t->setBackdropType( osgText::Text::OUTLINE );
		geode->addDrawable( t );

		float x, y;
		x = pAttr->getFloat( "x", 0.0f );
		y = pAttr->getFloat( "y", 0.0f );
		t->setPosition( osg::Vec3( x, 600-y, 0.0f ) );
	}

	if ( sName=="SULPROGRAMSHADERS" )
	{
		CSulString sN = pAttr->get( "name" );

		CSulString shaders = pAttr->get( "shaders" );
		CSulProgramShaders* programShaders = new CSulProgramShaders( shaders );
		addProgramShaders( sN, programShaders );

		programShaders->setParameter( GL_GEOMETRY_VERTICES_OUT_EXT, 6 );
		programShaders->setParameter( GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES );
		programShaders->setParameter( GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_LINE_STRIP );

		m_rSceneManager->addAttribute( sN, programShaders );
	}

	if ( sName=="TEXTURE2D" )
	{
		sigma::uint32 w = pAttr->getUint32( "w", 256 );
		sigma::uint32 h = pAttr->getUint32( "h", 256 );
		bool resizePowerTwoHint = pAttr->getBool( "ResizeNonPowerOfTwoHint", false );

		sigma::uint32 internalFormat = GL_RGBA;
		if ( pAttr->exist( "InternalFormat" ) )
		{
			CSulString s = pAttr->get( "InternalFormat" );
			if ( s=="GL_DEPTH_COMPONENT" )
				internalFormat = GL_DEPTH_COMPONENT;
		}

		osg::Texture2D* tex = new osg::Texture2D;
		tex->setTextureSize( w, h );
		tex->setInternalFormat( internalFormat );
		tex->setResizeNonPowerOfTwoHint( resizePowerTwoHint );

		CSulString sN = pAttr->get( "name" );
		addTexture2D( sN, tex );
	}

	if ( sName=="TEXCAM" )
	{
		osg::Group* pParentGroup = dynamic_cast<osg::Group*>(m_vecNodeStack[m_vecNodeStack.size()-1]);

		CSulString sN = pAttr->get( "name" );
		sigma::uint32 w = pAttr->getUint32( "w", 256 );
		sigma::uint32 h = pAttr->getUint32( "h", 256 );

		CSulTexCam* texcam = new CSulTexCam( w, h );
		add( sN, texcam );

		if ( pAttr->exist("attach_depth_buffer") )
		{
			osg::Texture2D* tex = getTexture2D( pAttr->get("attach_depth_buffer") );
			assert( tex );
			texcam->attach( osg::Camera::DEPTH_BUFFER, tex );
		}

		if ( pAttr->exist("attach_color_buffer") )
		{
			osg::Texture2D* tex = getTexture2D( pAttr->get("attach_color_buffer") );
			assert( tex );
			texcam->attach( osg::Camera::COLOR_BUFFER, tex );
		}
	
		sigma::uint32 clearMask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
		if ( pAttr->exist("clearz") )
		{
			// remove the bit
			clearMask = clearMask & (~GL_DEPTH_BUFFER_BIT);
			if ( pAttr->get( "clearz" ).asBool() )
				clearMask |= GL_DEPTH_BUFFER_BIT;
		}
		if ( pAttr->exist("clearbg") )
		{
			// remove the bit
			clearMask = clearMask & (~GL_COLOR_BUFFER_BIT);
			if ( pAttr->get( "clearbg" ).asBool() )
				clearMask |= GL_COLOR_BUFFER_BIT;
		}
		texcam->setClearMask( clearMask );

		if ( pAttr->exist( "compute_near_far" ) )
		{
			if ( !pAttr->get("compute_near_far").asBool() )
				texcam->setComputeNearFarMode( osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR );
		}

		if ( pAttr->exist("bgcolor") )
		{
			osg::Vec4 c = pAttr->get("bgcolor").asVec4();
			texcam->setClearColor( c );
		}

		// debug bar
		//texcam->addChild( new CSulScreenAlignedQuad( 800, 600, 0, 0, 100, 800, 50 ) );		// bar over
	}

	if ( sName=="SCREENQUAD" )
	{
		CSulString sN = pAttr->get( "name" );

		sigma::uint32 x = pAttr->getUint32( "x", 0 );
		sigma::uint32 y = pAttr->getUint32( "y", 0 );
		sigma::uint32 z = pAttr->getUint32( "z", 0 );
		sigma::uint32 w = pAttr->getUint32( "w", 256 );
		sigma::uint32 h = pAttr->getUint32( "h", 256 );

		// terrible way of  handling the view sizes, but I have no idea how to make this better
		sigma::uint32 view_w = pAttr->getUint32( "view_w", 1920 );
		sigma::uint32 view_h = pAttr->getUint32( "view_h", 1080 );

		osg::Texture2D* tex = 0;
		if ( pAttr->exist( "texture" ) )
		{
			tex = getTexture2D( pAttr->get("texture") );
			if ( !tex )
			{
				osg::Node* node = get( pAttr->get("texture") );
				if ( node )
				{
					CSulTexCam* texcam = dynamic_cast<CSulTexCam*>(node);
					tex = texcam->getTexture(0);
				}
			}
		}

		CSulScreenAlignedQuad* quad = new CSulScreenAlignedQuad( view_w, view_h, tex, osg::Vec3(x,y,z), w, h );
		add( sN, quad );

		if ( pAttr->exist( "texture_file" ) )
		{
			quad->setTexture( pAttr->get("texture_file") );
			quad->getGeode()->enableBlend();
		}

		if ( pAttr->exist( "renderbin_num" ) )
		{
			sigma::uint32 i = pAttr->get( "renderbin_num" ).asUint32();
			quad->getOrCreateStateSet()->setRenderBinDetails( i, "RenderBin" );
		}

		// StateSet
		if ( pAttr->exist("stateset") )
		{
			CSulString name = pAttr->get("stateset");
			if ( quad->getStateSet() )
			{
				osg::notify(osg::WARN) << "WARNING: CSulSceneManagerXml::elementStart -> non-empty stateset on node ["<<quad->getName()<<"] will be overridden!" << std::endl;
			}
			quad->setStateSet( m_rSceneManager->GetStateSet(name) );
		}
	}

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

		if ( pAttr->exist("mask") )
		{
			pNode->setNodeMask( m_rSceneManager->calcCullMask( pAttr->get("mask") ) );
		}

		if ( pAttr->exist( "renderbin_num" ) )
		{
			unsigned long i = pAttr->get( "renderbin_num" ).asUint32();
			pNode->getOrCreateStateSet()->setRenderBinDetails( i, "RenderBin" );
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
		osg::Group* pGroup = dynamic_cast<osg::Group*>(getLastNode());
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

		if ( pAttr->exist( "merge" ) )
		{
			CSulString ss = pAttr->get( "merge" );

			// split the string into it parts
			osg::ref_ptr<CSulStringList> rList = new CSulStringList( ss );
			CSulStringList::VECTOR_STRING::const_iterator iS = rList->getList().begin();
			CSulStringList::VECTOR_STRING::const_iterator iE = rList->getList().end();

			// merge each part into the current stateset
			while ( iS!=iE )
			{
				CSulString s = *iS;

				osg::StateSet* pSS = m_rSceneManager->GetStateSet( s );
				if ( !pSS )
				{
					osg::notify(osg::WARN) << "WARNING: CSulSceneManagerXml::elementStart -> STATESET: can not merge stateset [" << s << "]" << std::endl;
					++iS;
					continue;
				}

				m_pCurrentStateSet->merge( *pSS );

				++iS;
			}
		}
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
				m_pCurrentStateSet->setMode( mode, getStateAttribute(sValue) );
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
		else if ( type=="FLOAT" )
		{
			uniform = new osg::Uniform( sUniformName.c_str(), pAttr->get( "value" ).asFloat() );
		}
		else if ( type=="BOOL" )
		{
			uniform = new osg::Uniform( sUniformName.c_str(), pAttr->get( "value" ).asBool() );
		}
		else if ( type=="SAMPLER_2D" )
		{
			uniform = new osg::Uniform( osg::Uniform::SAMPLER_2D, sUniformName.c_str() );
			uniform->set( pAttr->get( "value" ).asInt32() );
		}

		osg::StateAttribute::Values stateValue = osg::StateAttribute::ON;
		if ( pAttr->exist( "state" ) )
		{
			stateValue = getStateAttribute( pAttr->get( "state" ) );
		}

		m_pCurrentStateSet->addUniform( uniform, stateValue );

		m_mapUniform[sUniformName] = uniform;
	}
}

void CSulSceneManagerXml::elementEnd( const CSulString& sName )
{
	if ( sName=="CAMERA" )
		m_vecNodeStack.pop_back();


	if ( sName=="SCREENLABEL" )
		m_vecNodeStack.pop_back();

	if ( sName=="TEXCAM" )
	{
		m_vecNodeStack.pop_back();
	}

	if ( sName=="SCREENQUAD" )
	{
		m_vecNodeStack.pop_back();
	}

	if ( sName=="NODE" )
	{
		m_vecNodeStack.pop_back();
	}

	if ( sName=="STATESET" )
	{
		m_pCurrentStateSet = 0;
	}
}

osg::Uniform* CSulSceneManagerXml::getUniform( const CSulString& name )
{
	return m_mapUniform[name];
}