// SulQtPropBool.h

#ifndef __SULQTPROPBOOL_H__
#define __SULQTPROPBOOL_H__

#include "SulQtPropBase.h"
#include <QtGui/qcheckbox>

class CSulQtPropBool : public CSulQtPropBase
{
public:
	CSulQtPropBool( const CSulString& label, bool& b ) :
		CSulQtPropBase( label, m_checkbox = new QCheckBox ),
		m_value( b )
		{
			m_checkbox->setChecked( b );
		}

	bool getState()
	{
		return m_checkbox->checkState();
	}

	virtual void updateFromUI()
	{
		m_value = getState();
	}

	virtual void updateToUI()
	{
		m_checkbox->setChecked( m_value );
	}

private:
	QCheckBox*	m_checkbox;
	bool&		m_value;
};

#endif // __SULQTPROPBOOL_H__