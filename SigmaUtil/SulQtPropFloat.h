// SulQtPropFloat.h

#ifndef __SULQTPROPFLOAT_H__
#define __SULQTPROPFLOAT_H__

#include "SulQtPropBase.h"
#include <QtGui/qlineedit>

class CSulQtPropFloat : public CSulQtPropBase
{
public:
	CSulQtPropFloat( const CSulString& label, float& value ) :
		CSulQtPropBase( label, m_lineEdit = new QLineEdit( CSulString(value).c_str() ) ),
		m_value( value )
		{
		}

	float getValue()
	{
		QString t = m_lineEdit->text();
		std::string s = t.toUtf8().constData();
		return CSulString(s).asFloat();
	}

	virtual void updateFromUI()
	{
		m_value = getValue();
	}

private:
	QLineEdit*	m_lineEdit;
	float&		m_value;
};

#endif // __SULQTPROPFLOAT_H__