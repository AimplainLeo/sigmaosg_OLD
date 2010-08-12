// SulGenHeightTexture.h

#ifndef __SULGENHEIGHTTEXTURE_H__
#define __SULGENHEIGHTTEXTURE_H__

#include "SulTypes.h"
#include "SulTexCam.h"
#include <osg/group>

class CSulGenHeightTexture : public osg::Group
{
public:
						CSulGenHeightTexture( osg::Node* pRenderMe,	sigma::uint32 texSizeXY );
		
	void				generate();
	osg::Texture2D*		getTexture();

private:
	osg::ref_ptr<osg::Node>		m_rRenderMe;
	osg::ref_ptr<CSulTexCam>	m_rTexCam;
};

#endif // __SULGENHEIGHTTEXTURE_H__