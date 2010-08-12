// SulDebugValueText.h

#ifndef __SULDEBUGVALUETEXT_H__
#define __SULDEBUGVALUETEXT_H__

#include "SulDebugValueBase.h"
#include "SulString.h"
#include "SulStringList.h"
#include "SulExport.h"

class SUL_EXPORT CSulDebugValueText : public CSulDebugValueBase
{
public:
	CSulDebugValueText( const std::string& sTitle, const std::string& sTextArray, const std::string& sTextDefault );

	void inc();
	void dec();

	CSulString getText();

private:
	osg::ref_ptr<CSulStringList>	m_listText;
	sigma::uint32					m_index;
};

#endif // __SULDEBUGVALUETEXT_H__