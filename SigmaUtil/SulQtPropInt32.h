// SulQtPropInt32.h

#ifndef __SULQTPROPINT32_H__
#define __SULQTPROPINT32_H__

#include "SulQtPropBase.h"
#include <QtGui/qlineedit>

class CSulQtPropInt32 : public CSulQtPropBase
{
public:
	CSulQtPropInt32( const CSulString& label, sigma::int32& value ) :
		CSulQtPropBase( label, m_lineEdit = new QLineEdit( CSulString(value).c_str() ) ),
		m_value( value )
		{
		}

	float getValue()
	{
		QString t = m_lineEdit->text();
		std::string s = t.toUtf8().constData();
		return CSulString(s).asInt32();
	}

	virtual void updateFromUI()
	{
		m_value = getValue();
	}

private:
	QLineEdit*		m_lineEdit;
	sigma::int32&	m_value;
};

#endif // __SULQTPROPINT32_H__