// SulGuiButton.h

#ifndef __SULGUIBUTTON_H__
#define __SULGUIBUTTON_H__

#include "SulGuiCanvas.h"
#include "SulGuiText.h"
#include "SulSigSlots.h"

class CSulGuiButton : public CSulGuiCanvas
{
public:
					CSulGuiButton();
					CSulGuiButton( const CSulString& sText, float x, float y, float w, float h );
					CSulGuiButton( float x, float y, float w, float h );

	virtual void	setupTheme( CSulGuiThemeXml* pThemeXml );
	virtual void	setupAttr( CSulXmlAttr* pAttr );

	virtual void	init();

	void			setText( const CSulString& sText );
	void			setFontSize( float size );

	void			createDownDecal();
	void			createUpDecal();

	CSulGuiButton*	asButton() { return this; }

protected:
	void			onHover( bool bHover );

private:
	osg::ref_ptr<CSulGuiText>	m_rText;
	CSulString					m_sText;
	float						m_sizeFont;
};

#endif // __SULGUIBUTTON_H__