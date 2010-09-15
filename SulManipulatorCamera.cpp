// SulManipulatorCamera.cpp

#include "stdafx.h"
#include "SulManipulatorCamera.h"

CSulManipulatorCamera::CSulManipulatorCamera() :
m_timeLast( 0.0f ),
m_timeDelta( 0.0f ),
m_fSpeed( 10.0f ),
m_bForward( false ),
m_bReverse( false ),
m_bTurnRight( false ),
m_bTurnLeft( false ),
m_bStrafeLeft( false ),
m_bStrafeRight( false ),
m_d(1.0f)
{
}

void CSulManipulatorCamera::SetSpeed( float fSpeed )
{
	m_fSpeed = fSpeed;
}

void CSulManipulatorCamera::setByMatrix( const osg::Matrixd& matrix )          
{
    m_c = osg::Vec3(0.0f,0.0f,-m_d)*matrix;
    m_q = matrix.getRotate();
}

void CSulManipulatorCamera::setByInverseMatrix( const osg::Matrixd& matrix )   
{ 
    setByMatrix( osg::Matrixd::inverse(matrix) );
}

osg::Matrixd CSulManipulatorCamera::getMatrix() const          
{ 
    return osg::Matrixd::translate(0.0,0.0,m_d)*osg::Matrixd::rotate(m_q)*osg::Matrixd::translate(m_c);
}

osg::Matrixd CSulManipulatorCamera::getInverseMatrix() const
{ 
    return osg::Matrixd::translate(-m_c)*osg::Matrixd::rotate(m_q.inverse())*osg::Matrixd::translate(0.0,0.0,-m_d);
} 


void CSulManipulatorCamera::home( double t )
{
    computePosition( _homeEye, _homeCenter, _homeUp );
}

void CSulManipulatorCamera::home( const osgGA::GUIEventAdapter& ea , osgGA::GUIActionAdapter& aa )
{
    home( ea.getTime() );
}

bool CSulManipulatorCamera::handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa )
{
    switch ( ea.getEventType() )
    {
        case osgGA::GUIEventAdapter::KEYDOWN:
            {
                if ( ea.getKey()=='w' ) m_bForward = true;
                if ( ea.getKey()=='s' ) m_bReverse = true;
                if ( ea.getKey()=='a' ) m_bStrafeLeft = true;
                if ( ea.getKey()=='d' ) m_bStrafeRight = true;
            }
            return true;

        case osgGA::GUIEventAdapter::KEYUP:
            {
                if ( ea.getKey()=='w' ) m_bForward = false;
                if ( ea.getKey()=='s' ) m_bReverse = false;
                if ( ea.getKey()=='a' ) m_bStrafeLeft = false;
                if ( ea.getKey()=='d' ) m_bStrafeRight = false;
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
                if ( m_bForward )       m_c.y() += m_timeDelta*m_fSpeed;
                if ( m_bReverse )       m_c.y() -= m_timeDelta*m_fSpeed;
                if ( m_bStrafeRight )   m_c.x() += m_timeDelta*m_fSpeed;
                if ( m_bStrafeLeft )    m_c.x() -= m_timeDelta*m_fSpeed;
            }
            return true;
    }

    return false;
}

void CSulManipulatorCamera::computePosition( const osg::Vec3& eye,const osg::Vec3& center,const osg::Vec3& up )
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
