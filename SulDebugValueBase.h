// SulDebugValueBase.h

#ifndef __SULDEBUGVALUEBASE_H__
#define __SULDEBUGVALUEBASE_H__

#include "SulExport.h"
#include <osgText/Text>

class SUL_EXPORT CSulDebugValueBase
{
public:
						CSulDebugValueBase( const std::string& sTitle );

	virtual void		inc() = 0;
	virtual void		dec() = 0;

	const std::string&	getTitle();

	osgText::Text*		getTextTitleObj();
	osgText::Text*		getTextValueObj();

	void				setTextPosition( float x, float y );
	void				setTextColor( const osg::Vec4& c );
	void				setTextValue( const std::string& s );

private:
	std::string						m_sTitle;
	osg::ref_ptr<osgText::Text>		m_textTitle;
	osg::ref_ptr<osgText::Text>		m_textValue;
};




#endif // __SULDEBUGVALUEBASE_H__