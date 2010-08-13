// SulGuiTextBox.h

#ifndef __SULGUITEXTBOX_H__
#define __SULGUITEXTBOX_H__

#include "SulGuiCanvas.h"
#include "SulGuiText.h"

class CSulGuiTextBox : public CSulGuiCanvas
{
public:
					CSulGuiTextBox( float x, float y );
					CSulGuiTextBox( const CSulString& sText, float x, float y, float w, float h, float fontSize );

	virtual void	setupTheme( CSulGuiThemeXml* pThemeXml );
	virtual void	setupAttr( CSulXmlAttr* pAttr );

	virtual void	init();

private:
	osg::ref_ptr<CSulGuiText>	m_rText;
	CSulString					m_sText;
	float						m_fontSize;
	float						m_ofs_x;
	float						m_ofs_y;
};

#endif // __SULGUITEXTBOX_H__