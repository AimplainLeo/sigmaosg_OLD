// SulGenTextureWithPositions.h

#ifndef __SULGENTEXTUREWITHPOSITIONS_H__
#define __SULGENTEXTUREWITHPOSITIONS_H__

#include "SulGenPositionMask.h"
#include "SulTypes.h"
#include "SulString.h"
#include <osg/referenced>

class CSulGenTextureWithPositions : public osg::Referenced
{
private:
	typedef std::vector< CSulString >									VEC_STRING;

public:
			CSulGenTextureWithPositions(
				osg::PositionAttitudeTransform*	pSceneTerrain,
				const sigma::VEC_LINESEGMENT&	line,
				const sigma::VEC_TRI&			tri,
				float							radius,							// radius between trees
				float							distance_between_trees_line,
				float							areaPadding,					// areaPadding is used to extend masks for areas
				VEC_GENPOSITIONMASK				vecMask,
				VEC_STRING						vecIgnoreNodeList
			);

	osg::Image*				getImage();
	sigma::uint32			getCount();
	sigma::uint32			getTexSizeSquared();
	const sigma::VEC_VEC3&	getPositions();

private:
	void	process();
	void	processLine( osg::LineSegment* pLine );
	void	processLines();
	void	processTriangles();
	void	processMasks();
	void	processMask( CSulGenPositionMask* pMask );
	void	processMaskLines( CSulGenPositionMask* pMask );
	void	processMaskTri( CSulGenPositionMask* pMask );
	void	processTexture();

private:
	osg::ref_ptr<osg::PositionAttitudeTransform>		m_rSceneTerrain;
	const sigma::VEC_LINESEGMENT&	m_vecLine;
	const sigma::VEC_TRI&			m_vecTri;
	float							m_radius;						// radius between trees on a patch
	float							m_distance_between_trees_line;	// distance between trees on a line
	float							m_areaPadding;
	sigma::VEC_VEC3					m_vecPos;						// positions are in world space
	sigma::uint32					m_texSizeSquared;
	osg::ref_ptr<osg::Image>		m_rImage;
	sigma::uint32					m_posCount;
	
	VEC_GENPOSITIONMASK				m_vecMask;
};

#endif __SULGENTEXTUREWITHPOSITIONS_H__