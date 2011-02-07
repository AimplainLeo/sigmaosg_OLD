// SulFilterPass.h

#ifndef __SULFILTERPASS_H__
#define __SULFILTERPASS_H__

#include "SulTexCam.h"
#include <osg/group>

class CSulFilterPass : public osg::Referenced
{
public:
	osg::Group*		create( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut, const std::string& sNameRTT );

	CSulTexCam*		getTexCam();
	osg::Group*		getGroup();

private:
	osg::ref_ptr<CSulTexCam>	m_rTexCam;
	osg::ref_ptr<osg::Group>	m_rGroup;
};

#endif // __SULFILTERPASS_H__