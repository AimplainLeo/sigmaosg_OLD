// SulGuiText.h

#ifndef __SULGUITEXT_H__
#define __SULGUITEXT_H__

#include "SulGuiComp.h"
#include <osgText/Text>

class CSulGuiText : public CSulGuiComp
{
public:
								CSulGuiText( const CSulString& sName, float x, float y, float size, CSulString font="Tempest.ttf" );

	virtual void				setupAttr( CSulXmlAttr* pAttr );

	virtual void				init();

	void						setText( const CSulString& sName );
	const CSulString&			getText() const;

	osgText::Text*				getTextObject();

	void						setFontSize( float size );

private:
	osg::ref_ptr<osgText::Text>	m_rText;
	CSulString					m_sText;
	float						m_size;
	CSulString					m_font;
};

#endif // __SULGUITEXT_H__