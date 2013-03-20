// SulXmlNodeTag.h

#ifndef __SULXMLNODETAG_H__
#define __SULXMLNODETAG_H__

#include "SulNodeGroup.h"
#include "SulXmlAttr.h"

class CSulXmlNodeTag : public CSulNodeGroup
{
public:
	CSulXmlNodeTag( const CSulString& name );

	const CSulString& getName();

	void setAttr( const CSulXmlAttr& attr );

	float		getAttrAsFloat( const CSulString& attrName, float defaultValue );
	osg::Vec3	getAttrAsVec3( const CSulString& attrName, osg::Vec3 defaultValue );
	osg::Vec4	getAttrAsVec4( const CSulString& attrName, osg::Vec4 defaultValue );
	CSulString	getAttrAsString( const CSulString& attrName, CSulString defaultValue );
	bool		getAttrAsBool( const CSulString& attrName, bool defaultValue );

private:
	CSulString	m_tagName;
	CSulXmlAttr	m_attr;
};

#endif // __SULXMLNODETAG_H__