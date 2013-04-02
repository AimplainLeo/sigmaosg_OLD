// ParticleEditor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ViewerWidget.h"
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

int _tmain(int argc, _TCHAR* argv[])
{
	// Qt requires that we construct the global QApplication before creating any widgets.
    QApplication app(argc, (char**)argv);

	// this improves the performance for particle creation 
    osgDB::ReaderWriter::Options* options = new osgDB::ReaderWriter::Options;
    options->setObjectCacheHint(osgDB::ReaderWriter::Options::CACHE_IMAGES);
    osgDB::Registry::instance()->setOptions(options);

    CViewerWidget* viewWidget = new CViewerWidget;
    viewWidget->setGeometry( 100, 100, 1600, 800 );
    viewWidget->show();

	app.exec();

	return 0;
}

