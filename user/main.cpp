#include "Window/MainWindow.h"
#include <qt4/Qt/qapplication.h>
#include <qt4/QtGui/QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    return a.exec();
}
