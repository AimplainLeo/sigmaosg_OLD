// SulPostFilterHDR.h

#ifndef __SULPOSTFILTERHDR_H__
#define __SULPOSTFILTERHDR_H__

#include "SulPostFilterBase.h"
#include "SulShaderBrightPass.h"
#include "SulShaderCombine.h"
#include "SulFilterPass.h"
#include "SulString.h"

class CSulPostFilterHDR : public CSulPostFilter
{
public:
							CSulPostFilterHDR();

	void					setView( const CSulString& sView );

	void					in( osg::Texture2D* pTex, int iDownSampleSize );
	osg::Texture2D*			out();

	CSulShaderBrightPass*	getShaderBrightPass();
	CSulShaderCombine*		getShaderCombine();

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

	osg::ref_ptr<CSulShaderBrightPass>	m_rShaderBrightPass;
	osg::ref_ptr<CSulShaderCombine>		m_rShaderCombine;

	osg::ref_ptr<CSulFilterPass>		m_rFilterBrightPass;

	CSulString							m_sView;
};

#endif // __SULPOSTFILTERHDR_H__