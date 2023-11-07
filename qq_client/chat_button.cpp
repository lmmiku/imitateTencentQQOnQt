#include "chat_button.h"
#include "ui_chat_button.h"

chat_button::chat_button(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chat_button)
{
    ui->setupUi(this);
}

chat_button::chat_button(QString Icon, QString username, QString chat):ui(new Ui::chat_button) {
    ui->setupUi(this);

    ui->button_Icon->setScaledContents(true);
    //ui->button->setStyleSheet("text-align: left;border: 1px solid lightgray;border-radius: 10px;");

    ui->button_Icon->setPixmap(QPixmap(Icon));
    QBitmap mask(ui->button_Icon->size());
    mask.fill(Qt::color0);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing, true);  // 启用抗锯齿
    painter.setBrush(Qt::color1);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(mask.rect());
    painter.end();
    ui->button_Icon->setMask(mask);

    ui->button_username->setText(username);
    ui->button_chat->setText(chat);
    ui->button_chat->setStyleSheet("color: rgb(127, 127, 127);");

    menu = new QMenu(this);
    QAction *action_1 = new QAction("从会话列表移除");
    action_1->setIcon(QIcon(":/res/delete_chat.png"));
    QAction *action_2 = new QAction("删除所有聊天记录");
    action_2->setIcon(QIcon(":/res/delete_chatHistory.png"));
    connect(action_1,&QAction::triggered,this,[=](){
        emit delete_chat();
    });
    connect(action_2,&QAction::triggered,this,[=](){
        emit delete_chatHistory();
    });
    //设置菜单颜色(莫名其妙默认颜色变为黑色了)
    menu->setStyleSheet("QMenu { background-color: white; color: black; }");
    menu->addAction(action_1);
    menu->addAction(action_2);
}

void chat_button::update_chatHistory(QString last_message){
    ui->button_chat->setText(last_message);
}

void chat_button::mouseDoubleClickEvent(QMouseEvent *event){
    emit mouseDoubleDlick();
}

void chat_button::mousePressEvent(QMouseEvent*event){
    if(event->button() == Qt::RightButton){
        menu->exec(event->globalPos());
    }else{
        this->setStyleSheet("border:none;background-color:rgb(245,245,245);border-radius: 10px;");
        emit mousePress();
    }
}

void chat_button::enterEvent(QEvent *event) {
    // 鼠标进入子控件范围时，将背景颜色设置为灰色
    this->setStyleSheet("border:none;background-color:rgb(235,235,235);border-radius: 10px;");
}

void chat_button::leaveEvent(QEvent *event) {
    // 鼠标离开子控件范围时，将背景颜色设置为白色
    this->setStyleSheet("border:none;background-color:white;border-radius: 10px;");
}



chat_button::~chat_button()
{
    delete ui;
}
