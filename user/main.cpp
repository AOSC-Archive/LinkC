/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： March 22, 2014
 */
#include "Window/MainWindow.h"
#include <QApplication>
#include <QPushButton>

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
