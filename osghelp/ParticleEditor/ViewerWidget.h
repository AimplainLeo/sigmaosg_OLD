// ViewerWidget.h

#ifndef __VIEWERWIDGET_H__
#define __VIEWERWIDGET_H__

#include <SigmaUtil/SulParticleSystemOsg.h>
#include <osgViewer/CompositeViewer>
#include <QtCore/QTimer>
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtGui/QtEvents>
#include <QtGui/QMessageBox>
#include <QtGui/QGridLayout>
#include <QtGui/qsplitter>
#include <QtGui/QScrollArea>
#include <QtCore\qobject>
#include <QtGui\qpushbutton>
#include <QtGui\QListWidget>
#include <QtGui\QListWidgetItem>

class CViewerWidget : public QWidget, public osgViewer::CompositeViewer
{
	Q_OBJECT

private slots:
	void addParticleSystem();
	void removeParticleSystem();
	void clickedParticleSystem( QListWidgetItem * item );
	void load();
	void save();

public:
    CViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel=osgViewer::CompositeViewer::SingleThreaded);
    
    QWidget* addViewWidget( osg::Camera* camera, osg::Node* scene );
	osg::Camera* createCamera( int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false );
    virtual void paintEvent( QPaintEvent* event );

	void create( const osg::Vec3& pos );

private:
	CSulParticleSystemDataOsg* createParticleSystemData( const CSulString& title, bool bEnabled=true );

	osg::Node* createScene();

	void createLayout();
	void createViews();

	bool load( const CSulString& file );
	bool save( const CSulString& file );

protected:    
    QTimer _timer;

private:
	osg::Group*				m_group;
	CSulParticleSystemOsg*	m_particleSystem;

	QWidget*				m_wView0;
	QWidget*				m_wView1;
	QWidget*				m_wView2;
	QWidget*				m_widgetToolbar;

	QWidget*				m_widgetPropertySheetContainer;
	QScrollArea*			m_scrollAreaPropertySheet;

	QListWidget*			m_listParticleSystem;
	QLineEdit*				m_particleSystemName;

	CSulParticleSystemDataPropertySheetQt* m_propertySheet;

	CSulParticleSystemDataOsg* m_dataLast;

	QPushButton*			m_multi;
	QLineEdit*				m_wind;
};


#endif // __VIEWERWIDGET_H__