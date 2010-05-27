// placementRTT.h

#ifndef __PLACEMENTRTT_H__
#define __PLACEMENTRTT_H__

#include <SigmaUtil/SulRTT.h>
#include <osg/camera>

class CPlacementRTT
{
public:
					CPlacementRTT();

	void			init( osg::Camera* pCam, osg::Node* pRenderMe );

	osg::Camera*	getCamera();

	osg::Texture2D*	getTexture();

private:
	osg::ref_ptr<CSulRTT>	m_rRTT;
};

#endif // __PLACEMENTRTT_H__