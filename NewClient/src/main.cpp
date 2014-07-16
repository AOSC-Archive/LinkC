#include <QApplication>
#include <QTextCodec>
#include <QVariant>
#include "linkc_def.h"
#include "linkc_client.h"
#include "mainwindow.h"
#include "loginwindow.h"
#include "linkc_netowrk_UDP_system.h"

int main(int argc,char **argv){
#if QT_VERSION < 0x050000
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForTr(codec);
        QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
        QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
#else
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
#endif
//注册数据类型
        QVariant DataVar;
        LoginData   D1;
        UserData    D2;
        DataVar.setValue(D1);
        qRegisterMetaType<LoginData>("LoginData");
        DataVar.setValue(D2);
        qRegisterMetaType<UserData>("UserData");
//注册完毕
        InitLCUDPEnvironment();
        QApplication a(argc, argv);
        MainWindow w;
        w.hide();
        return a.exec();
}
