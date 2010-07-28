// SulGuiManager.h

#ifndef __SULGUIMANAGER_H__
#define __SULGUIMANAGER_H__

#include "SulString.h"
#include "SulGuiEventHandler.h"
#include <osg/group>
#include <osg/projection>
#include <osg/matrixtransform>
#include <osgViewer/Viewer>

class CSulGuiManager : public osg::Projection
{
public:
			CSulGuiManager( osgViewer::Viewer* pViewer, float viewW, float viewH );

	bool	load( const CSulString& sFileXml, osg::Group* pParent=0 );

private:
	osg::ref_ptr<osg::MatrixTransform>	m_rMT;
	osg::ref_ptr<osgViewer::Viewer>		m_rViewer;
	osg::ref_ptr<CSulGuiEventHandler>	m_rEventHandler;
};

#endif // __SULGUIMANAGER_H__