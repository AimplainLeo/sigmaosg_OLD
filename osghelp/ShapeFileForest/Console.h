// Console.h

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <SigmaUtil/SulRenderInstances.h>
#include <SigmaUtil/SulConsoleDisplay.h>
#include <osg/positionattitudetransform>

class CConsole
{
public:
			CConsole( 
				osgViewer::Viewer* pViewer,
				CSulConsoleDisplay* pConsoleDisplay,
				osg::PositionAttitudeTransform* pScene,
				osg::PositionAttitudeTransform* pShape );

	void	save();
	void	generate();

    void	Command( const CSulString& sText );

private:
	void			showTrees( osg::Image* pImage, sigma::uint32 posCount, osg::BoundingBox& bb, sigma::uint32 texSizeSquared );
	void			showPlaceholders( osg::Image* pImage, sigma::uint32 posCount );
	osg::Geode*		createSphere( const osg::Vec3& pos );
	osg::ref_ptr<osg::Geometry>	createSingleTreeCollisionGeometry(float s);
	osg::ref_ptr<osg::Node> createTreeCollisionGeometry(const std::vector<osg::Vec3>& positionList);

private:
	osg::ref_ptr<osgViewer::Viewer>					m_rViewer;
	osg::ref_ptr<CSulConsoleDisplay>				m_rConsole;
	osg::ref_ptr<osg::PositionAttitudeTransform>	m_rScene;
	osg::ref_ptr<osg::PositionAttitudeTransform>	m_rShape;

	CSulRenderInstances* pRenderInstances;
	osg::ref_ptr<osg::Node> m_rTreeCollisionGeometry; 
};

#endif  __CONSOLE_H__