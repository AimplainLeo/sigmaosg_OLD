// SulPass.h

#ifndef __SULPASS_H__
#define __SULPASS_H__

#include "SulString.h"
#include "SulTexCam.h"
#include <osg/geode>

class CSulPass : public osg::Referenced
{
public:
								CSulPass( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut, const CSulString& sName );

	CSulTexCam*					getTexCam();
	osg::Geode*					getGeode();

protected:
	osg::Geode*					create( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut, const std::string& sNameRTT );

private:
	osg::ref_ptr<CSulTexCam>	m_rTexCam;
	osg::ref_ptr<osg::Geode>	m_rGeode;
};

#endif // __SULPASS_H__