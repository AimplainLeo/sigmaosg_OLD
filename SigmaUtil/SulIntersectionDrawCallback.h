// SulIntersectionDrawCallback.h

#ifndef __SULINTERSECTIONDRAWCALLBACK_H__
#define __SULINTERSECTIONDRAWCALLBACK_H__

#include "SulIntersectionInfo.h"
#include <osg/Camera>
#include <osg/io_utils>

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

	void setNear( float n )
	{
		m_near = n;
	}

	void setFar( float f )
	{
		m_far = f;
	}

	void setProjectionMatrix( const osg::Matrix& proj )
	{
		m_proj = proj;
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
				sigma::int32 w = m_rImage->s();
				sigma::int32 h = m_rImage->t();

				//m_hit = based on texture and inversed camera
				//osg::Vec3 v = (*(osg::Vec3*)m_rImage->data(w/2,h/2));
				//osg::Matrix m = cam.getInverseViewMatrix();
				//m_hit = v*m;

				float zm = (*(float*)m_rImage->data(w/2,h/2));

				osg::Image* pTest = new osg::Image;
				pTest->readPixels( 400,300,1,1, GL_DEPTH_COMPONENT, GL_FLOAT ); 
				float f = (*(float*)pTest->data());
				zm = f;

				//float z = -4.f * m_far * m_near / ((zm+1.f) * (m_far-m_near) - 2*(m_far+m_near));
				//float z = -m_far*m_near / (m_far*(zm-1.f) - m_near*zm);
//				float z = -m_near / (-m_far - zm * (m_far-m_near)) * m_far;
				float z = m_near * m_far / (zm*m_near - zm*m_far + m_far);

				//osg::Matrix mi = m_proj.inverse( m_proj );
				//float z = (osg::Vec3(0, 0, zm) * mi).z();

				osg::Matrix m = cam.getInverseViewMatrix();
				m_hit = osg::Vec3(0, 0, -z) * m;


				osg::notify(osg::ALWAYS) << 
					"\n *************************************" << 
					"\n zm: " << zm << 
					"\n z: " << z << 
					"\n hit y : " << m_hit.y() << 
					"\n cam world: " << osg::Vec3(0,0,0) * m << 
					"\n m_near : " << m_near << 
					"\n m_far : " << m_far << 
					"\n readpixels : " << f << 
					std::endl;
			
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

	float								m_near;
	float								m_far;
	osg::Matrix							m_proj;
};

#endif // __SULINTERSECTIONDRAWCALLBACK_H__