// MouseGraphIntersection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgGA/GUIEventHandler>
#include <osg/ShapeDrawable>
#include <osg/positionattitudetransform>
#include <osg/LineSegment>
#include <osgUtil/IntersectVisitor>

class CInputHandler : public osgGA::GUIEventHandler 
{
public:
	CInputHandler( osg::PositionAttitudeTransform* pPatSphere, osg::Node* pGraph )
    {
        m_rPatSphere	= pPatSphere;
		m_rGraph		= pGraph;
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
			// normalized mouse position
			float x = ea.getXnormalized();
			float y = ea.getYnormalized();

			osg::Matrixd proj = pViewer->getCamera()->getProjectionMatrix();

			osg::Viewport* viewport = pViewer->getCamera()->getViewport();
			double mx = viewport->x() + (int)((double )viewport->width()*(x*0.5+0.5));
			double my = viewport->y() + (int)((double )viewport->height()*(y*0.5+0.5));

			osg::Matrix MVPW = pViewer->getCamera()->getViewMatrix() * proj;
			MVPW.postMult( pViewer->getCamera()->getViewport()->computeWindowMatrix() );

			osg::Matrixd inverseMVPW;
			inverseMVPW.invert(MVPW);

			osg::Vec3 nearPoint = osg::Vec3(mx,my,0.0f) * inverseMVPW;
			osg::Vec3 farPoint = osg::Vec3(mx,my,1.0f) * inverseMVPW;
			
			osg::LineSegment* pLingSeg = new osg::LineSegment;
			pLingSeg->set(nearPoint, farPoint);

			osgUtil::IntersectVisitor iv;
			iv.reset();
			iv.addLineSegment( pLingSeg );

			m_rGraph->accept( iv );

			if ( iv.hits() )
			{
				osgUtil::IntersectVisitor::HitList hitList = iv.getHitList( pLingSeg );

				osg::Vec3 v = hitList[0].getWorldIntersectPoint();
				m_rPatSphere->setPosition( v );
			}

			return true; // return true, event handled
        }

        return false;
    }

private:
    osg::ref_ptr<osg::PositionAttitudeTransform>	m_rPatSphere;
	osg::ref_ptr<osg::Node>							m_rGraph;
};

osg::Node* CreateScene( osgViewer::Viewer* pViewer )
{
    // create a group to contain our floor and sphere
	osg::Group* pGroup = new osg::Group;

	// create floor
    osg::Geode* pGeodeFloor = new osg::Geode;
    pGeodeFloor->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(0.0f,0.0f,-0.5f),100.0f, 100.0f, 1.0f) ) );
    pGroup->addChild( pGeodeFloor );

	// create sphere
    osg::Geode* pGeodeSphere = new osg::Geode;
    pGeodeSphere->addDrawable( new osg::ShapeDrawable( new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f),5.0f) ) );

	// create transform to move sphere
	osg::PositionAttitudeTransform* pPat = new osg::PositionAttitudeTransform;
	pPat->addChild( pGeodeSphere );
	pGroup->addChild( pPat );

    // add the handler to the viewer
    pViewer->addEventHandler( new CInputHandler(pPat, pGeodeFloor ) );

    return pGroup;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 512, 512 );

    // set the scene-graph data the viewer will render
    rViewer->setSceneData( CreateScene( rViewer.get() ) );

    // execute main loop
    return rViewer->run();
}

