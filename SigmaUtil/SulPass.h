// SulPass.h

#ifndef __SULPASS_H__
#define __SULPASS_H__

#include "SulString.h"
#include "SulTexCam.h"
#include "SulGeomQuad.h"
#include <osg/geode>

class CSulPass : public osg::Referenced
{
public:
								CSulPass( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut, const CSulString& sName );

	CSulTexCam*					getTexCam();
	CSulGeode*					getQuad();

protected:
	CSulGeomQuad*				create( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut, const std::string& sNameRTT );

private:
	osg::ref_ptr<CSulTexCam>	m_rTexCam;
	osg::ref_ptr<CSulGeode>		m_rQuad;
};

#endif // __SULPASS_H__