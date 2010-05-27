// MousePlaneIntersection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgGA/GUIEventHandler>
#include <osg/ShapeDrawable>
#include <osg/positionattitudetransform>
#include <osgManipulator/Projector>

class CInputHandler : public osgGA::GUIEventHandler 
{
public:
	CInputHandler( osg::PositionAttitudeTransform* pPatSphere )
	{
		m_rPatSphere = pPatSphere;
		osg::Plane plane( osg::Vec3(0,0,1), osg::Vec3(0,0,0) );

		m_rPlaneProj = new osgManipulator::PlaneProjector;
		m_rPlaneProj->setPlane( plane );
	}

    virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
    {
        osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(&aa);
        if ( !pViewer )
        {
            return false;
        }

		if ( ea.getEventType()==osgGA::GUIEventAdapter::PUSH )
        {
            osgViewer::ViewerBase::Cameras cams;
            pViewer->getCameras( cams );

			osg::Vec3d v;
			osgManipulator::PointerInfo pi;
		
			pi.reset();
			pi.setCamera( cams[0] );
			pi.setMousePosition( ea.getX(), ea.getY() );
			m_rPlaneProj->project( pi, v );
			
			m_rPatSphere->setPosition( v );

            return true; // return true, event handled
        }

        return false;
    }

private:
	osg::ref_ptr<osgManipulator::PlaneProjector> m_rPlaneProj;
	osg::ref_ptr<osg::PositionAttitudeTransform> m_rPatSphere;
};

osg::Node* CreateScene( osg::PositionAttitudeTransform* pPat )
{
	osg::Group* pGroup = new osg::Group;

	osg::Geode* pGeode = new osg::Geode;
	pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,-0.5f),100.0f, 100.0f, 1.0f) ) );
	pGroup->addChild( pGeode );

	osg::Geode* pGeodeSphere = new osg::Geode;
	pGeodeSphere->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),5.0f) ) );
    pPat->addChild( pGeodeSphere );
	pGroup->addChild( pPat );

    return pGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 512, 512 );

	osg::ref_ptr<osg::PositionAttitudeTransform> rPat = new osg::PositionAttitudeTransform;

    // add the handler to the viewer
    rViewer->addEventHandler( new CInputHandler(rPat.get()) );

    // set the scene-graph data the viewer will render
    rViewer->setSceneData( CreateScene( rPat.get() ) );

    // execute main loop
    return rViewer->run();
}

