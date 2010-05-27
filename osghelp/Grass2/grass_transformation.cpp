// grass_transformation.cpp

#include "stdafx.h"
#include "grass_transformation.h"

class CGrassKeyboardHandler : public osgGA::GUIEventHandler 
{
public:
	CGrassKeyboardHandler( CGrassTransformation* pT )
		: osgGA::GUIEventHandler()
	{
		m_rT = pT;
		m_last_t = 0.0;
		dx = 0.0f;
		dy = 0.0f;
	}
	
	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Object* pObject, osg::NodeVisitor* pNodeVisitor )
    {
        if ( ea.getEventType()==osgGA::GUIEventAdapter::KEYDOWN )
        {
            if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Right )	dx = 1.0f;
			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Left )	dx = -1.0f;
            if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Up )		dy = 1.0f;
			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Down )	dy = -1.0f;

			return false; // return true, event handled
		}

		if ( ea.getEventType()==osgGA::GUIEventAdapter::KEYUP )
        {
            if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Right )	dx = 0.0f;
			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Left )	dx = 0.0f;
            if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Up )		dy = 0.0f;
			if ( ea.getKey()==osgGA::GUIEventAdapter::KEY_Down )	dy = 0.0f;
            
			return false; // return true, event handled
		}

		if ( ea.getEventType()==osgGA::GUIEventAdapter::FRAME )
		{
			if ( m_last_t==0.0 )
			{
				m_last_t = ea.getTime();
				return true;
			}

			double t = ea.getTime();
			double dt = t-m_last_t;
			m_last_t = ea.getTime();

			osg::Vec3 vPos = m_rT->getPosition();
			vPos += osg::Vec3(dt*dx*10.0f,dt*dy*10.0f,0 );
			m_rT->setPosition( vPos );

			return true;
		}
        
        return false;
    }

private:
	osg::ref_ptr<CGrassTransformation>	m_rT;
	double								m_last_t;
	float								dx;
	float								dy;
};


CGrassTransformation::CGrassTransformation( osgViewer::Viewer* pViewer )
{
	osg::ref_ptr<CGrassKeyboardHandler> rKeyboardHandler = new CGrassKeyboardHandler( this );
	pViewer->addEventHandler( rKeyboardHandler );
}
