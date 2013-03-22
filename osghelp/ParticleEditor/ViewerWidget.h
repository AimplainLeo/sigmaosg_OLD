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

	void place( const osg::Vec3& pos );
	void create();

private:
	osg::Node* createScene();

	void createLayout();
	void createViews();

protected:    
    QTimer _timer;

private:
	osg::Group*				m_group;
	CSulParticleSystemOsg*	m_particleSystem;

	QWidget* m_wView0;
	QWidget* m_wView1;
	QWidget* m_wView2;
	QWidget* m_widgetToolbar;

	QListWidget*	m_listParticleSystem;
	QLineEdit*		m_particleSystemName;

	CSulParticleSystemContainerOsg* m_psContainer;

	typedef std::vector< osg::ref_ptr<CSulParticleSystemContainerOsg> >	VEC_PARTICLESYSTEMCONTAINEROSG;
	VEC_PARTICLESYSTEMCONTAINEROSG						m_vecParticleSystemContainer;
};


#endif // __VIEWERWIDGET_H__