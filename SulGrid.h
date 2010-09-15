// SulGrid.h

#ifndef __SULGRID_H__
#define __SULGRID_H__

#include "SulExport.h"
#include "SulGeomGrid.h"
#include <osg/Geode>
#include <osg/ref_ptr>
#include <osg/referenced>
#include <osg/positionattitudetransform>

class SUL_EXPORT CSulGrid : public osg::Referenced
{
public:
									CSulGrid();

	osg::PositionAttitudeTransform*	GetPAT();

	void							Update();

private:
	void							CalcCorners( osg::Node* pNode, osg::Camera* pCam );
	void							testme( osg::Node* node );

private:
	osg::ref_ptr<osg::Geode>		m_rGeode;
	osg::ref_ptr<CSulGeomGrid>		m_rGeomGrid;

	osg::ref_ptr<osg::PositionAttitudeTransform>	m_rPat;
};

#endif // __SULGRID_H__