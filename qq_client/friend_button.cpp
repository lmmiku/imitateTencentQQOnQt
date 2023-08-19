#include "friend_button.h"
#include "ui_friend_button.h"

friend_button::friend_button(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::friend_button)
{
    ui->setupUi(this);
}

friend_button::friend_button(QString Icon,QString username,QString signature):ui(new Ui::friend_button){
    //重写构造函数时展示加载UI文件！！！
    ui->setupUi(this);

    ui->button_Icon->setScaledContents(true);
    //ui->button->setStyleSheet("border: 2px solid lightgray;border-radius: 10px;");
    ui->button_Icon->setPixmap(QPixmap(Icon));
    ui->button_Icon->setMask(QRegion(ui->button_Icon->rect(),QRegion::RegionType::Ellipse));
    ui->button_username->setText(username);
    ui->button_signature->setText(signature);
    ui->button_signature->setStyleSheet("color:rgb(127,127,127)");

    menu = new QMenu(this);
    QAction *action_1 = new QAction("删除好友");
    action_1->setIcon(QIcon(":/res/cancle_out.png"));
    connect(action_1,&QAction::triggered,this,[=](){
        emit delete_friend();
    });
    QAction *action_2 = new QAction("添加到会话列表");
    action_2->setIcon(QIcon(":/res/add_chat.png"));
    connect(action_2,&QAction::triggered,this,[=](){
        emit add_chat();
    });
    //设置菜单颜色(莫名其妙默认颜色变为黑色了)
    menu->setStyleSheet("QMenu { background-color: white; color: black; }");
    menu->addAction(action_1);
    menu->addAction(action_2);
    this->installEventFilter(this);
}

void friend_button::updata_Icon(QString Icon){
    ui->button_Icon->setPixmap(QPixmap(Icon));
}

void friend_button::mouseDoubleClickEvent(QMouseEvent *event){
    emit mouseDoubleDlick();
}

void friend_button::mousePressEvent(QMouseEvent*event){
    if(event->button() == Qt::RightButton){
        menu->exec(event->globalPos());
    }else{
        this->setStyleSheet("border:none;background-color:rgb(245,245,245);border-radius: 10px;");
        emit mousePress();
    }
}

void friend_button::enterEvent(QEvent *event) {
    // 鼠标进入子控件范围时，将背景颜色设置为灰色
    //qDebug()<<"进入friendButton的范围";
    this->setStyleSheet("border:none;background-color:rgb(235,235,235);border-radius: 10px;");
}

void friend_button::leaveEvent(QEvent *event) {
    // 鼠标离开子控件范围时，将背景颜色设置为白色
    //qDebug()<<"离开friendButton的范围";
    this->setStyleSheet("border:none;background-color:white;border-radius: 10px;");
}

friend_button::~friend_button()
{
    delete ui;
}
