// SulIntersectionCullCallback.h

#ifndef __SULINTERSECTIONCULLCALLBACK_H__
#define __SULINTERSECTIONCULLCALLBACK_H__

#include "SulIntersectionDrawCallback.h"
#include <osg/drawable>
#include <osgUtil/CullVisitor>

class CSulIntersectionCullCallback : public osg::NodeCallback
{
public:
	CSulIntersectionCullCallback( CSulIntersectionDrawCallback* pDrawCallback )
	{
		m_pDrawCallback = pDrawCallback;
	}

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    { 
		traverse(node,nv);

        osgUtil::CullVisitor* cv = dynamic_cast<osgUtil::CullVisitor *>(nv);
		if ( cv )
		{
			m_pDrawCallback->setNear( cv->getCalculatedNearPlane() );
			m_pDrawCallback->setFar( cv->getCalculatedFarPlane() );
			m_pDrawCallback->setProjectionMatrix( *cv->getProjectionMatrix() );
		}
        
    }

private:
	CSulIntersectionDrawCallback* m_pDrawCallback;
};

#endif // __SULINTERSECTIONCULLCALLBACK_H__