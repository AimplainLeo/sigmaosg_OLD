// SulProfiler.cpp

#include "stdafx.h"
#include "SulProfiler.h"
#include "SulGeomBarChart.h"
#include <iostream>

CSulProfiler::CSulProfiler()
{
	/*
	CSulGeomBarChart* p = new CSulGeomBarChart;
	m_rGeode = p->createGeode();
	*/
}

void CSulProfiler::create( const CSulString& name, const CSulString& groupName )
{
	MAP_GROUP::iterator iFound = m_mapGroup.find( groupName );
	if ( iFound==m_mapGroup.end() )
	{
		m_mapGroup[groupName] = new MAP_DATA;
	}

	MAP_DATA* data = m_mapGroup[groupName];

	(*data)[name] = new CSulProfilerData;
}

void CSulProfiler::start( const CSulString& name, const CSulString& groupName )
{
	MAP_DATA* data = m_mapGroup[groupName];
	CSulProfilerData* p = (*data)[name];
	p->start();
}

void CSulProfiler::end( const CSulString& name, const CSulString& groupName )
{
	MAP_DATA* data = m_mapGroup[groupName];
	CSulProfilerData* p = (*data)[name];
	p->end();
}

void CSulProfiler::frameUpdate()
{
	// reset all data
	MAP_GROUP::iterator iS, iE;

	iS = m_mapGroup.begin();
	iE = m_mapGroup.end();

	while ( iS!=iE )
	{
		MAP_DATA::iterator iiS, iiE;
		iiS = iS->second->begin();
		iiE = iS->second->end();
		while ( iiS!=iiE )
		{
			iiS->second->reset();
			++iiS;
		}

		++iS;
	}
}

void CSulProfiler::dumpData( CSulProfilerData* pData )
{
	MAP_GROUP::iterator iS, iE;

	iS = m_mapGroup.begin();
	iE = m_mapGroup.end();

	while ( iS!=iE )
	{
		MAP_DATA::iterator iiS, iiE;
		iiS = iS->second->begin();
		iiE = iS->second->end();
		while ( iiS!=iiE )
		{
			double dt = iiS->second->getTime();
			std::cout << "time: " << dt << std::endl;
			++iiS;
		}

		++iS;
	}
}

void CSulProfiler::dump()
{
	MAP_GROUP::iterator iS, iE;

	iS = m_mapGroup.begin();
	iE = m_mapGroup.end();

	while ( iS!=iE )
	{
		MAP_DATA::iterator iiS, iiE;
		iiS = iS->second->begin();
		iiE = iS->second->end();
		while ( iiS!=iiE )
		{
			dumpData( iiS->second );
			++iiS;
		}

		++iS;
	}
}



