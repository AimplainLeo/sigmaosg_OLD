// SulDebugValueFloat.h

#ifndef __SULDEBUGVALUEFLOAT_H__
#define __SULDEBUGVALUEFLOAT_H__

#include "SulDebugValueBase.h"
#include "SulExport.h"

class SUL_EXPORT CSulDebugValueFloat : public CSulDebugValueBase
{
public:
	CSulDebugValueFloat( const std::string& sTitle, float fMin, float fMax, float fVal, float fStep );

	void inc();
	void dec();

	float getVal();

private:
	float		m_val;
	float		m_minVal;
	float		m_maxVal;
	float		m_stepVal;
};

#endif // __SULDEBUGVALUEFLOAT_H__