// SulQtPropBase.h

#ifndef __SULQTPROPBASE_H__
#define __SULQTPROPBASE_H__

#include <QtGui/QWidget>

class CSulQtPropBase
{
public:
	CSulQtPropBase( const CSulString& label, QWidget* widget=0 ) {
		m_label = label;
		m_widget = widget;
	}

	const CSulString& getLabel() {
		return m_label;
	}

	QWidget* getWidget() {
		return m_widget;
	}

	virtual void updateFromUI() {}
	virtual void updateToUI() {}

public:
	CSulString	m_label;
	QWidget*	m_widget;
};

#endif // __SULQTPROPBASE_H__