// SulQtPropVec3.h

#ifndef __SULQTPROPVEC3_H__
#define __SULQTPROPVEC3_H__

#include "SulQtPropBase.h"

class CSulQtPropVec3 : public CSulQtPropBase
{
public:
	CSulQtPropVec3( const CSulString& label, osg::Vec3& value ) :
		CSulQtPropBase( label, m_lineEdit = new QLineEdit( CSulString(value).c_str() ) ),
		m_value( value )
		{
		}

	osg::Vec3 getValue()
	{
		QString t = m_lineEdit->text();
		std::string s = t.toUtf8().constData();
		return CSulString(s).asVec3();
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
	osg::Vec3& m_value;
};

#endif // __SULQTPROPVEC3_H__