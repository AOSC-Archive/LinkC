#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

namespace Ui {
class MainWIndow;
}

class MainWIndow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWIndow(QWidget *parent = 0);
    ~MainWIndow();

private:
    Ui::MainWIndow *ui;
};

#endif // MAINWINDOW_H
