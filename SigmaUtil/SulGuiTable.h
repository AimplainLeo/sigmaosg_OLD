// SulGuiTable.h

#ifndef __SULGUITABLE_H__
#define __SULGUITABLE_H__

#include "SulGuiCanvas.h"
#include "SulExport.h"

class SUL_EXPORT CSulGuiTable : public CSulGuiCanvas
{
public:
									CSulGuiTable( const CSulString& sCompName );
									CSulGuiTable();

	void							initConstructor();

	virtual void					setupAttr( CSulXmlAttr* pAttr );

	void							init();

	void							setText( sigma::uint32 row, sigma::uint32 col, const CSulString& text );

	sigma::uint32					getRowCount();
	sigma::uint32					getColCount();

	virtual class CSulGuiTable*		asTable()		{ return this; }

private:
	sigma::uint32					m_rows;
	sigma::uint32					m_cols;
	sigma::uint32					m_padding;
	osg::ref_ptr<osg::Group>		m_rCellGroup;
};

#endif // __SULGUITABLE_H__