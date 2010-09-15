// SulFilterPass.h

#ifndef __SULFILTERPASS_H__
#define __SULFILTERPASS_H__

#include "SulTexCam.h"

class CSulFilterPass : public osg::Referenced
{
public:
	osg::Geode*		create( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut, const std::string& sNameRTT );

	CSulTexCam*		getTexCam();
	osg::Geode*		getGeode();

private:
	osg::ref_ptr<CSulTexCam>	m_rTexCam;
	osg::ref_ptr<osg::Geode>	m_rGeode;
};

#endif // __SULFILTERPASS_H__