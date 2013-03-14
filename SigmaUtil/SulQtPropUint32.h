// SulQtPropUint32.h

#ifndef __SULQTPROPUINT32_H__
#define __SULQTPROPUINT32_H__

#include "SulQtPropBase.h"
#include <QtGui/qlineedit>

class CSulQtPropUint32 : public CSulQtPropBase
{
public:
	CSulQtPropUint32( const CSulString& label, sigma::uint32& value ) :
		CSulQtPropBase( label, m_lineEdit = new QLineEdit( CSulString(value).c_str() ) ),
		m_value( value )
		{
		}

	float getValue()
	{
		QString t = m_lineEdit->text();
		std::string s = t.toUtf8().constData();
		return CSulString(s).asUint32();
	}

	virtual void updateFromUI()
	{
		m_value = getValue();
	}

private:
	QLineEdit*		m_lineEdit;
	sigma::uint32&	m_value;
};


#endif // __SULQTPROPUINT32_H__