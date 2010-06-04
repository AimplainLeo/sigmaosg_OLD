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
	}

	bool isFrameRendered()
	{
		return m_bTest;
	}

	void reset()
	{
		m_bTest = false;
	}

	// NOTE: this is called by the frameupdate on the mainthread
	void shoot()
	{
		// FIXME: need to add hit point to m_rIntersectionInfo
		
		m_rIntersectionInfo->setHit( m_hit );

		m_rIntersectionInfo->update();
	}

public:
	void operator()( osg::RenderInfo& renderInfo ) const
	{
		if ( !m_bTest )
		{
			

			//m_hit = based on texture and inversed camera
			m_bTest = true;
		}
	}

private:
	mutable bool						m_bTest;
	osg::ref_ptr<CSulIntersectionInfo>	m_rIntersectionInfo;
	osg::Vec3							m_hit;
	osg::ref_ptr<osg::Image>			m_rImage;
};

#endif // __SULINTERSECTIONDRAWCALLBACK_H__