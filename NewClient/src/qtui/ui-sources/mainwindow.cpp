#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWIndow::MainWIndow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWIndow)
{
    ui->setupUi(this);
}

MainWIndow::~MainWIndow()
{
    delete ui;
}
