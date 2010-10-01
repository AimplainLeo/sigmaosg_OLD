// SceneBase.h

#ifndef __SCENEBASE_H__
#define __SCENEBASE_H__

#include <SigmaUtil/SulString.h>
#include <SigmaUtil/SulCoordLatLon.h>
#include <SigmaUtil/SulCoordUTM.h>
#include <SigmaUtil/SulTypes.h>
#include <SigmaUtil/SulExtractGeometry.h>
#include <osg/group>
#include <osgSim/GeographicLocation>
#include <osg/PositionAttitudeTransform>
#include <osg/ComputeBoundsVisitor>

class CSceneBase : public osg::Group
{
public:
									CSceneBase( const CSulString& sFile, bool bRenderMe, const osg::Vec4& clipColor=osg::Vec4(0,0,1,0.5f) );

	bool							isRenderMe();

	double							getLat();
	double							getLon();

	CSulCoordLatLon*				getCoordLatLonObject();

	virtual void					adjustPosition( CSulCoordUTM* pWorldUTM  );

	void							addOffset( float x, float y, float z );

	void							drawBoundingBox();

	// planes are in world positions
	sigma::VEC_PLANE				getBoundingBoxPlanes();

	osg::ComputeBoundsVisitor*		getBoundsVisitor();
	osg::BoundingBox				getBoundingBoxWorld();

	void							clip( const sigma::VEC_PLANE& vecPlane );

	// lines are in local space
	const sigma::VEC_LINESEGMENT&	getClippedLineList();

	// triangles are in local space 
	const sigma::VEC_TRI&			getClippedTriangleList();

	osg::PositionAttitudeTransform* getPat();

private:
	void							clipLines( const CSulExtractGeometry& sceneGeometry, const sigma::VEC_PLANE& vecPlane );
	void							clipTriangles( const CSulExtractGeometry& sceneGeometry, const sigma::VEC_PLANE& vecPlane );

protected:
	osg::ref_ptr<osg::Node>							m_rScene;
	osg::ref_ptr<osg::PositionAttitudeTransform>	m_rPat;
	osg::ref_ptr<osgSim::GeographicLocation>		m_loc;
	osg::ref_ptr<CSulCoordLatLon>					m_rCoordLatLon;
	osg::ref_ptr<osg::ComputeBoundsVisitor>			m_rComputeBounds;

	sigma::VEC_TRI									m_vecTriangles;
	sigma::VEC_LINESEGMENT							m_vecLines;

	osg::Vec4										m_clipColor;

	bool											m_bRenderMe;
};

#endif // __SCENEBASE_H__