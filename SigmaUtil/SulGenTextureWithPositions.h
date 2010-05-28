// SulGenTextureWithPositions.h

#ifndef __SULGENTEXTUREWITHPOSITIONS_H__
#define __SULGENTEXTUREWITHPOSITIONS_H__

#include "SulGenPositionMask.h"
#include "SulTypes.h"
#include <osg/referenced>

class CSulGenTextureWithPositions : public osg::Referenced
{
public:
			CSulGenTextureWithPositions(
				osg::PositionAttitudeTransform*	pSceneTerrain,
				const Sigma::VEC_LINESEGMENT&	line,
				const Sigma::VEC_TRI&			tri,
				float							radius,						// radius between trees
				float							distance_between_trees_line,
				VEC_GENPOSITIONMASK				vecMask
			);

	osg::Image*		getImage();
	Sigma::uint32	getCount();
	Sigma::uint32	getTexSizeSquared();

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
	const Sigma::VEC_LINESEGMENT&	m_vecLine;
	const Sigma::VEC_TRI&			m_vecTri;
	float							m_radius;						// radius between trees on a patch
	float							m_distance_between_trees_line;	// distance between trees on a line
	Sigma::VEC_VEC3					m_vecPos;				// positions are in world space
	Sigma::uint32					m_texSizeSquared;
	osg::ref_ptr<osg::Image>		m_rImage;
	Sigma::uint32					m_posCount;
	
	VEC_GENPOSITIONMASK				m_vecMask;
};

#endif __SULGENTEXTUREWITHPOSITIONS_H__