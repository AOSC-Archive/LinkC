/*#include "../include/linkc_def.h"
#include "../include/linkc_error.h"
#include "../include/linkc_client.h"
#include "../include/linkc_client_nonui.h"
#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <QTextCodec>
#include <QApplication>
#include "mainwindow.h"

int main(int argc,char **argv){
    if(argc == 2){
        setlocale(LC_ALL,"");
        if((strcmp(argv[1],"-NonUi")) == 0){
            NonUiMode();
            endwin();
            exit(0);
        }
    }else{

*/
#include <QApplication>
#include <QTextCodec>
#include "linkc_def.h"
#include "mainwindow.h"

int main(int argc,char **argv){
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
        w.show();
        return a.exec();
}
