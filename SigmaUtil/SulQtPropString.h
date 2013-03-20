// SulQtPropString.h

#ifndef __SULQTPROPSTRING_H__
#define __SULQTPROPSTRING_H__

#include "SulQtPropBase.h"
#include <QtGui/qlineedit>

class CSulQtPropString : public CSulQtPropBase
{
public:
	CSulQtPropString( const CSulString& label, CSulString& value ) :
	  CSulQtPropBase( label, m_lineEdit = new QLineEdit( value.c_str() ) ),
		m_value(value)
	{
	}

	CSulString getValue()
	{
		QString t = m_lineEdit->text();
		std::string s = t.toUtf8().constData();
		return s;
	}

	virtual void updateFromUI()
	{
		m_value = getValue();
	}

	virtual void updateToUI()
	{
		m_lineEdit->setText( QString(CSulString(m_value).c_str()) );
	}

private:
	QLineEdit* m_lineEdit;
	CSulString& m_value;
};

#endif // __SULQTPROPSTRING_H__