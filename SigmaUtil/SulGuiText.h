// SulGuiText.h

#ifndef __SULGUITEXT_H__
#define __SULGUITEXT_H__

#include "SulGuiComp.h"
#include "SulExport.h"
#include <osgText/Text>

class SUL_EXPORT CSulGuiText : public CSulGuiComp
{
public:
								CSulGuiText( const CSulString& sName, float x, float y, float size, CSulString font="Tempest.ttf" );

	virtual void				setupTheme( CSulGuiThemeXml* pThemeXml );
	virtual void				setupAttr( CSulXmlAttr* pAttr );

	virtual void				init();

	void						setText( const CSulString& sName );
	const CSulString&			getText() const;

	osgText::Text*				getTextObject();

	void						setFontSize( float size );

	void						setColor( const osg::Vec4& color );

	virtual class CSulGuiText*	asText()		{ return this; }

private:
	osg::ref_ptr<osgText::Text>	m_rText;
	CSulString					m_sText;
	float						m_size;
	CSulString					m_font;
	osg::Vec4					m_color;
};

#endif // __SULGUITEXT_H__