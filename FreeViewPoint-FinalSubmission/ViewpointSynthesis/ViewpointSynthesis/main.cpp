#include "ViewpointSynthesis.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ViewpointSynthesis w;
	w.show();
	return a.exec();
}
