// SulGuiTextUpdateCallback.h

#ifndef __SULGUITEXTUPDATECALLBACK_H__
#define __SULGUITEXTUPDATECALLBACK_H__

#include "SulGuiEditBox.h"
#include <osg/timer>

class CSulGuiTextUpdateCallback : public osg::NodeCallback
{
public:
	CSulGuiTextUpdateCallback( CSulGuiEditBox* pGuiEditBox )
	{
		m_rGuiEditBox = pGuiEditBox;
	}

    void operator()( osg::Node* node, osg::NodeVisitor* nv )
    {
        int i = ((int)osg::Timer::instance()->time_s()%2);
		m_rGuiEditBox->setCursor( i?"|":" " );
		traverse( node, nv );
	}

private:
	osg::ref_ptr<CSulGuiEditBox>	m_rGuiEditBox;
};

#endif // __SULGUITEXTUPDATECALLBACK_H__