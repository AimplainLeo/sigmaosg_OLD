// SulQtPropertySheet.h

#ifndef __SULQTPROPERTYSHEET_H__
#define __SULQTPROPERTYSHEET_H__

#include "SulString.h"
#include "SulQtPropBase.h"
#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>

class CSulQtPropertySheet : public QWidget
{
public:
	CSulQtPropertySheet( const CSulString& title="" );

	void add( CSulQtPropBase* prop );

	void updateFromUI();

private:
	typedef std::vector< CSulQtPropBase* >	VEC_PROP;
	VEC_PROP								m_vecProp;

	QVBoxLayout*	m_layout;
	QGridLayout*	m_grid;
	sigma::uint32	m_row;
};

#endif // __SULQTPROPERTYSHEET_H__