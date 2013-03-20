// SulQtPropVec4s.h

#ifndef __SULQTPROPVEC4S_H__
#define __SULQTPROPVEC4S_H__

#include "SulQtPropBase.h"
#include "SulString.h"

class CSulQtPropVec4s : public CSulQtPropBase
{
public:
	CSulQtPropVec4s( const CSulString& label, osg::Vec4s& value ) :
		CSulQtPropBase( label, m_lineEdit = new QLineEdit( CSulString(value).c_str() ) ),
		m_value( value )
		{
		}

	osg::Vec4s getValue()
	{
		QString t = m_lineEdit->text();
		std::string s = t.toUtf8().constData();
		return CSulString(s).asVec4s();
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
	osg::Vec4s& m_value;
};

#endif // __SULQTPROPVEC4S_H__