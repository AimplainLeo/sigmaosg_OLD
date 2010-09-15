// SulPostFilterBloom.h

#ifndef __SULPOSTFILTERBLOOM_H__
#define __SULPOSTFILTERBLOOM_H__

#include "SulPostFilterBase.h"

class CSulPostFilterBloom : public CSulPostFilterBase
{
public:
					CSulPostFilterBloom();

	void			in( osg::Texture2D* pTex, int iDownSampleSize );
	osg::Texture2D* out();

private:
	void			filterBrightPass( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut );
	void			filterGaussH( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut );
	void			filterGaussV( osg::Texture2D* pTexIn, osg::Texture2D* pTexOut );
	void			filterCombine( osg::Texture2D* pTexOrg, osg::Texture2D* pTexIn, osg::Texture2D* pTexOut );

private:
	osg::ref_ptr<osg::Texture2D>		m_rTex;
	float								m_fExp;
	float								m_fFactor;
	float								m_fMax;
};

#endif // __SULPOSTFILTERBLOOM_H__