// DebugMenu.h

#ifndef __DEBUGMENU_H__
#define __DEBUGMENU_H__

#include <SigmaUtil/SulDebugMenu.h>
#include <SigmaUtil/SulDebugValueFloat.h>
#include <SigmaUtil/SulDebugValueText.h>
#include <SigmaUtil/SulPostFilterGodRays.h>

class CDebugMenu : public CSulDebugMenu
{
public:
	CDebugMenu( CSulPostFilterGodRays* pGodRays )
	{
		m_rGodRays = pGodRays;

		add(
			new CSulDebugValueFloat(
				"exposure",		// title
				0.0f, 10.0f,	// min, max
				0.1f,			// default
				0.1f			// step
			));

		add(
			new CSulDebugValueFloat(
				"decay",		// title
				0.0f, 10.0f,	// min, max
				0.95f,			// default
				0.01f			// step
			));

		add(
			new CSulDebugValueFloat(
				"density",		// title
				0.0f, 10.0f,	// min, max
				1.22f,			// default
				0.01f			// step
			));

		add(
			new CSulDebugValueFloat(
				"weight",		// title
				0.0f, 10.0f,	// min, max
				0.55f,			// default
				0.01f			// step
			));
	}

	virtual void valueChanged( CSulDebugValueBase* pObj )
	{
		CSulString sTitle = pObj->getTitle();

		if ( sTitle=="exposure" )
		{
			float val = ((CSulDebugValueFloat*)pObj)->getVal();
			m_rGodRays->getPassGodRays()->getShader()->setExposure( val );
		}

		if ( sTitle=="decay" )
		{
			float val = ((CSulDebugValueFloat*)pObj)->getVal();
			m_rGodRays->getPassGodRays()->getShader()->setDecay( val );
		}

		if ( sTitle=="density" )
		{
			float val = ((CSulDebugValueFloat*)pObj)->getVal();
			m_rGodRays->getPassGodRays()->getShader()->setDensity( val );
		}

		if ( sTitle=="weight" )
		{
			float val = ((CSulDebugValueFloat*)pObj)->getVal();
			m_rGodRays->getPassGodRays()->getShader()->setWeight( val );
		}
	}

private:
	osg::ref_ptr<CSulPostFilterGodRays>	m_rGodRays;

};


#endif // __DEBUGMENU_H__