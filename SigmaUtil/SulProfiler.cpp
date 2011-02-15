// SulProfiler.cpp

#include "stdafx.h"
#include "SulProfiler.h"
#include <iostream>

CSulProfiler::CSulProfiler()
{
	m_rChart = new CSulGeomBarChart;
}

CSulGeomBarChart* CSulProfiler::getChart()
{
	return m_rChart;
}

void CSulProfiler::create( const CSulString& name, const CSulString& groupName )
{
	MAP_GROUP::iterator iFound;

	// we need to check to see if the name and groupName already exists, if it does
	// then we do nothing
	iFound = m_mapGroup.find( groupName );
	if ( iFound!=m_mapGroup.end() )
	{
		MAP_DATA::iterator iFoundData;
		iFoundData = iFound->second->find( name );
		if ( iFoundData!=iFound->second->end() )
		{
			return;
		}
	}
	
	if ( iFound==m_mapGroup.end() )
	{
		m_mapGroup[groupName] = new MAP_DATA;
	}

	MAP_DATA* data = m_mapGroup[groupName];

	(*data)[name] = new CSulProfilerData( name, m_rChart );
}

void CSulProfiler::start( const CSulString& name, const CSulString& groupName )
{
	MAP_GROUP::iterator iGroup = m_mapGroup.find( groupName );
	if ( iGroup==m_mapGroup.end() )
		return;

	MAP_DATA* data = iGroup->second;

	MAP_DATA::iterator iData = data->find( name );
	if ( iData==data->end() )
		return;

	CSulProfilerData* p = iData->second;
	p->start();
}

void CSulProfiler::end( const CSulString& name, const CSulString& groupName )
{
	MAP_GROUP::iterator iGroup = m_mapGroup.find( groupName );
	if ( iGroup==m_mapGroup.end() )
		return;

	MAP_DATA* data = iGroup->second;

	MAP_DATA::iterator iData = data->find( name );
	if ( iData==data->end() )
		return;

	CSulProfilerData* p = iData->second;
	p->end();
}

void CSulProfiler::frameUpdate()
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
			iiS->second->frameUpdate();
			++iiS;
		}

		++iS;
	}
}

void CSulProfiler::dumpData( CSulProfilerData* pData )
{
	/*
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
			//std::cout << "time: " << dt << std::endl;
			++iiS;
		}

		++iS;
	}
	*/
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



