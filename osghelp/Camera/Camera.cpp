// Camera.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <osgViewer/Viewer>
#include <osgGA/GUIEventHandler>
#include <osg/ShapeDrawable>

#define BOX_SIZE	(10)
#define BOX_SPACING	(3)

class CMyCameraManipulator : public osgGA::CameraManipulator
{
public:
	CMyCameraManipulator() :
	m_timeLast( 0.0f ),
	m_timeDelta( 0.0f ),
	m_fSpeed(10.0f),
	m_bForward(false),
	m_bReverse(false),
	m_bTurnRight(false),
	m_bTurnLeft(false),
	m_bStrafeLeft(false),
	m_bStrafeRight(false),
	m_d(1.0f)
	{
	}

	void setByMatrix( const osg::Matrixd& matrix )			
	{
		m_c = osg::Vec3(0.0f,0.0f,-m_d)*matrix;
		m_q = matrix.getRotate();
	}

	void setByInverseMatrix( const osg::Matrixd& matrix )	
	{ 
		setByMatrix( osg::Matrixd::inverse(matrix) );
	}

	osg::Matrixd getMatrix() const			
	{ 
		return osg::Matrixd::translate(0.0,0.0,m_d)*osg::Matrixd::rotate(m_q)*osg::Matrixd::translate(m_c);
	}

	osg::Matrixd getInverseMatrix() const
	{ 
		return osg::Matrixd::translate(-m_c)*osg::Matrixd::rotate(m_q.inverse())*osg::Matrixd::translate(0.0,0.0,-m_d);
	} 

	void home( double t )
	{
		computePosition( _homeEye, _homeCenter, _homeUp );
	}

	void home( const osgGA::GUIEventAdapter& ea , osgGA::GUIActionAdapter& aa )
	{
		home( ea.getTime() );
	}

	bool handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa )
	{
		switch ( ea.getEventType() )
		{
			case osgGA::GUIEventAdapter::KEYDOWN:
				{
					if ( ea.getKey()=='w' )	m_bForward = true;
					if ( ea.getKey()=='s' )	m_bReverse = true;
					if ( ea.getKey()=='a' )	m_bStrafeLeft = true;
					if ( ea.getKey()=='d' )	m_bStrafeRight = true;
				}
				return true;

			case osgGA::GUIEventAdapter::KEYUP:
				{
					if ( ea.getKey()=='w' )	m_bForward = false;
					if ( ea.getKey()=='s' )	m_bReverse = false;
					if ( ea.getKey()=='a' )	m_bStrafeLeft = false;
					if ( ea.getKey()=='d' )	m_bStrafeRight = false;
				}
				return true;

			case( osgGA::GUIEventAdapter::FRAME ):
				{
					double t = ea.getTime();

					// check for first time initialization
				    if ( m_timeLast==0.0 )
					{
						m_timeLast = ea.getTime();
						m_timeDelta = 0.0;
					}
					else
					{
						m_timeDelta = t - m_timeLast;
						m_timeLast = t;
					}

					// do movement
					if ( m_bForward )		m_c.y() += m_timeDelta*m_fSpeed;
					if ( m_bReverse )		m_c.y() -= m_timeDelta*m_fSpeed;
					if ( m_bStrafeRight )	m_c.x() += m_timeDelta*m_fSpeed;
					if ( m_bStrafeLeft )	m_c.x() -= m_timeDelta*m_fSpeed;
				}
				return true;
		}

		return false;
	}

	void computePosition( const osg::Vec3& eye,const osg::Vec3& center,const osg::Vec3& up )
	{
		osg::Vec3 lv(center-eye);

		osg::Vec3 f(lv);
		f.normalize();
		osg::Vec3 s(f^up);
		s.normalize();
		osg::Vec3 u(s^f);
		u.normalize();
	    
		osg::Matrix rotation_matrix(s[0],     u[0],     -f[0],     0.0f,
									s[1],     u[1],     -f[1],     0.0f,
									s[2],     u[2],     -f[2],     0.0f,
									0.0f,     0.0f,     0.0f,      1.0f);
	                   
		m_c = center;
		m_d = lv.length();
		m_q = rotation_matrix.getRotate().inverse();
	}

private:
    osg::Vec3d	m_c;		// center
    osg::Quat   m_q;		// rotation
	float		m_d;		// distance

    double		m_timeLast;
    double		m_timeDelta;
	
	bool		m_bForward;
	bool		m_bReverse;
	bool		m_bTurnRight;
	bool		m_bTurnLeft;
	bool		m_bStrafeLeft;
	bool		m_bStrafeRight;

	float		m_fSpeed;
};


osg::Node* CreateScene()
{
	osg::Geode* pGeode = new osg::Geode;

	for ( int y=0; y<BOX_SIZE; y++ )
		for( int x=0; x<BOX_SIZE; x++ )
		{
			pGeode->addDrawable( new osg::ShapeDrawable( new osg::Box(osg::Vec3(x*BOX_SPACING,y*BOX_SPACING,0.0f),2.0f) ) );
		}

	return pGeode;
}

int _tmain(int argc, _TCHAR* argv[])
{
    // construct the viewer
    osg::ref_ptr<osgViewer::Viewer> rViewer = new osgViewer::Viewer;

    // make the viewer create a 512x512 window and position it at 32, 32
    rViewer->setUpViewInWindow( 32, 32, 512, 512 );

	// set the scene-graph data the viewer will render
	osg::Node* pScene = CreateScene();
    rViewer->setSceneData( CreateScene() );

	CMyCameraManipulator* pManipulator = new CMyCameraManipulator;
	pManipulator->setHomePosition( 
		osg::Vec3d( (BOX_SIZE*BOX_SPACING)/2, (BOX_SIZE*BOX_SPACING)/2-20, 40 ), 
		osg::Vec3d( (BOX_SIZE*BOX_SPACING)/2, (BOX_SIZE*BOX_SPACING)/2, 0 ), 
		osg::Vec3d( 0,0,1 ) );
	pManipulator->computeHomePosition();
	pManipulator->home( 1.0f );
	rViewer->setCameraManipulator( pManipulator );

    // execute main loop
    return rViewer->run();
}


