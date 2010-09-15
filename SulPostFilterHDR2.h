// SulPostFilterHDR2.h

#ifndef __SULPOSTFILTERHDR2_H__
#define __SULPOSTFILTERHDR2_H__

#include "SulPassBrightPass.h"
#include "SulPassToneMap.h"
#include "SulPassStreaking.h"
#include "SulPostFilterBase.h"
#include "SulShaderBrightPass.h"
#include "SulShaderCombine.h"
#include "SulFilterPass.h"
#include "SulString.h"
#include "SulExport.h"

class SUL_EXPORT CSulPostFilterHDR2 : public CSulPostFilterBase
{
public:
							CSulPostFilterHDR2();

	void					setView( const CSulString& sView );
	void					setGlowFactor( float glowFactor );

	void					in( osg::Texture2D* pTex );
	osg::Texture2D*			out();

	CSulShaderBrightPass*	getShaderBrightPass();
	CSulShaderCombine*		getShaderCombine();

osg::ref_ptr<CSulTextureBuffer> texLum;

private:
	osg::ref_ptr<osg::Texture2D>		m_rTex;
	float								m_fExp;
	float								m_fFactor;
	float								m_fMax;

	osg::ref_ptr<CSulPassBrightPass>	m_rPassBrightPass;
	osg::ref_ptr<CSulPassToneMap>		m_rPassToneMap;
	osg::ref_ptr<CSulPassStreaking>		m_rStreak0;
	osg::ref_ptr<CSulPassStreaking>		m_rStreak1;
	osg::ref_ptr<CSulPassStreaking>		m_rStreak2;
	osg::ref_ptr<CSulPassStreaking>		m_rStreak3;

	CSulString							m_sView;
};

#endif // __SULPOSTFILTERHDR2_H__