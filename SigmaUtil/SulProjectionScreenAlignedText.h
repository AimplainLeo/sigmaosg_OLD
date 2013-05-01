// SulRenderableScreenAlignedText.h

#ifndef __SULRENDERABLESCREENALIGNEDTEXT_H__
#define __SULRENDERABLESCREENALIGNEDTEXT_H__

#include "SulString.h"
#include <osg/projection>
#include <osgText/text>
#include "SulExport.h"

class SUL_EXPORT CSulProjectionScreenAlignedText : public osg::Projection
{
public:
	CSulProjectionScreenAlignedText();

	void setText( const CSulString& text );
	void setPosition( const osg::Vec2& pos );

private:
	osg::ref_ptr<osgText::Text>		m_text;
};

#endif // __SULRENDERABLESCREENALIGNEDTEXT_H__