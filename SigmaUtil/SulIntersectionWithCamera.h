// SulIntersectionWithCamera.h

#ifndef __SULINTERSECTIONWITHCAMERA_H__
#define __SULINTERSECTIONWITHCAMERA_H__

#include "SulTexCam.h"
#include "SulIntersectionDrawCallback.h"
#include "SulIntersectionWithCameraCallback.h"
#include "SulIntersectionInfo.h"
#include <osg/referenced>
#include <osg/node>
#include <osg/group>

class CSulIntersectionWithCamera : public osg::Group
{
public:
					CSulIntersectionWithCamera( osg::Node* pNode, CSulIntersectionInfo* pIntersectionInfo );

	void			setPosition( const osg::Vec3& pos );
	void			setDirection( const osg::Vec3& dir );

	osg::Texture2D*	getTexture();
	
	void			shoot();

private:
	osg::ref_ptr<osg::Node>								m_rNode;
	osg::ref_ptr<CSulTexCam>							m_rTexCam;
	osg::ref_ptr<CSulIntersectionDrawCallback>			m_rDrawCallback;
	osg::ref_ptr<CSulIntersectionWithCameraCallback>	m_rSulIntersectionWithCameraCallback;
};

#endif // __SULINTERSECTIONWITHCAMERA_H__