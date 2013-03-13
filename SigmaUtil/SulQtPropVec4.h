// SulQtPropVec4.h

#ifndef __SULQTPROPVEC4_H__
#define __SULQTPROPVEC4_H__

#include "SulQtPropBase.h"

class CSulQtPropVec4 : public CSulQtPropBase
{
public:
	CSulQtPropVec4( const CSulString& label, osg::Vec4& value ) :
		CSulQtPropBase( label, m_lineEdit = new QLineEdit( CSulString(value).c_str() ) ),
		m_value( value )
		{
		}

	osg::Vec4 getValue()
	{
		QString t = m_lineEdit->text();
		std::string s = t.toUtf8().constData();
		return CSulString(s).asVec4();
	}

	virtual void updateFromUI()
	{
		m_value = getValue();
	}

private:
	QLineEdit* m_lineEdit;
	osg::Vec4& m_value;
};

#endif // __SULQTPROPVEC4_H__