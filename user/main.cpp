#include "Window/MainWindow.h"
#include <qt4/QtGui/QApplication>
#include <qt4/QtGui/QPushButton>

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QApplication a(argc, argv);
    MainWindow w;
    return a.exec();
}
