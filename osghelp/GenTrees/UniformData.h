// UniformData.h

#ifndef __UNIFORMDATA_H__
#define __UNIFORMDATA_H__

#include <osg/referenced>
#include <SigmaUtil/SulString.h>

class CUniformData : public osg::Referenced
{
public:
	CUniformData( const CSulString& sName, const CSulString& sValue, const CSulString& sType );

	osg::Uniform* createUniform();

private:
	CSulString m_sName;
	CSulString m_sValue;
	CSulString m_sType;
};

#endif // __UNIFORMDATA_H__