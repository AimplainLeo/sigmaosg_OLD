// SulProfilerData.h

#ifndef __SULPROFILERDATA_H__
#define __SULPROFILERDATA_H__

#include "SulGeomBarChart.h"
#include <osg/Referenced>
#include <osg/timer>
#include <iostream>
#include <assert.h>

class CSulProfilerData : public osg::Referenced
{
public:
	CSulProfilerData::CSulProfilerData( const CSulString& name, CSulGeomBarChart* pChart )
	{
		b = true;
		m_name = name;
		m_rChart = pChart;
		m_indexBar = m_rChart->createBar( name );
		reset();
	}

	void reset()
	{
		m_total = 0.0;
	}

	void start()
	{
		if ( !b )
			return;

		b = false;
		m_time.setStartTick();
	}

	void end()
	{
		if ( b )
			return;

		b = true;
		m_total += m_time.time_s();
	}

	double getTime()
	{
		return m_total;
	}

	void frameUpdate()
	{
		if ( !b )
			return;

		m_rChart->setBarSize( m_indexBar, getTime()*10000.0f );
//		std::cout << "time (" << m_name << "): " << getTime()*10000.0f << std::endl;
		reset();
	}

private:
	osg::Timer m_time;
	double m_total;
	osg::ref_ptr<CSulGeomBarChart>	m_rChart;
	sigma::uint32 m_indexBar;
	CSulString m_name;

	bool b;
};

#endif // __SULPROFILERDATA_H__