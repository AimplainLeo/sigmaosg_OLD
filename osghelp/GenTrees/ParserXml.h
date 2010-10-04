// ParserXml.h

#ifndef __PARSERXML_H__
#define __PARSERXML_H__

#include "SceneTerrain.h"
#include "SceneShape.h"
#include "SceneShapeMask.h"
#include <SigmaUtil/SulDataShapeMask.h>
#include <SigmaUtil/SulXmlParser.h>
#include <SigmaUtil/SulCoordLatLon.h>
#include <SigmaUtil/SulCoordUTM.h>
#include <SigmaUtil/SulTypes.h>
#include <SigmaUtil/SulGenTextureWithPositions.h>

class CParserXml : public CSulXmlParser
{
private:
	typedef std::vector< CSulString >									VEC_STRING;


public:
						CParserXml();

	void				elementStart( const CSulString& sName, CSulXmlAttr* pAttr );

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
};

#endif // __PARSERXML_H__