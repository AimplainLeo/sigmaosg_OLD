// SulPostFilterStreaking.h

#ifndef __SULPOSTFILTERSTREAKING_H__
#define __SULPOSTFILTERSTREAKING_H__

#include "SulPostFilterBase.h"

class CSulPostFilterStreaking : public CSulPostFilterBase
{
public:

	void							in( osg::Texture2D* pTex );
	osg::Texture2D*					out();

private:
	osg::ref_ptr<osg::Texture2D>	m_rTex;
};

#endif // __SULPOSTFILTERSTREAKING_H__