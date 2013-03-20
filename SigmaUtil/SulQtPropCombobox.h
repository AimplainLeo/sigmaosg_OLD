// SulQtPropCombobox.h

#ifndef __SULQTPROPCOMBOBOX_H__
#define __SULQTPROPCOMBOBOX_H__

#include "SulQtPropBase.h"
#include "SulStringList.h"
#include <QtGui/qcombobox>

class CSulQtPropCombobox : public CSulQtPropBase
{
public:
	CSulQtPropCombobox( const CSulString& label, CSulString& value, const CSulString& stringList ) :
		CSulQtPropBase( label, m_combobox = new QComboBox ),
		m_value( value )
		{
			CSulStringList list( stringList );
			const CSulStringList::VECTOR_STRING& vec = list.getList();
			CSulStringList::VECTOR_STRING::const_iterator i = vec.begin();
			CSulStringList::VECTOR_STRING::const_iterator ie = vec.end();

			while ( i!=ie )
			{
				m_combobox->addItem( QString((*i).c_str()) );
				++i;
			}
		}


	CSulQtPropCombobox( const CSulString& label, CSulString& value, const CSulStringList& list ) :
		CSulQtPropBase( label, m_combobox = new QComboBox ),
		m_value( value )
		{
			const CSulStringList::VECTOR_STRING& vec = list.getList();
			CSulStringList::VECTOR_STRING::const_iterator i = vec.begin();
			CSulStringList::VECTOR_STRING::const_iterator ie = vec.end();

			while ( i!=ie )
			{
				m_combobox->addItem( QString((*i).c_str()) );
				++i;
			}
		}

	CSulString getValue()
	{
		QString t = m_combobox->currentText();
		std::string s = t.toUtf8().constData();
		return s;
	}

	virtual void updateFromUI()
	{
		m_value = getValue();
	}

	virtual void updateToUI()
	{
		sigma::int32 index = m_combobox->findText( QString(m_value.c_str()) );
		m_combobox->setCurrentIndex( index );
	}

private:
	QComboBox* m_combobox;
	CSulString& m_value;
};


#endif // __SULQTPROPCOMBOBOX_H__