// camsync.h

#ifndef __CAMSYNC_H__
#define __CAMSYNC_H__

#include <SigmaUtil/SulNodeCallbackCameraSync.h>
#include <osgText/Text>

class CCamSync : public CSulNodeCallbackCameraSync
{
public:
	CCamSync( osgText::Text* pText, osg::Camera* pCamSync, EMODECAM eMode=MODE_FULL ) 
		: CSulNodeCallbackCameraSync( pCamSync, eMode )
	{
		m_rText = pText;
	}

	virtual void operator()( osg::Node* node, osg::NodeVisitor* nv )
    {
		// this is the camera that needs to get into sync
		osg::Camera* pCamSyncMe = dynamic_cast<osg::Camera*>(node);

		osg::Vec3 v = m_pCam->getViewMatrix().getTrans();
		char dest[1024];
		sprintf( dest, "x: %.2f, y: %.2f, z: %.2f", v.x(), v.y(), v.z() );

		// m_pCam is the camera we want to sync from
		if ( m_rText.valid() )
		{
			m_rText->setText( dest );
		}

		CSulNodeCallbackCameraSync::operator()(node,nv);
	}

private:
	osg::ref_ptr<osgText::Text>	m_rText;
};

#endif // __CAMSYNC_H__