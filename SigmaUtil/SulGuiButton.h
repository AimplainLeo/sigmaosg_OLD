// SulGuiButton.h

#ifndef __SULGUIBUTTON_H__
#define __SULGUIBUTTON_H__

#include "SulGuiCanvas.h"
#include "SulGuiText.h"
#include "SulGuiButtonMediator.h"

class CSulGuiButton : public CSulGuiCanvas
{
public:
			CSulGuiButton( const CSulString& sText, float x, float y, float w, float h );
			
			CSulGuiButton( float x, float y, float w, float h );

	void	setupEventHandler( CSulGuiEventHandler* pEventHandler );

	void	addMediator( CSulGuiButtonMediatorBase* p );

	void	setMouseRelease( bool bInside );

protected:
	void	init();
	void	setMouseHover( bool bHover );

private:
	osg::ref_ptr<CSulGuiText>	m_rText;

CSulGuiButtonMediatorBase* m_test;
};

#endif // __SULGUIBUTTON_H__