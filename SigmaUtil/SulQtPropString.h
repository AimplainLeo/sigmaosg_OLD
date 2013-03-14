// SulQtPropString.h

#ifndef __SULQTPROPSTRING_H__
#define __SULQTPROPSTRING_H__

#include "SulQtPropBase.h"
#include <QtGui/qlineedit>

class CSulQtPropString : public CSulQtPropBase
{
public:
	CSulQtPropString( const CSulString& label, sigma::uint32 value=0 ) :
	  CSulQtPropBase( label, m_lineEdit = new QLineEdit( CSulString(value).c_str() ) )
	{
	}

	CSulQtPropString( const CSulString& label, const CSulString& text ) :
		CSulQtPropBase( label, m_lineEdit = new QLineEdit( text.c_str() ) )
		{
		}

	CSulString getString()
	{
		QString t = m_lineEdit->text();
		std::string s = t.toUtf8().constData();
		return s;
	}

	/*
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
	*/

	osg::Vec3 getVec3()
	{
		QString t = m_lineEdit->text();
		std::string s = t.toUtf8().constData();
		return CSulString(s).asVec3();
	}
	/*
	osg::Vec4 getVec4()
	{
		QString t = m_lineEdit->text();
		std::string s = t.toUtf8().constData();
		return CSulString(s).asVec4();
	}
	*/
	osg::Vec4s getVec4s()
	{
		QString t = m_lineEdit->text();
		std::string s = t.toUtf8().constData();
		return CSulString(s).asVec4s();
	}

private:
	QLineEdit* m_lineEdit;
};

#endif // __SULQTPROPSTRING_H__