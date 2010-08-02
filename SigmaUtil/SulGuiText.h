// SulGuiText.h

#ifndef __SULGUITEXT_H__
#define __SULGUITEXT_H__

#include "SulGuiComp.h"
#include <osgText/Text>

class CSulGuiText : public CSulGuiComp
{
public:
								CSulGuiText( const CSulString& sName, float x, float y, float size );

	void						setText( const CSulString& sName );
	const CSulString&			getText() const;

	osgText::Text*				getTextObject();

private:
	osg::ref_ptr<osgText::Text>	m_rText;
	CSulString					m_sText;
};

#endif // __SULGUITEXT_H__