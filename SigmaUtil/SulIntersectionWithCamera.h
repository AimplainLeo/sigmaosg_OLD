// SulIntersectionWithCamera.h

#ifndef __SULINTERSECTIONWITHCAMERA_H__
#define __SULINTERSECTIONWITHCAMERA_H__

#include <osg/referenced>
#include <osg/node>

class CSulIntersectionWithCamera : public osg::Referenced
{
public:
	void		setNode( osg::Node* pNode );

private:
	osg::ref_ptr<osg::Node>		m_rNode;
};

#endif // __SULINTERSECTIONWITHCAMERA_H__