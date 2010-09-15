// SulIntersectionWithCamera.h

#ifndef __SULINTERSECTIONWITHCAMERA_H__
#define __SULINTERSECTIONWITHCAMERA_H__

#include "SulTexCam.h"
#include "SulIntersectionDrawCallback.h"
#include "SulIntersectionWithCameraCallback.h"
#include "SulIntersectionInfo.h"
#include "SulExport.h"
#include <osg/referenced>
#include <osg/node>
#include <osg/group>

class SUL_EXPORT CSulIntersectionWithCamera : public osg::Group
{
public:
					CSulIntersectionWithCamera( osg::Node* pNode, CSulIntersectionInfo* pIntersectionInfo );

	void			setPosition( const osg::Vec3& pos );
	void			setDirection( const osg::Vec3& dir );

	void			setLine( const osg::Vec3& v0, const osg::Vec3& v1 );

	osg::Texture2D*	getTexture();
	
	void			shoot();

	bool			IsReady();

private:
	osg::ref_ptr<osg::Node>								m_rNode;
	osg::ref_ptr<CSulTexCam>							m_rTexCam;
	osg::ref_ptr<CSulIntersectionDrawCallback>			m_rDrawCallback;
	osg::ref_ptr<CSulIntersectionWithCameraCallback>	m_rSulIntersectionWithCameraCallback;
};

#endif // __SULINTERSECTIONWITHCAMERA_H__