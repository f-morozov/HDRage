#include "mainwindow.h"
#include <QtGui/QApplication>
#include <cstring>

int main(int argc, char *argv[])
{
	if (argc == 1 || (argc == 2 && !strcmp(argv[1], "-g")))
	{
		QApplication a(argc, argv);
		MainWindow w;
		w.show();
		w.setPath(QString(argv[0]));
		return a.exec();
	}	
}
