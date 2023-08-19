#include "enter.h"
#include "ui_enter.h"
#include "mypushbutton.h"

Enter::Enter(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Enter)
{
    ui->setupUi(this);
    this->setFixedSize(380,250);
    QPixmap pixmap(":/res/MenuSceneStartButton.png");
    myPushbutton *btn_head=new myPushbutton(":/res/MenuSceneStartButton.png");
    btn_head->setFixedSize(pixmap.width(),pixmap.height());
    btn_head->setParent(this);
    QLabel *background_label = new QLabel();
    background_label->setScaledContents(true);
    background_label->setPixmap(QPixmap(":/res/backGround.jpg"));
    background_label->setFixedSize(this->size());
}

void Enter::paintEvent(QPaintEvent *event){
}

Enter::~Enter()
{
    delete ui;
}

