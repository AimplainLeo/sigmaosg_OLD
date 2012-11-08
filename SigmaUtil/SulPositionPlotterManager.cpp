// SulPositionPlotterManager.cpp

#include "stdafx.h"
#include "SulPositionPlotterManager.h"
#include "SulGeodeLineStrip.h"

void CSulPositionPlotterManager::add( CSulPositionPlotter* plotterPosition )
{
	m_vecPositionPlotter.push_back( plotterPosition );
}

osg::Node* CSulPositionPlotterManager::createPath()
{
	osg::Group* group = new osg::Group;

	// step through each plotter object
	VEC_POSITIONPLOTTER::iterator i = m_vecPositionPlotter.begin();
	VEC_POSITIONPLOTTER::iterator ie = m_vecPositionPlotter.end();

	while ( i!=ie )
	{
		CSulGeodeLineStrip* p = new CSulGeodeLineStrip( (*i)->getPositions(), osg::Vec4(0,1,0,1) );
	
		p->setWidth( 5.0f );
		group->addChild( p );
		++i;
	}

	return group;
}