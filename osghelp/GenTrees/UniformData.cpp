// UniformData.cpp

#include "stdafx.h"
#include "UniformData.h"

CUniformData::CUniformData( const CSulString& sName, const CSulString& sValue, const CSulString& sType )
{
	m_sName = sName;
	m_sValue = sValue;
	m_sType = sType;
}

osg::Uniform* CUniformData::createUniform()
{
	if ( m_sType=="BOOL" )
	{
		return new osg::Uniform( m_sName.c_str(), m_sValue.asBool() );
	}

	if ( m_sType=="INT" )
	{
		return new osg::Uniform( m_sName.c_str(), m_sValue.asInt32() );
	}

	return 0;
}