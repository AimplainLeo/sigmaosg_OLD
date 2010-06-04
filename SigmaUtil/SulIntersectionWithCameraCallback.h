// SulIntersectionWithCameraCallback.h

#ifndef __SULINTERSECTIONWITHCAMERACALLBACK_H__
#define __SULINTERSECTIONWITHCAMERACALLBACK_H__

#include "SulIntersectionDrawCallback.h"

class CSulIntersectionWithCameraCallback : public osg::NodeCallback
{
public:
	CSulIntersectionWithCameraCallback( CSulIntersectionDrawCallback* pDrawCallback )
	{
		m_rDrawCallback = pDrawCallback;
		m_bTest = false;
	}

    void operator()( osg::Node* node, osg::NodeVisitor* nv )
    {
		if ( m_bTest )
		{
			// check drawcallback 
			if ( m_rDrawCallback->isFrameRendered() )
			{
				m_rDrawCallback->shoot();
				m_bTest = false;
			}
		}

		// traverse subtree
        traverse( node, nv );
	}

	void shoot()
	{
		m_rDrawCallback->reset();
		m_bTest = true;
	}

private:
	osg::ref_ptr<CSulIntersectionDrawCallback>	m_rDrawCallback;
	bool										m_bTest;
};

#endif // __SULINTERSECTIONWITHCAMERACALLBACK_H__