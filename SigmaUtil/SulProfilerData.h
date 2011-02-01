// SulProfilerData.h

#ifndef __SULPROFILERDATA_H__
#define __SULPROFILERDATA_H__

#include <osg/Referenced>
#include <osg/timer>

class CSulProfilerData : public osg::Referenced
{
public:
	CSulProfilerData::CSulProfilerData()
	{
		reset();
	}

	void reset()
	{
		m_total = 0.0;
	}

	void start()
	{
		m_time.setStartTick();
	}

	void end()
	{
		m_total += m_time.time_s();
	}

	double getTime()
	{
		return m_total;
	}

private:
	osg::Timer m_time;
	double m_total;
};

#endif // __SULPROFILERDATA_H__