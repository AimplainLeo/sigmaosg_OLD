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
#include <QtGui/QCheckBox>

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

	CSulString getString()
	{
		QString t = m_lineEdit->text();
		std::string s = t.toUtf8().constData();
		return s;
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

	osg::Vec4 getVec4()
	{
		QString t = m_lineEdit->text();
		std::string s = t.toUtf8().constData();
		return CSulString(s).asVec4();
	}

	osg::Vec4s getVec4s()
	{
		QString t = m_lineEdit->text();
		std::string s = t.toUtf8().constData();
		return CSulString(s).asVec4s();
	}

private:
	QLineEdit* m_lineEdit;
};

class CPropBool : public CPropBase
{
public:
	CPropBool( const CSulString& label, bool b ) :
		CPropBase( label, m_checkbox = new QCheckBox )
		{
			m_checkbox->setChecked( b );
		}

	bool getState()
	{
		return m_checkbox->checkState();
	}

private:
	QCheckBox* m_checkbox;
};

class CPropFloat : public CPropString
{
public:
	CPropFloat( const CSulString& label, float value=0.0f ) :
		CPropString( label, CSulString( value ) )
		{
		}
};

class CPropVec4 : public CPropString
{
public:
	CPropVec4( const CSulString& label, const osg::Vec4& v=osg::Vec4(0,0,0,0) ) :
		CPropString( label, CSulString( v ) )
		{
		}
};

class CPropVec4s : public CPropString
{
public:
	CPropVec4s( const CSulString& label, const osg::Vec4s& v=osg::Vec4s(0,0,0,0) ) :
		CPropString( label, CSulString( v ) )
		{
		}
};

class CPropertySheet : public QWidget
{
public:
	CPropertySheet( const CSulString& title="" );

	void add( CPropBase* prop );

private:
	QVBoxLayout*	m_layout;
	QGridLayout*	m_grid;
	sigma::uint32	m_row;
};

#endif // __PROPERTYSHEET_H__
