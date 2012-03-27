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
m_maxTree( 3.0f ),
m_bColGeom( false ),
m_bIgnoreGeo( false ),
m_bSuppressShaders( false ),
m_bCells( false ),
m_lodDist_min( 0.0f ),
m_lodDist_max( 0.0f ),
m_bSun( false )
{
}

void CParserXml::elementStart( const CSulString& sName, CSulXmlAttr* pAttr )
{
	if ( sName=="CELL" )
	{
		CSulString sx = pAttr->get( "cx" );
		CSulString sy = pAttr->get( "cy" );
		m_cellxy.set( sx.asFloat(), sy.asFloat() );
		m_bCells = true;

		if ( pAttr->exist( "lod_dist_min" ) && pAttr->exist( "lod_dist_max" ) )
		{
			m_lodDist_min = pAttr->get( "lod_dist_min" ).asFloat();
			m_lodDist_max = pAttr->get( "lod_dist_max" ).asFloat();
		}

		if ( pAttr->exist( "json" ) )
		{
			m_sCellJson = pAttr->get( "json" );
		}

		if ( pAttr->exist( "debug" ) )
		{
			m_bUseCellDebug = pAttr->get( "debug" ).asBool();
		}
	}

	if ( sName=="SHADER" )
	{
		CSulString sType = pAttr->get( "type" );
		CSulString sFile = pAttr->get( "file" );

		if ( !m_rShaderProgram.valid() )
			m_rShaderProgram = new osg::Program;

		osg::Shader* p = 0;

		if ( sType=="vertex" )
		{
			p = new osg::Shader( osg::Shader::VERTEX );
		}

		if ( sType=="fragment" )
		{
			p = new osg::Shader( osg::Shader::FRAGMENT );
		}

		p->loadShaderSourceFromFile( sFile );
		m_rShaderProgram->addShader( p );
	}

	if ( sName=="GEN" )
	{
		m_useLights			= strtoul( pAttr->get( "uselights" ).c_str(), 0, 2 );
		m_areaPadding		= pAttr->get( "areapadding" ).asFloat();		
		m_bColGeom			= pAttr->get( "col_geom" ).asBool();
		m_bIgnoreGeo		= pAttr->get( "no_geo" ).asBool();
		m_bSuppressShaders	= pAttr->get( "suppress_shaders" ).asBool();
	}

	if ( sName=="TEXTURE_TREE" )
	{
		m_bSuppressTextureTree = pAttr->get( "suppress" ).asBool();
		m_texUnit = pAttr->get( "unit" ).asUint32();
	}

	if ( sName=="TERRAIN" )
	{
		// if bDebug is true it means that there will be no file associated with the terrain, but a user generated grid
		bool bDebug = false;
		if ( pAttr->exist( "debug" ) )
			bDebug = pAttr->get( "debug" ).asBool();

		float offsetX = pAttr->get( "offsetx" ).asFloat();
		float offsetY = pAttr->get( "offsety" ).asFloat();
		bool bRenderMe = pAttr->get( "renderme" ).asBool();

		if ( !bDebug )
		{
			CSulString sTerrainFile = pAttr->get( "file" );
			if (!m_sInputFileOverride.empty())
			{
				sTerrainFile = m_sInputFileOverride;
			}
			m_rSceneTerrain = new CSceneTerrain( sTerrainFile, bRenderMe, offsetX, offsetY );
		}
		else
		{
			sigma::uint32 gx = pAttr->get( "gridx" ).asUint32();
			sigma::uint32 gy = pAttr->get( "gridy" ).asUint32();
			float cellw = pAttr->get( "cellw" ).asFloat();
			float cellh = pAttr->get( "cellw" ).asFloat();
			sigma::uint32 subx = pAttr->get( "subx" ).asUint32();
			sigma::uint32 suby = pAttr->get( "suby" ).asUint32();

			// debug
			m_rSceneTerrain = new CSceneTerrain( gx, gy, cellw, cellh, subx, suby, bRenderMe, offsetX, offsetY );
		}

		if ( !m_bIgnoreGeo )
		{
			// we create a lat lon reference point that the other flt files will be relative to
			m_rWorldUTM = new CSulCoordUTM( *m_rSceneTerrain->getCoordLatLonObject() );

			// adjust position of terrain
			m_rSceneTerrain->adjustPosition( m_rWorldUTM );
		}

		// get boundingbox planes
		m_vecBoundingBoxPlanes = m_rSceneTerrain->getBoundingBoxPlanes();
	}

	if ( sName=="SHAPE_QUAD" )
	{
		float w = pAttr->get( "w" ).asFloat();
		float h = pAttr->get( "h" ).asFloat();
		bool bRenderMe = pAttr->get( "renderme" ).asBool();

		m_rSceneShape = new CSceneShape( w, h, bRenderMe );

		if ( pAttr->exist( "offsetz" ) )
		{
			m_rSceneShape->addOffset( 0, 0, pAttr->get( "offsetz" ).asFloat() );
		}

		// clip shape to scenes boundingbox planes
		m_rSceneShape->clip( m_vecBoundingBoxPlanes );
	}

	if ( sName=="SHAPE" )
	{
		CSulString sShapeFile = pAttr->get( "file" );
		bool bRenderMe = pAttr->get( "renderme" ).asBool();

		m_rSceneShape = new CSceneShape( sShapeFile, bRenderMe );
		
		if ( !m_bIgnoreGeo )
		{
			m_rSceneShape->adjustPosition( m_rWorldUTM );
		}

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

		if ( !m_bIgnoreGeo )
		{
			pSceneShapeMask->adjustPosition( m_rWorldUTM );
		}

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

		m_bSun = pAttr->exist( "sun" )?pAttr->get( "sun" ).asBool():false;
	}

	if ( sName=="IGNORE_NODE" )
	{
		m_vecNodeIgnoreList.push_back( pAttr->get("name") );
	}

	if ( sName=="UNIFORM" )
	{
		CSulString sName = pAttr->get( "name" );
		CSulString sValue = pAttr->get( "value" );
		CSulString sType = pAttr->get( "type" );

		CUniformData* u = new CUniformData( sName, sValue, sType );

		m_vecUniformList.push_back( u );
	}
}

const CParserXml::VEC_UNIFORMDATA& CParserXml::getUniformDataList()
{
	return m_vecUniformList;
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

void CParserXml::loadFinished()
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
		m_rSceneTerrain.valid()?m_rSceneTerrain->getPat():0,
		m_rSceneShape->getClippedLineList(),
		m_rSceneShape->getClippedTriangleList(),
		m_radiusBetweenTrees,
		m_distance_between_trees_line,
		m_areaPadding,
		vecMask,
		m_vecNodeIgnoreList
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

sigma::uint32 CParserXml::getTexUnit()
{
	return m_texUnit;
}

const CSulString& CParserXml::getOutputFile() const
{
	return m_sOutputFile;
}

sigma::uint32 CParserXml::getUseLights()
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

bool CParserXml::hasColGeom()
{
	return m_bColGeom;
}

bool CParserXml::getSuppressShaders()
{
	return m_bSuppressShaders;
}

osg::Program* CParserXml::getProgram()
{
	return m_rShaderProgram;
}

bool CParserXml::pivotVisible()
{
	return m_showPivots;
}

osg::Vec2 CParserXml::getCellXY()
{
	return m_cellxy;
}

bool CParserXml::cells()
{
	return m_bCells;
}

float CParserXml::getLodDistMin()
{
	return m_lodDist_min;
}

float CParserXml::getLodDistMax()
{
	return m_lodDist_max;
}

CSulString CParserXml::getCellJson()
{
	return m_sCellJson;
}

bool CParserXml::getUseCellDebug()
{
	return m_bUseCellDebug;
}

bool CParserXml::getUseSun()
{
	return m_bSun;
}