// SulGuiListView.h

#ifndef __SULGUILISTVIEW_H__
#define __SULGUILISTVIEW_H__

#include "SulGuiTable.h"
#include "SulExport.h"
#include "SulString.h"

class SUL_EXPORT CSulGuiListView : public CSulGuiTable
{
public:
	typedef std::vector<CSulString::VEC_STRING>	VEC_ROW;

public:
										CSulGuiListView();

	void								setupAttr( CSulXmlAttr* pAttr );
	void								init();

	sigma::uint32						addRow();
	void								set( sigma::uint32 row, sigma::uint32 col, const CSulString& s ); 

	virtual class CSulGuiListView*		asListView()	{ return this; }

private:
	CSulString							getText( sigma::uint32 row, sigma::uint32 col );

	void								updateView();

private:
	VEC_ROW								m_vecRow;
	CSulString							m_header;
};

#endif // __SULGUILISTVIEW_H__