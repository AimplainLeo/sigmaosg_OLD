// ParserXml.cpp

#include <stdafx.h>
#include "ParserXml.h"
#include <SigmaUtil/SulXmlAttr.h>
#include <SigmaUtil/SulDataShapeMask.h>

CParserXml::CParserXml() :
m_bSuppressTextureTree( false ),
m_texUnit( 0 ),
m_suppressViewer( false ),
m_showPivots( false ),
m_useLights( 0 ),
m_minTree( 3.0f ),
m_maxTree( 3.0f )
{
}

void CParserXml::ElementStart( const CSulString& sName, CSulXmlAttr* pAttr )
{
	if ( sName=="GEN" )
	{
		m_useLights = strtoul( pAttr->get( "uselights" ).c_str(), 0, 16 );
	}

	if ( sName=="TEXTURE_TREE" )
	{
		m_bSuppressTextureTree = pAttr->get( "suppress" ).asBool();
		m_texUnit = pAttr->get( "unit" ).asUint32();
	}

	if ( sName=="TERRAIN" )
	{
		CSulString sTerrainFile = pAttr->get( "file" );
		if (!m_sInputFileOverride.empty())
		{
			sTerrainFile = m_sInputFileOverride;
		}
		float offsetX = pAttr->get( "offsetx" ).asFloat();
		float offsetY = pAttr->get( "offsety" ).asFloat();
		bool bRenderMe = pAttr->get( "renderme" ).asBool();
		m_rSceneTerrain = new CSceneTerrain( sTerrainFile, bRenderMe, offsetX, offsetY );

		// we create a lat lon reference point that the other flt files will be relative to
		m_rWorldUTM = new CSulCoordUTM( *m_rSceneTerrain->getCoordLatLonObject() );

		// adjust position of terrain
		m_rSceneTerrain->adjustPosition( m_rWorldUTM );

		// get boundingbox planes
		m_vecBoundingBoxPlanes = m_rSceneTerrain->getBoundingBoxPlanes();
	}

	if ( sName=="SHAPE" )
	{
		CSulString sShapeFile = pAttr->get( "file" );
		bool bRenderMe = pAttr->get( "renderme" ).asBool();

		m_rSceneShape = new CSceneShape( sShapeFile, bRenderMe );
		m_rSceneShape->adjustPosition( m_rWorldUTM );

		if ( pAttr->exist( "offsetz" ) )
		{
			m_rSceneShape->addOffset( 0, 0, pAttr->get( "offsetz" ).asFloat() );
		}

		// clip shape to scenes boundingbox planes
		m_rSceneShape->clip( m_vecBoundingBoxPlanes );
	}

	if ( sName=="MASK" )
	{
		CSulString sShapeFile = pAttr->get( "file" );
		bool bRenderMe = pAttr->get( "renderme" ).asBool();
		float lineDist = pAttr->get( "linedist" ).asFloat();

		CSceneShapeMask* pSceneShapeMask = new CSceneShapeMask( sShapeFile, bRenderMe, lineDist );
		pSceneShapeMask->adjustPosition( m_rWorldUTM );

		if ( pAttr->exist( "offsetz" ) )
		{
			pSceneShapeMask->addOffset( 0, 0, pAttr->get( "offsetz" ).asFloat() );
		}

		// clip mask to scenes boundingbox planes
		pSceneShapeMask->clip( m_vecBoundingBoxPlanes );

		m_vecShapeMasks.push_back( pSceneShapeMask );
	}

	if ( sName=="OUTPUT" )
	{
		if(m_sOutputFile.empty())
		{
			m_sOutputFile = pAttr->get( "file" );
		}
	}

	if ( sName=="TREE" )
	{
		m_radiusBetweenTrees = pAttr->get( "radius_between_trees" ).asFloat();
		m_distance_between_trees_line = pAttr->get( "distance_between_trees_line" ).asFloat();

		m_minTree = pAttr->get( "min" ).asFloat();
		m_maxTree = pAttr->get( "max" ).asFloat();
	}

	if ( sName=="VIEWER" )
	{
		m_suppressViewer = pAttr->get( "suppress" ).asBool();
	}

	if ( sName=="DEBUGGING" )
	{
		m_showPivots = pAttr->get( "showpivots" ).asBool();
	}

}

void CParserXml::SetInputFileOverride(const CSulString& filename)
{
	m_sInputFileOverride = filename;
}

void CParserXml::SetOutputFileOverride(const CSulString& filename)
{
	m_sOutputFile = filename;
}

CSulGenTextureWithPositions* CParserXml::getGen()
{
	return m_gen;
}

void CParserXml::LoadFinished()
{
	VEC_GENPOSITIONMASK vecMask;

	VEC_SHAPEMASK::iterator i;
	i = m_vecShapeMasks.begin();
	while ( i!=m_vecShapeMasks.end() )
	{
		CSulGenPositionMask* mask = new CSulGenPositionMask;
		mask->m_vecLineMask.insert( mask->m_vecLineMask.end(), (*i)->getClippedLineList().begin(), (*i)->getClippedLineList().end() );
		mask->m_vecTriMask.insert( mask->m_vecTriMask.end(), (*i)->getClippedTriangleList().begin(), (*i)->getClippedTriangleList().end() );
		mask->m_lineDist = (*i)->getLineDist();
		vecMask.push_back( mask );
		++i;
	}

	m_gen = new CSulGenTextureWithPositions(
		m_rSceneTerrain->getPat(),
		m_rSceneShape->getClippedLineList(),
		m_rSceneShape->getClippedTriangleList(),
		m_radiusBetweenTrees,
		m_distance_between_trees_line,
		vecMask
	);
}

bool CParserXml::isViewerSuppressed()
{
	return m_suppressViewer;
}

CSceneTerrain* CParserXml::getSceneTerrain()
{
	return m_rSceneTerrain;
}

CSceneShape* CParserXml::getSceneShape()
{
	return m_rSceneShape;
}

const VEC_SHAPEMASK& CParserXml::getShapeMaskList() const
{
	return m_vecShapeMasks;
}

bool CParserXml::isTextureSuppressed()
{
	return m_bSuppressTextureTree;
}

Sigma::uint32 CParserXml::getTexUnit()
{
	return m_texUnit;
}

const CSulString& CParserXml::getOutputFile() const
{
	return m_sOutputFile;
}

Sigma::uint32 CParserXml::getUseLights()
{
	return m_useLights;
}

float CParserXml::getMinTree()
{
	return m_minTree;
}

float CParserXml::getMaxTree()
{
	return m_maxTree;
}
