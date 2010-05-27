// bloomppu.h

#ifndef __BLOOMPPU_H__
#define __BLOOMPPU_H__

#include <osgPPU/Processor.h>
#include <osgPPU/UnitBypass.h>

class CBloomPPU
{
public:
	bool			Create();

	osgPPU::Unit*	GetFirstUnit();
	osgPPU::Unit*	GetLastUnit();

private:
	osg::ref_ptr<osgPPU::UnitBypass>	m_rFirstUnit;
};

#endif // __BLOOMPPU_H__