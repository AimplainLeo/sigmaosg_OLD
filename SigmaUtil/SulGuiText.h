// SulGuiText.h

#ifndef __SULGUITEXT_H__
#define __SULGUITEXT_H__

#include "SulGuiComp.h"
#include <osgText/Text>

class CSulGuiText : public CSulGuiComp
{
public:
	CSulGuiText( const CSulString& sName, float x, float y, float w, float h, float size );

	void	setName( const CSulString& sName );

private:
	osg::ref_ptr<osgText::Text>	m_rText;
};

#endif // __SULGUITEXT_H__