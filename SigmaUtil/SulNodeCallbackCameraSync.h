// SulNodeCallbackCameraSync.h

#ifndef __IUNODECALLBACKCAMERASYNC_H__
#define __IUNODECALLBACKCAMERASYNC_H__

#include <osgUtil/SceneView>
#include <osg/Camera>

// usage example:
// pMyCam->addUpdateCallback( new CSulNodeCallbackCameraSync( pCamToSyncWith ) );

class CSulNodeCallbackCameraSync : public osg::NodeCallback
{
public:
	enum EMODECAM
	{
		MODE_FULL,
		MODE_SIMPLE,
		MODE_POSITION_XY,			// sync only positions xy (not sure this mode is working correctly)
		MODE_VIEWMATRIX,
		MODE_SIMPLE_OFFSET
	};

public:
	CSulNodeCallbackCameraSync( osg::Camera* pCam, osg::Camera* pCamSync )
	{
		m_pCam = pCamSync;
		m_eMode = MODE_SIMPLE;

		osg::Matrixd mProj	= pCamSync->getProjectionMatrix();
		pCam->setProjectionMatrix( mProj );
	}

	CSulNodeCallbackCameraSync( osg::Camera* pCamSync, EMODECAM eMode=MODE_FULL )
	{
		m_pCam = pCamSync;
		m_eMode = eMode;
	}

    virtual void operator()( osg::Node* node, osg::NodeVisitor* nv )
    {
		traverse( node, nv );

		if ( m_eMode==MODE_FULL )
		{
			osg::Matrixd mView							= m_pCam->getViewMatrix();
			osg::Matrixd mProj							= m_pCam->getProjectionMatrix();
			osgUtil::SceneView::ComputeNearFarMode cnfm	= m_pCam->getComputeNearFarMode();

			osg::Camera* pFollowCam = dynamic_cast<osg::Camera*>(node);
			pFollowCam->setViewMatrix( mView );
			pFollowCam->setProjectionMatrix( mProj );
			pFollowCam->setComputeNearFarMode( cnfm );
		}

		if ( m_eMode==MODE_SIMPLE )
		{
			osg::Matrixd mView		= m_pCam->getViewMatrix();
			osg::Camera* pFollowCam = dynamic_cast<osg::Camera*>(node);
			pFollowCam->setViewMatrix( mView );
		}

		if ( m_eMode==MODE_SIMPLE_OFFSET )
		{
			osg::Matrixd mView		= m_pCam->getViewMatrix();
			osg::Camera* pFollowCam = dynamic_cast<osg::Camera*>(node);

//			osg::Matrix m = osg::Matrix::rotate( osg::PI_2, osg::Vec3(0,0,1) );

			pFollowCam->setViewMatrix( mView );
		}

		/*
		if ( m_eMode==MODE_POSITION_XY )
		{
			osg::Matrixd mView	= m_pCam->getViewMatrix();
			osg::Camera* pFollowCam = dynamic_cast<osg::Camera*>(node);

			osg::Matrixd mViewMatrixFollow = pFollowCam->getViewMatrix();
			
			osg::Vec3 v0 = mView.getTrans();
			osg::Vec3 v1 = mViewMatrixFollow.getTrans();
			osg::Vec3 vNew( v0.x(), v0.y(), v1.z() );
			mViewMatrixFollow.setTrans( vNew );

			pFollowCam->setViewMatrix( mViewMatrixFollow );
		}
		*/

		if ( m_eMode==MODE_POSITION_XY )
		{
			osg::Matrixd mView0		= m_pCam->getInverseViewMatrix();

			osg::Camera* pCam1 = dynamic_cast<osg::Camera*>(node);
			osg::Matrixd mView1 = pCam1->getInverseViewMatrix();
			
			osg::Vec3 v0 = mView0.getTrans();
			osg::Vec3 v1 = mView1.getTrans();
			osg::Vec3 vNew( v0.x(), v0.y(), v1.z() );
			mView1.setTrans( vNew );

			mView1 = osg::Matrixd::inverse( mView1 );
			pCam1->setViewMatrix( mView1 );
		}


		if ( m_eMode==MODE_VIEWMATRIX )
		{
			osg::Matrixd mView	= m_pCam->getViewMatrix();
			osg::Camera* pFollowCam = dynamic_cast<osg::Camera*>(node);
			pFollowCam->setViewMatrix( mView );
		}
	}

protected:
	osg::Camera*		m_pCam;			// camera we pull data to sync from
	EMODECAM			m_eMode;
};

#endif // __SULNODECALLBACKCAMERASYNC_H__
