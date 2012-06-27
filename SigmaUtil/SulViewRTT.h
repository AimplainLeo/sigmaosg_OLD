// SulViewRTT.h

#ifndef __SULVIEWRTT_H__
#define __SULVIEWRTT_H__

#include "SulTexCam.h"
#include "SulTypes.h"
#include <osgViewer/Viewer>

class CSulViewRTT : public osg::Group
{
public:
						CSulViewRTT( osgViewer::Viewer* viewer );

						CSulViewRTT( osgViewer::Viewer* viewer, osg::Node* render, CSulTexCam::ESETUP eSetup=CSulTexCam::STANDARD, sigma::int32 x=0.0f, sigma::int32 y=0.0f, sigma::uint32 w=200.0f, sigma::uint32 h=150.0f );

	void				setReferenceFrame( osg::Transform::ReferenceFrame rf );

	osg::Texture2D*		getTexture( sigma::uint32 index=0 );

	sigma::uint32		getWidth();
	sigma::uint32		getHeight();

	CSulTexCam*			getTexCam();

private:
	osg::ref_ptr<CSulTexCam> m_rtt;
};

#endif // __SULVIEWRTT_H__