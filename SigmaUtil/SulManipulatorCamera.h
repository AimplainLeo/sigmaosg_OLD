// SulManipulatorCamera.h

#ifndef __SULMANIPULATORCAMERA_H__
#define __SULMANIPULATORCAMERA_H__

#include <osgGA/CameraManipulator>

class CSulManipulatorCamera : public osgGA::CameraManipulator
{
public:
				CSulManipulatorCamera();

	void		SetSpeed( float fSpeed );

    void 		setByMatrix( const osg::Matrixd& matrix );          
    void 		setByInverseMatrix( const osg::Matrixd& matrix );
    osg::		Matrixd getMatrix() const;
    osg::		Matrixd getInverseMatrix() const;

    void 		home( double t );
    void 		home( const osgGA::GUIEventAdapter& ea , osgGA::GUIActionAdapter& aa );

	bool 		handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa );
	void 		computePosition( const osg::Vec3& eye,const osg::Vec3& center,const osg::Vec3& up );

private:
    osg::Vec3d  m_c;        // center
    osg::Quat   m_q;        // rotation
    float       m_d;        // distance

    double      m_timeLast;
    double      m_timeDelta;
    
    bool        m_bForward;
    bool        m_bReverse;
    bool        m_bTurnRight;
    bool        m_bTurnLeft;
    bool        m_bStrafeLeft;
    bool        m_bStrafeRight;

    float       m_fSpeed;
};



#endif // __SULMANIPULATORCAMERA_H__