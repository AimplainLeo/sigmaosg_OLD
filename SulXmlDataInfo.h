// SulXmlDataInfo.h

#ifndef __SULXMLDATAINFO_H__
#define __SULXMLDATAINFO_H__

#include "SulExport.h"
#include "SulXmlAttr.h"
#include <osg/Referenced>
#include <osg/ref_ptr>

class SUL_EXPORT CSulXmlDataInfo : public osg::Referenced
{
public:
	void						setAttr( CSulXmlAttr* pAttr );
	CSulXmlAttr*				getAttr();

	void						setName( const char* pName );
	const CSulString&			getName() const;

private:
	CSulString					m_strName;
	osg::ref_ptr<CSulXmlAttr>	m_rAttr;
};

#endif // __SULXMLDATAINFO_H__