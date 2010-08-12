// SulGenPositionTextures.h

// FIXME: rename class and file to CSulGenPositionTexture without the 's'

#ifndef __SULGENPOSITIONTEXTURES_H__
#define __SULGENPOSITIONTEXTURES_H__

#include "SulTypes.h"
#include "SulNodeVisitorExtractLines.h"
#include "SulClipTriangle.h"
#include "SulDataShapeMask.h"
#include <osg/linesegment>

class CSulGenPositionTextures
{
public:
	CSulGenPositionTextures();

	CSulGenPositionTextures(
		osg::Node*				pScene,
		osg::Matrix				lineTransform,
		osg::Node*				pShape,
		const osg::BoundingBox&	bb,
		const std::vector<osg::Plane> vecPlane,
		const VEC_DATASHAPEMASK& vecDataShapeMask,
		float					lineSpacing = 2.0f,
		float					lineJitterRadius = 2.0f,
		bool					lineSnap = true );

	void	process();

	sigma::uint32				getCount();
	sigma::uint32				getCountPos();

	osg::Image*					getImage();

	sigma::uint32				getTexSizeSquared();

	const CSulClipTriangle&		getClipTriangles();

	const std::vector<osg::Vec3>& getTreePositions() const;

	const sigma::VEC_VEC3&		getRejectedPositionsList();

private:
	void	addLine( const osg::LineSegment& line );
	void	addTri( CSulDataTri tri );

	void	plantTrees( sigma::uint32 numTrees, const std::vector<float>& vecA, const sigma::VEC_TRI& tri );

	void	preprocessMasks();

	void	processTexture();

	void	addTree( const osg::Vec3& pos );

private:
	osg::ref_ptr<osg::Node>		m_rShape;
	osg::BoundingBox			m_bb;
	float						m_lineSpacing;
	float						m_lineJitterRadius;
	bool						m_lineSnap;
	osg::ref_ptr<osg::Image>	m_rImage;
	sigma::uint32				m_lineCount;
	sigma::uint32				m_posCount;
	osg::Matrix					m_lineTransform;
	osg::ref_ptr<osg::Node>		m_rScene;

	std::vector<osg::Vec3>		m_vecPositions;
	float						m_radius;

	sigma::VEC_VEC3				m_vecPos;
	sigma::uint32				m_texSizeSquared;

	std::vector<osg::Plane>		m_vecPlane;

	CSulClipTriangle*			m_clipTri;

	VEC_DATASHAPEMASK			m_vecDataShapeMask;
	sigma::VEC_VEC3				m_vecRejectedPositions;
};

#endif // __SULGENPOSITIONTEXTURES_H__