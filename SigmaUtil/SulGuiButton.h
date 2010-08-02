// SulGuiButton.h

#ifndef __SULGUIBUTTON_H__
#define __SULGUIBUTTON_H__

#include "SulGuiCanvas.h"
#include "SulGuiText.h"

class CSulGuiButton : public CSulGuiCanvas
{
public:
			CSulGuiButton( const CSulString& sText, float x, float y, float w, float h );

protected:
	void	setMouseHover( bool bHover );

private:
	osg::ref_ptr<CSulGuiText>	m_rText;
};

#endif // __SULGUIBUTTON_H__