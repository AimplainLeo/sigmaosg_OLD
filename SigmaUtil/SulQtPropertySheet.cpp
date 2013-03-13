// SulQtPropertySheet.cpp

#include "stdafx.h"
#include "SulQtPropertySheet.h"
#include <QtGui/qlabel>

CSulQtPropertySheet::CSulQtPropertySheet( const CSulString& title ) :
QWidget()
{
	setStyleSheet("background-color:red;");

	m_layout = new QVBoxLayout;

	setLayout( m_layout );

	QLabel* label = new QLabel( title.c_str() );
	label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed ); 
	label->setStyleSheet("background-color:green;");
	m_layout->addWidget( label, Qt::AlignTop );
	
	QWidget* w = new QWidget;
	w->setStyleSheet("background-color:gray;");
	w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed ); 
	m_layout->addWidget( w, Qt::AlignTop );
	
	m_grid = new QGridLayout;
	w->setLayout( m_grid );

	m_layout->addStretch(1);

	m_row = 0;
}

void CSulQtPropertySheet::add( CSulQtPropBase* prop )
{
	m_vecProp.push_back( prop );

	m_grid->addWidget( new QLabel( prop->getLabel().c_str() ), m_row, 0 );

	if ( prop->getWidget() )
		m_grid->addWidget( prop->getWidget(), m_row, 1 );

	m_row++;
}

void CSulQtPropertySheet::updateFromUI()
{
	VEC_PROP::iterator i = m_vecProp.begin();
	VEC_PROP::iterator ie = m_vecProp.end();

	while ( i!=ie )
	{
		(*i)->updateFromUI();
		++i;
	}
}