// MouseSceneIntersection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgGA/GUIEventHandler>
#include <osg/ShapeDrawable>
#include <osg/positionattitudetransform>

class CInputHandler : public osgGA::GUIEventHandler 
{
public:
    CInputHandler( osg::PositionAttitudeTransform* pPatSphere )
    {
        m_rPatSphere = pPatSphere;
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

			float x = ea.getXnormalized();
			float y = ea.getYnormalized();

			osgUtil::LineSegmentIntersector* picker = new osgUtil::LineSegmentIntersector( osgUtil::Intersector::PROJECTION, x, y );
			osgUtil::IntersectionVisitor iv( picker );
			cams[0]->accept( iv );

			if ( picker->containsIntersections() )
			{
				osgUtil::LineSegmentIntersector::Intersection intersection = picker->getFirstIntersection();
				osg::Vec3 v = intersection.getWorldIntersectPoint();
				m_rPatSphere->setPosition( v );
			}

			return true; // return true, event handled
        }

        return false;
    }

private:
    osg::ref_ptr<osg::PositionAttitudeTransform> m_rPatSphere;
};

osg::Node* CreateScene( osg::PositionAttitudeTransform* pPat )
{
    // create a group to contain our floor and sphere
	osg::Group* pGroup = new osg::Group;

	// create floor
    osg::Geode* pGeode = new osg::Geode;
    pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,-0.5f),100.0f, 100.0f, 1.0f) ) );
    pGroup->addChild( pGeode );

	// create sphere
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