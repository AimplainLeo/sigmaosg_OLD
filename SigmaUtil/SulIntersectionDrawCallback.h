// SulIntersectionDrawCallback.h

#ifndef __SULINTERSECTIONDRAWCALLBACK_H__
#define __SULINTERSECTIONDRAWCALLBACK_H__

#include "SulIntersectionInfo.h"
#include <osg/Camera>

class CSulIntersectionDrawCallback : public osg::Camera::DrawCallback
{
public:
	CSulIntersectionDrawCallback( CSulIntersectionInfo* pIntersectionInfo, osg::Image* pImage )
	{
		m_rImage = pImage;
		m_rIntersectionInfo = pIntersectionInfo;
		m_bTest = false;
ii = 0;
	}

	bool isFrameRendered()
	{
		return !m_bTest;
	}

	void reset()
	{
		m_bTest = true;
	}

	// NOTE: this is called by the frameupdate on the mainthread
	void shoot()
	{
		m_rIntersectionInfo->setHit( m_hit );
		m_rIntersectionInfo->update();
	}

public:
	void operator()( const osg::Camera& cam ) const
	{
		if ( m_bTest )
		{
			ii++;

			if ( ii==2 )
			{	
				Sigma::int32 w = m_rImage->s();
				Sigma::int32 h = m_rImage->t();

				//m_hit = based on texture and inversed camera
				osg::Vec3 v = (*(osg::Vec3*)m_rImage->data(w/2,h/2));
				osg::Matrix m = cam.getInverseViewMatrix();
				m_hit = v*m;
			
				ii = 0;	
				m_bTest = false;
			}
		}
	}

private:
	mutable bool						m_bTest;
	osg::ref_ptr<CSulIntersectionInfo>	m_rIntersectionInfo;
	mutable osg::Vec3					m_hit;
	osg::ref_ptr<osg::Image>			m_rImage;
	mutable int							ii;							// used to make sure we render atleast two frames (if the main thread should have requested while in operator)
};

#endif // __SULINTERSECTIONDRAWCALLBACK_H__