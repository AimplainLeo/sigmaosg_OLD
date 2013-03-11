// propertySheet.h

#ifndef __PROPERTYSHEET_H__
#define __PROPERTYSHEET_H__

#include <SigmaUtil/SulTypes.h>
#include <SigmaUtil/SulString.h>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtGui/qsplitter>
#include <QtGui/QLineEdit>
#include <QtGui/QMessageBox>

class CPropBase
{
public:
	CPropBase( const CSulString& label, QWidget* widget=0 ) {
		m_label = label;
		m_widget = widget;
	}

	const CSulString& getLabel() {
		return m_label;
	}

	QWidget* getWidget() {
		return m_widget;
	}

public:
	CSulString	m_label;
	QWidget*	m_widget;
};

class CPropString : public CPropBase
{
public:
	CPropString( const CSulString& label, sigma::uint32 value=0 ) :
	  CPropBase( label, m_lineEdit = new QLineEdit( CSulString(value).c_str() ) )
	{
	}

	CPropString( const CSulString& label, const CSulString& text ) :
		CPropBase( label, m_lineEdit = new QLineEdit( text.c_str() ) )
		{
		}

	float getFloat()
	{
		QString t = m_lineEdit->text();
		std::string s = t.toUtf8().constData();
		return CSulString(s).asFloat();
	}

	sigma::uint32 getUint32()
	{
		QString t = m_lineEdit->text();
		std::string s = t.toUtf8().constData();
		return CSulString(s).asUint32();
	}

private:
	QLineEdit* m_lineEdit;
};

class CPropFloat : public CPropString
{
public:
	CPropFloat( const CSulString& label, float value=0.0f ) :
		CPropString( label, CSulString( value ) )
		{

		}
};

class CPropertySheet : public QWidget
{
public:
	CPropertySheet();

	void add( CPropBase* prop );

private:
	QVBoxLayout*	m_layout;
	QGridLayout*	m_grid;
	sigma::uint32	m_row;
};

#endif // __PROPERTYSHEET_H__
