// SulCrossQuad.h

#ifndef __SULCROSSQUAD_H__
#define __SULCROSSQUAD_H__

#include "SulTypes.h"
#include "SulGeomQuad.h"
#include <osgViewer/Viewer>

class CSulCrossQuad : public osg::Group
{
private:
	enum ORTHOVIEW
	{
		FRONT,
		BACK,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM
	};

public:
							CSulCrossQuad( osgViewer::Viewer* pViewer, osg::Node* pRender, osg::BoundingBox* pBB, const CSulString& sJson );

	void					process();

private:
	osg::ref_ptr<osg::Texture2D>	createTextureFromOrthoView2( sigma::uint32 texW, sigma::uint32 texH, osg::Node* pRenderMe, ORTHOVIEW eOrthoView, osg::Vec3 pos, float far );
	osg::Node*						createQuad( sigma::uint32 texW, sigma::uint32 texH, const osg::Vec3& pos, float l, float h, float far, float near, CSulGeomQuad::EPLANE ePlane );

private:
	osg::ref_ptr<osg::Group>		m_rContainer;	// container is need because the caller might have used shaders to render the geometry
	osg::ref_ptr<osgViewer::Viewer>	m_rViewer;
	osg::ref_ptr<osg::Node>			m_rRender;
	osg::BoundingBox*				m_pBB;
	CSulString						m_sJson;
};

#endif // __SULCROSSQUAD_H__