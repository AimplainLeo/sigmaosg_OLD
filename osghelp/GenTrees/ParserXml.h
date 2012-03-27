// ParserXml.h

#ifndef __PARSERXML_H__
#define __PARSERXML_H__

#include "SceneTerrain.h"
#include "SceneShape.h"
#include "SceneShapeMask.h"
#include "UniformData.h"
#include <SigmaUtil/SulDataShapeMask.h>
#include <SigmaUtil/SulXmlParser.h>
#include <SigmaUtil/SulCoordLatLon.h>
#include <SigmaUtil/SulCoordUTM.h>
#include <SigmaUtil/SulTypes.h>
#include <SigmaUtil/SulGenTextureWithPositions.h>

class CParserXml : public CSulXmlParser
{
public:
	typedef std::vector< CSulString >	VEC_STRING;
	typedef std::vector< osg::ref_ptr<CUniformData> >	VEC_UNIFORMDATA;

public:
						CParserXml();

	void				elementStart( const CSulString& sName, CSulXmlAttr* pAttr );

	const VEC_UNIFORMDATA&		getUniformDataList();

	void				SetInputFileOverride(const CSulString& filename);
	void				SetOutputFileOverride(const CSulString& filename);

	const VEC_SHAPEMASK& getShapeMaskList() const;

	bool				isViewerSuppressed();

	CSceneTerrain*		getSceneTerrain();
	CSceneShape*		getSceneShape();

	bool				hasColGeom();

	CSulGenTextureWithPositions* getGen();

	bool				isTextureSuppressed();

	sigma::uint32		getTexUnit();

	const CSulString&	getOutputFile() const;

	sigma::uint32		getUseLights();

	float				getMinTree();
	float				getMaxTree();

	bool				getSuppressShaders();

	osg::Program*		getProgram();

	bool				pivotVisible();

	bool				cells();
	osg::Vec2			getCellXY();

	float				getLodDistMin();
	float				getLodDistMax();

	CSulString			getCellJson();
	bool				getUseCellDebug();

	bool				getUseSun();

private:
	void				loadFinished();

private:
	VEC_SHAPEMASK								m_vecShapeMasks;
	
	bool										m_bSuppressTextureTree;
	sigma::uint32								m_texUnit;
	CSulString									m_sOutputFile;
	bool										m_suppressViewer;
	bool										m_showPivots;

	osg::ref_ptr<CSceneTerrain>					m_rSceneTerrain;
	osg::ref_ptr<CSceneShape>					m_rSceneShape;

	osg::ref_ptr<CSulCoordUTM>					m_rWorldUTM;

	sigma::VEC_PLANE							m_vecBoundingBoxPlanes;
	
	float										m_radiusBetweenTrees;
	float										m_distance_between_trees_line;

	osg::ref_ptr<CSulGenTextureWithPositions>	m_gen;

	sigma::uint32								m_useLights;

	CSulString									m_sInputFileOverride;

	float										m_minTree;
	float										m_maxTree;

	float										m_areaPadding;
	bool										m_bColGeom;

	VEC_STRING									m_vecNodeIgnoreList;

	bool										m_bSuppressShaders;
	bool										m_bIgnoreGeo;

	VEC_UNIFORMDATA								m_vecUniformList;

	osg::ref_ptr<osg::Program>					m_rShaderProgram;

	osg::Vec2									m_cellxy;
	bool										m_bCells;

	float										m_lodDist_min;
	float										m_lodDist_max;

	CSulString									m_sCellJson;
	bool										m_bUseCellDebug;

	bool										m_bSun;
};

#endif // __PARSERXML_H__