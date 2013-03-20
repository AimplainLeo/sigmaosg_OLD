// ParticleTrackballManipulator.cpp

#include "stdafx.h"
#include "ParticleTrackballManipulator.h"
#include <osgViewer/view>
#include <osgManipulator/Projector>

CParticleTrackballManipulator::CParticleTrackballManipulator( CSulParticleSystemOsg* particleSystem )
{
	m_particleSystem = particleSystem;
	m_bCameraControl = false;
	m_particleSystemContainerLast = 0;
}

bool CParticleTrackballManipulator::calcHitPoint(  const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa, osg::Vec3d& hit )
{
	osgViewer::View* view = dynamic_cast<osgViewer::View*>(&aa);
	if ( view )
	{
		osg::Plane plane( osg::Vec3(0,0,1), osg::Vec3(0,0,0) );
		osgManipulator::PlaneProjector* planeProjector = new osgManipulator::PlaneProjector;
		planeProjector->setPlane( plane );

		osgManipulator::PointerInfo pi;
		pi.reset();
		pi.setCamera( view->getCamera() );
		pi.setMousePosition( ea.getX(), ea.getY() );
		planeProjector->project( pi, hit );
		return true;
	}

	return false;
}

bool CParticleTrackballManipulator::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
	switch ( ea.getEventType() )
	{
		case osgGA::GUIEventAdapter::KEYDOWN:
			{
				int key = ea.getKey();
				int mask = ea.getModKeyMask();

				if ( mask & osgGA::GUIEventAdapter::MODKEY_LEFT_CTRL )
					m_bCameraControl = true;
			}
			break;

		case osgGA::GUIEventAdapter::KEYUP:
			{
				int key = ea.getKey();
				int mask = ea.getModKeyMask();

				if ( mask & osgGA::GUIEventAdapter::MODKEY_LEFT_CTRL )
					m_bCameraControl = false;
			}
			break;

	}

	if ( !m_bCameraControl )
	{
		switch ( ea.getEventType() )
		{
			case osgGA::GUIEventAdapter::RELEASE:
				{
					osg::Vec3d hit;

					if ( calcHitPoint( ea, us, hit ) )
					{
						m_particleSystem->placeParticleSystem( hit, m_particleSystemContainerLast );
					}
				}
				break;

			case osgGA::GUIEventAdapter::PUSH:
				m_particleSystem->updateFromUI();
				m_particleSystemContainerLast = m_particleSystem->createParticleSystem();
				break;
		}

		return true;
	}
		
	return osgGA::TrackballManipulator::handle( ea, us );
}
