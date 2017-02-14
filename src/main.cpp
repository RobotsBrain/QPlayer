#include <QApplication>

#include "proto.hpp"
#include "mainwindow.hpp"


// extern QApplication* app_;


int main(int argc, char *argv[])
{	
	vd::Logger logger;
	vd::Logger::i().wl();

	// vd::MediaDecoder md;
	QApplication app(argc, argv);
	// app_ = &app;
	vd::MainWindow w;
	w.show();

	return app.exec();
}


