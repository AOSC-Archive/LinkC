#include "MessageBox.h"

MessageBox::MessageBox(QWidget *parent, const QString &mes, int sec):
    QWidget(parent),
    second(sec),
    message(mes)
{
    initUI();
    timerProducer = new QTimeLine(second*1000,this);
    timerProducer->setDirection(QTimeLine::Backward);//使valueChanged(qreal)信号从1开始
    //valueChanged 0~1值改变
    connect(timerProducer,SIGNAL(valueChanged(qreal)),this,SLOT(slt_desalting(qreal)));
    connect(this,SIGNAL(sig_show()),timerProducer,SLOT(start()));
    connect(timerProducer,SIGNAL(finished()),this,SLOT(close()));//QTimeLine结束则关闭窗口
}

void MessageBox::initUI()
{

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Sheet);
    setAttribute(Qt::WA_X11DoNotAcceptFocus, true);
    //FramelessWindowHint 无边框 WindowStaysOnTopHint 顶层窗体 Sheet 像mac的sheet一样
    setStyleSheet("background-color: darkGray");//默认神灰色背景
    setFocusPolicy(Qt::NoFocus);
    lable = new QLabel(message);
    QPalette pal;
    pal.setColor(QPalette::WindowText,Qt::white);//白色字体
    lable->setPalette(pal);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(lable);
    setLayout(mainLayout);
}

void MessageBox::slt_desalting(qreal opacity)
{
    //完全1.0不透明
    this->setWindowOpacity(opacity);


}

void MessageBox::show()
{
    emit sig_show();
    QWidget::show();
}

void MessageBox::closeEvent(QCloseEvent *e)
{
    emit sig_close();
    QWidget::closeEvent(e);
}
/**
 * @brief MessageBox::start 调用
 * @param mes 信息 会html更好，字体可以改变
 * @param sec 显示的时间
 * @param parent 父窗口
 */
void MessageBox::start(QWidget *parent, const QString &mes, int sec)
{
    QEventLoop loop;//局部时间循环
    QScopedPointer<MessageBox> box(new MessageBox(parent, mes, sec));
    QObject::connect(box.data(),SIGNAL(sig_close()),&loop,SLOT(quit()));//窗口关闭推出局部时间循环
    box->show();
    loop.exec();
}


