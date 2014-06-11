/*
 * Author		： Junfeng Zhang <564691478@qq.com>
 * Last-Change		： Aprli 2, 2014
 */
#include "Window/MainWindow.h"
#include "Protocol/LinkC_NetWorkProtocol.h"
#include <QApplication>
#include <QPushButton>
#include <QTextCodec>

int main(int argc, char *argv[])
{
#if QT_VERSION < 0x050000
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
#else
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
#endif
    QApplication a(argc, argv);
    MainWindow w;
    return a.exec();
}
