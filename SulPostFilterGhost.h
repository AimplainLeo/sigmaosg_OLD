// SulPostFilterGhost.h

#ifndef __SULPOSTFILTERGHOST_H__
#define __SULPOSTFILTERGHOST_H__

#include "SulPostFilterBase.h"

class CSulPostFilterGhost : public CSulPostFilterBase
{
public:
	void			in( osg::Texture2D* pTex );
	osg::Texture2D* out();

private:
	osg::ref_ptr<osg::Texture2D>		m_rTex;
};


#endif // __SULPOSTFILTERGHOST_H__