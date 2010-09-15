// SulPostFilterGodRays.h

#ifndef __SULPOSTFILTERGODRAYS_H__
#define __SULPOSTFILTERGODRAYS_H__

#include "SulPostFilterBase.h"
#include "SulPassGodRays.h"

class CSulPostFilterGodRays : public CSulPostFilterBase
{
public:
	void			in( osg::Texture2D* pTex_normal, osg::Texture2D* pTex_occluding );
	osg::Texture2D* out();

	CSulPassGodRays*	getPassGodRays();

private:
	osg::ref_ptr<osg::Texture2D>		m_rTex;
	osg::ref_ptr<CSulPassGodRays>		m_rPassGodRays;
};


#endif // __SULPOSTFILTERGODRAYS_H__

