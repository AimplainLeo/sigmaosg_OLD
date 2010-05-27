// bloomppu.cpp

#include "stdafx.h"
#include "bloomppu.h"

bool CBloomPPU::Create()
{
	m_rFirstUnit = new osgPPU::UnitBypass();

	return true;
}

osgPPU::Unit* CBloomPPU::GetFirstUnit()
{
	return m_rFirstUnit.get();
}

osgPPU::Unit* CBloomPPU::GetLastUnit()
{
	return 0;
}