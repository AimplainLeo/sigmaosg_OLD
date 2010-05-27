// DebugMenu.h

#ifndef __DEBUGMENU_H__
#define __DEBUGMENU_H__

#include <SigmaUtil/SulDebugMenu.h>
#include <SigmaUtil/SulDebugValueFloat.h>
#include <SigmaUtil/SulDebugValueText.h>
#include <SigmaUtil/SulPostFilterHDR2.h>

class CDebugMenu : public CSulDebugMenu
{
public:
	CDebugMenu( CSulPostFilterHDR2* pHDR )
	{
		m_rHDR = pHDR;

		add(
			new CSulDebugValueFloat(
				"brightpass",	// title
				0.0f, 10.0f,	// min, max
				1.2f,			// default
				0.1f			// step
			));

		add(
			new CSulDebugValueFloat(
				"exposure",		// title
				0.0f, 10.0f,	// min, max
				1.0f,			// default
				0.01f			// step
			));

		add(
			new CSulDebugValueFloat(
				"factor",		// title
				0.0f, 100.0f,	// min, max
				1.0f,			// default
				0.1f			// step
			));

		add(
			new CSulDebugValueFloat(
				"max",			// title
				0.0f, 100.0f,	// min, max
				1.0f,			// default
				0.1f			// step
			));

		add(
			new CSulDebugValueFloat(
				"glowFactor",	// title
				0.0f, 100.0f,	// min, max
				0.1f,			// default
				0.01f			// step
			));

		add(
			new CSulDebugValueFloat(
			"bp falloff",	// title
			0.0f, 1.0f,	// min, max
			0.5f,			// default
			0.05f			// step
			));

	}

	virtual void valueChanged( CSulDebugValueBase* pObj )
	{
		CSulString sTitle = pObj->getTitle();

		if ( sTitle=="bp falloff" )
		{
			float val = ((CSulDebugValueFloat*)pObj)->getVal();
			m_rHDR->getShaderBrightPass()->setFalloff( val );
		}

		if ( sTitle=="brightpass" )
		{
			float val = ((CSulDebugValueFloat*)pObj)->getVal();
			m_rHDR->getShaderBrightPass()->setThreshold( val );
		}

		if ( sTitle=="exposure" )
		{
			if ( m_rHDR->getShaderCombine() )
			{
				float val = ((CSulDebugValueFloat*)pObj)->getVal();
				m_rHDR->getShaderCombine()->setExposure( val );
			}
		}

		if ( sTitle=="factor" )
		{
			float val = ((CSulDebugValueFloat*)pObj)->getVal();
			m_rHDR->getShaderCombine()->setFactor( val );
		}

		if ( sTitle=="max" )
		{
			float val = ((CSulDebugValueFloat*)pObj)->getVal();
			m_rHDR->getShaderCombine()->setMax( val );
		}

		if ( sTitle=="glowFactor" )
		{
			float val = ((CSulDebugValueFloat*)pObj)->getVal();
			m_rHDR->setGlowFactor( val );
		}
	}

private:
	osg::ref_ptr<CSulPostFilterHDR2>	m_rHDR;
};

#endif // __DEBUGMENU_H__