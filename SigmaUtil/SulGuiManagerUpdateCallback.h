// SulGuiManagerUpdateCallback.h

#ifndef __SULGUIMANAGERUPDATECALLBACK_H__
#define __SULGUIMANAGERUPDATECALLBACK_H__

#include "SulSigSlots.h"
#include <osgViewer/view>

class CSulGuiManagerUpdateCallback : public osg::NodeCallback
{
public:
	CSulGuiManagerUpdateCallback( osgViewer::View* pView )
	{
		m_rView = pView;

		m_w = pView->getCamera()->getViewport()->width();
		m_h = pView->getCamera()->getViewport()->height();
	}

    void operator()( osg::Node* node, osg::NodeVisitor* nv )
    {
		float w = m_rView->getCamera()->getViewport()->width();
		float h = m_rView->getCamera()->getViewport()->height();

		if ( w!=m_w || h!=m_h )
		{
			m_w = w;
			m_h = h;
			signalSizeChanged( w, h );
		}

		traverse( node, nv );
	}

	sigma::signal2<float, float> signalSizeChanged;

private:
	osg::ref_ptr<osgViewer::View>	m_rView;
	float							m_w;
	float							m_h;
};

#endif // __SULGUIMANAGERUPDATECALLBACK_H__