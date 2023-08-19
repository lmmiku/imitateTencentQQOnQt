#include "chatbox.h"
#include "ui_chatbox.h"

chatbox::chatbox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chatbox)
{

}

chatbox::chatbox(user_data *userdata,udp* udp_socket,QString account,QString friend_account):ui(new Ui::chatbox){
    ui->setupUi(this);
    //去掉边框
    this->setWindowFlags(Qt::WindowType::FramelessWindowHint);

    //显示所有聊天记录
    view_chatHistory(userdata,account,friend_account);

    ui->widget->setLayout(layout);

    //关闭按钮功能
    connect(ui->btn_close,&QPushButton::clicked,this,[=](){
        this->close();
        //释放当前页面资源
        this->deleteLater();
    });

    //最小化按钮功能
    connect(ui->btn_min,&QPushButton::clicked,[=](){
        this->showMinimized();
    });

    //设置好友名称
    ui->label_friend_username->setText(userdata->get_username(friend_account));

    //发送功能实现
    connect(ui->btn_send,&QPushButton::clicked,[=](){
        QString str = ui->textEdit_2->toPlainText();
        QString send_data ="发送消息："+str;
        //ui->textEdit->append(send_data);
        udp_socket->udp_send(account,friend_account,str);
        ui->textEdit_2->clear();
    });

    //接受消息功能
    connect(udp_socket,&udp::rece_data,this,[=](){
        qDebug()<<"收到消息";
        QString rece_str = udp_socket->udp_rece();
        //ui->textEdit->append("收到消息: "+rece_str);
    });
}

//显示所有聊天记录
void chatbox::view_chatHistory(user_data *userdata,QString account,QString friend_account){
    QList<QPair<QString,QString>> L = userdata->get_ALLChat(account,friend_account);
    layout = new QVBoxLayout(ui->widget);
    for(auto s:L){
        chat_bubble * chat;
        if(s.first == account){
            //ui->textEdit->append("发送消息: "+s.second);
            chat = new chat_bubble(userdata->get_headPath(account),s.second);
            layout->addWidget(chat,0,Qt::AlignRight);
        }else{
            //ui->textEdit->append("收到消息: "+s.second);
            chat = new chat_bubble(userdata->get_headPath(friend_account),s.second);
            layout->addWidget(chat,0,Qt::AlignLeft);
        }
    }
}

//窗口拖动实现
void chatbox::mousePressEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->pos();
        m_isDragging = true;
    }
    QWidget::mousePressEvent(event);
}

void chatbox::mouseMoveEvent(QMouseEvent* event){
    if (m_isDragging) {
        QPoint diff = event->pos() - m_dragStartPosition;
        QPoint newTopLeft = this->geometry().topLeft() + diff;
        move(newTopLeft);
    }
    QWidget::mouseMoveEvent(event);
}

void chatbox::mouseReleaseEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
    }
    QWidget::mouseReleaseEvent(event);
}

//实现边框阴影
void chatbox::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    QPixmap pixmap(":/res/background.png");
    qDrawBorderPixmap(&painter, this->rect(), QMargins(0, 0, 0, 0), pixmap);
    QRect rect(this->rect().x()+8, this->rect().y()+12, this->rect().width()-16, this->rect().height()-30);
    painter.fillRect(rect, QColor(255, 255, 255));
}

chatbox::~chatbox()
{
    delete ui;
}

