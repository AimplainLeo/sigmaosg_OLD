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

	if ( m_sType=="SAMPLER_2D" )
	{
		osg::Uniform* u = new osg::Uniform( osg::Uniform::SAMPLER_2D, m_sName.c_str() );
		u->set( m_sValue.asInt32() );	
		return u;
	}

	if ( m_sType=="FLOAT" )
	{
		return new osg::Uniform( m_sName.c_str(), m_sValue.asFloat() );
	}

	return 0;
}