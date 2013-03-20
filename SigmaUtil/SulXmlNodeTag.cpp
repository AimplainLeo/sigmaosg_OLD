// SulXmlNodeTag.cpp

#include "stdafx.h"
#include "SulXmlNodeTag.h"


CSulXmlNodeTag::CSulXmlNodeTag( const CSulString& name ) :
CSulNodeGroup()
{
	m_tagName = name;
}

const CSulString& CSulXmlNodeTag::getName()
{
	return m_tagName;
}

void CSulXmlNodeTag::setAttr( const CSulXmlAttr& attr )
{
	m_attr = attr;
}

float CSulXmlNodeTag::getAttrAsFloat( const CSulString& attrName, float defaultValue )
{
	return m_attr.getFloat( attrName, defaultValue );
}

osg::Vec3 CSulXmlNodeTag::getAttrAsVec3( const CSulString& attrName, osg::Vec3 defaultValue )
{
	return m_attr.getVec3( attrName, defaultValue );
}

osg::Vec4 CSulXmlNodeTag::getAttrAsVec4( const CSulString& attrName, osg::Vec4 defaultValue )
{
	return m_attr.getVec4( attrName, defaultValue );
}

CSulString CSulXmlNodeTag::getAttrAsString( const CSulString& attrName, CSulString defaultValue )
{
	return m_attr.getString( attrName, defaultValue );
}

bool CSulXmlNodeTag::getAttrAsBool( const CSulString& attrName, bool defaultValue )
{
	return m_attr.getBool( attrName, defaultValue );
}