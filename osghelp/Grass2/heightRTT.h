// heightRTT.h

#ifndef __HEIGHTRTT_H__
#define __HEIGHTRTT_H__

#include <SigmaUtil/SulRTT.h>
#include <osg/group>
#include <osgtext/text>
#include <osgViewer/Viewer>

class CHeightRTT
{
public:
	void			init( osgViewer::Viewer* pViewer, osgText::Text* pTextCam, osg::Camera* pMainCam, osg::Group* pGroup, osg::Node* pRenderMe );

	// returns RTT camera
	osg::Camera*	getCamera();

	osg::Texture2D*	getTexture();

	void setOrtho( float cc, float sp );

	void	setLock( bool bLock );

private:
	osg::ref_ptr<CSulRTT> rRTT;
	osgText::Text*			m_pTextCam;
	osg::Camera*			m_pMainCam;
};

#endif // __HEIGHTRTT_H__