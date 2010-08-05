// SulGuiItem.h

#ifndef __SULGUIITEM_H__
#define __SULGUIITEM_H__

#include "SulGuiComp.h"

class CSulGuiItem : public CSulGuiComp
{
public:
	CSulGuiItem( const CSulString& sText );
};

typedef std::vector< osg::ref_ptr<CSulGuiItem> >	VEC_GUIITEM;

#endif // __SULGUIITEM_H__