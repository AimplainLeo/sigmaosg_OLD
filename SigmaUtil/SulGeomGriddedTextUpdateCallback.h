// SulGeomGriddedTextUpdateCallback.h

#ifndef __SULGEOMGRIDDEDTEXTUPDATECALLBACK_H__
#define __SULGEOMGRIDDEDTEXTUPDATECALLBACK_H__

#include <osg/nodecallback>
#include <osg/timer>

class CSulGeomGriddedTextUpdateCallback : public osg::NodeCallback
{
public:
	CSulGeomGriddedTextUpdateCallback( osg::Geode* geode )
	{
		m_geode = geode;
		m_bBlink = false;
	}

	void blink( bool bBlink )
	{
		m_bBlink = bBlink;

		// make sure we are visible (might have been in the off state while blinking)
		if ( !bBlink )
			m_geode->setNodeMask( -1 );
	}

	virtual void operator() ( osg::Node* node, osg::NodeVisitor* nv )
    {
		traverse(node,nv);

		if ( m_bBlink )
		{
			double m = osg::Timer::instance()->time_m();
	
		    int i = ((int)m%500)<250;
			m_geode->setNodeMask( i?-1:0 );
		}
	}

private:
	osg::ref_ptr<osg::Geode>	m_geode;
	bool						m_bBlink;
};

#endif // __SULGEOMGRIDDEDTEXTUPDATECALLBACK_H__