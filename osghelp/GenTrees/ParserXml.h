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
public:
						CParserXml();

	void				ElementStart( const CSulString& sName, CSulXmlAttr* pAttr );

	void				SetInputFileOverride(const CSulString& filename);
	void				SetOutputFileOverride(const CSulString& filename);

	const VEC_SHAPEMASK& getShapeMaskList() const;

	bool				isViewerSuppressed();

	CSceneTerrain*		getSceneTerrain();
	CSceneShape*		getSceneShape();

	CSulGenTextureWithPositions* getGen();

	float				getSizeMultiplier();

	bool				isTextureSuppressed();

	Sigma::uint32		getTexUnit();

	const CSulString&	getOutputFile() const;

	Sigma::uint32		getUseLights();

private:
	void				LoadFinished();
private:
	VEC_SHAPEMASK								m_vecShapeMasks;
	
	bool										m_bSuppressTextureTree;
	Sigma::uint32								m_texUnit;
	CSulString									m_sOutputFile;
	float										m_sizeMultiplier;
	bool										m_suppressViewer;
	bool										m_showPivots;

	osg::ref_ptr<CSceneTerrain>					m_rSceneTerrain;
	osg::ref_ptr<CSceneShape>					m_rSceneShape;

	osg::ref_ptr<CSulCoordUTM>					m_rWorldUTM;

	Sigma::VEC_PLANE							m_vecBoundingBoxPlanes;
	
	float										m_radiusBetweenTrees;

	osg::ref_ptr<CSulGenTextureWithPositions>	m_gen;

	Sigma::uint32								m_useLights;

	CSulString					m_sInputFileOverride;
};

#endif // __PARSERXML_H__