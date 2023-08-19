#include "mainwidget.h"
#include "ui_mainwidget.h"
mainWidget::mainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainWidget)
{
}

mainWidget::mainWidget(user_data* userdata,QString account):ui(new Ui::mainWidget){

    ui->setupUi(this);this->setAttribute(Qt::WA_NoChildEventsForParent);
    //去掉边框
    this->setWindowFlags(Qt::WindowType::FramelessWindowHint);

    this->setWindowIcon(QIcon(":/res/title_Icon.jpg"));
    //背景透明
    this->setAttribute(Qt::WA_TranslucentBackground);

    //设置QTabWidget的内部空间背景变为白色（QTabWidget::pane:!selected 无法使用，只能通过每次切换来重新设置样式表）
    ui->widget_7->setStyleSheet("border: none;background-color: white;");
    ui->toolBox_Person->setStyleSheet("border: none;background-color: white;");
    ui->toolBox->setStyleSheet("border: none; background-color: white;");
    connect(ui->tabWidget, &QTabWidget::currentChanged, this,[=](int index){
        if(index == 0){
            ui->widget_7->setStyleSheet("border: none;background-color: white;");
        }else if(index == 1){
            ui->toolBox_Person->setStyleSheet("border: none;background-color: white;");
        }else if(index == 2){
            ui->toolBox->setStyleSheet("border: none; background-color: white;");
        }
    });

    ui->label_top->lower();

    //为头像控件安装事件过滤器，以达到改变头像的需求
    ui->label_head->setMask(QRegion(ui->label_head->rect(),QRegion::RegionType::Ellipse));
    ui->label_head->installEventFilter(this);
    ui->tabWidget->setContentsMargins(0, 0, 0, 0);

    //消息界面的布局
    layout = new QVBoxLayout(ui->widget_7);
        layout->setAlignment(Qt::AlignTop);
        layout->setSpacing(1);
    ui->widget_7->setLayout(layout);

    //设置用户名
    ui->username->setText(userdata->get_username(account));

    //设置个性签名
    ui->lineEdit_bardian->setText(userdata->get_signature(account));
    connect(ui->lineEdit_bardian,&QLineEdit::textChanged,this,[=](const QString& text){
        userdata->save_signature(account,text);
    });

    //设置头像
    QString pixmap = userdata->get_headPath(account);
    QFile file(pixmap);
    if(pixmap == ""|| !file.exists()){
        pixmap = ":/res/head.png";
    }
    ui->label_head->setPixmap(QPixmap(pixmap));
    connect(this, &mainWidget::changedHead,this, [=]() {
        qDebug() << "label_head 被点击了";
        QString currentPath = QCoreApplication::applicationDirPath();
        QString filePath = QFileDialog::getOpenFileName(nullptr,"选择文件",currentPath,"All Files (*.png)");
        if(filePath != ""){
            userdata->save_headPath(account,filePath);
            ui->label_head->setPixmap(QPixmap(userdata->get_headPath(account)));
        }

    });

    //设置服务器连接状态(connectToHost可能是异步的，所以不能第一时间获取state，而是等待它发送连接成功/失败信号)
    //有点问题，连接服务器过程中，state也会被改为true
    //使用定时器来循环监听tcp连接的状态
    tcp_socket = new Tcp();
    tcp_socket->tcp_connect(account);
    QTimer *timer = new QTimer(this);
    timer->setInterval(20000);
    connect(timer,&QTimer::timeout,this,[=](){
        qDebug()<<"-------"<<tcp_socket->state<<"-----";
        qDebug()<<"定时器触发";
        if(tcp_socket->state == false){
            tcp_socket->tcp_connect(account);
        }
    });
    timer->start();
    ui->link_state->setScaledContents(true);
    connect(tcp_socket,&Tcp::link_changed,this,[=](){
        if(tcp_socket->state){
            ui->link_state->setPixmap(QPixmap(":/res/online.png"));
            //ui->label_head->setPixmap(QPixmap(":/res/head.png"));
            userdata->updata_user(account,"在线");
        }else{
            ui->link_state->setPixmap(QPixmap(":/res/offline.png"));
            //ui->label_head->setPixmap(QPixmap(":/res/head_1.png"));
            userdata->updata_user(account,"离线");
        }
    });

    //初始化udp连接
    udp_socket = new udp(account);
    //当接收到消息时，所有控件查询数据库，更新最后一条聊天记录
    connect(udp_socket,&udp::rece_data,this,[=](){
        emit update_chatLastHistory();
    });
    connect(udp_socket,&udp::send_data,this,[=](){
        emit update_chatLastHistory();
    });

    //最小化按钮功能
    connect(ui->btn_min,&QPushButton::clicked,this,[=](){
        this->showMinimized();
    });

    //关闭按钮功能
    connect(ui->btn_close,&QPushButton::clicked,this,[=](){
        tcp_socket->tcp_disconnect();
        this->close();

    });

    //更多按钮功能
    QMenu *btn_menu = new QMenu(this);
    QAction *btn_action1 = new QAction("登录其他账号",this);
        btn_action1->setIcon(QIcon(":/res/login_account.png"));
    QAction *btn_action2 = new QAction("注销当前账号",this);
        btn_action2->setIcon(QIcon(":/res/cancle_out.png"));
    btn_menu->addAction(btn_action1);
    btn_menu->addAction(btn_action2);
    //取消菜单的下拉框(黑色小三角)
    ui->btn_more->setStyleSheet("QPushButton::menu-indicator{image:none;}");
    connect(btn_action1,&QAction::triggered,this,[=](){
        userdata->updata_user(account,"离线");
        //tcp_socket->disconnect();
        emit back_widget();
        this->close();
        //停止计时器，不能使用deleteLater()来释放所有资源，程序会崩溃，原因未知
        timer->stop();
        this->deleteLater();
    });
    connect(btn_action2,&QAction::triggered,this,[=](){
        QMessageBox::StandardButton ret_button = QMessageBox::information(this,"注销","你确定要注销账号吗？",QMessageBox::Yes|QMessageBox::Cancel,QMessageBox::Cancel);
        if(ret_button == QMessageBox::Yes){
            userdata->delete_user(account);
            emit deleteUser();
            this->close();
        }
    });
    ui->btn_more->setMenu(btn_menu);

    //初始化已有联系人
    init_friend(userdata,account);
    //添加联系人功能
    connect(ui->btn_add,&QPushButton::clicked,this,[=](){
        if(!this->add_friend(userdata,account,ui->lineEdit_add->text(),-1)){
            QMessageBox::information(this,"失败","联系人已存在或其他错误",QMessageBox::Ok);
        }
    });

    //初始化已有聊天记录
    init_chat(userdata,account);
}

//实现添加联系人功能
bool mainWidget::add_friend(user_data *userdata,QString account , QString friend_account,int currentIndex){
    if(!userdata->find_account(friend_account)||account==friend_account){
        qDebug()<<"联系人不存在";
        return false;
    }
    if(!(userdata->find_friend_account(account,friend_account).isEmpty())&&currentIndex == -1){
        qDebug()<<"联系人已添加";
        return false;
    }
    //获取当前页面索引(如果currentIndex==-1) 用于初始化与添加联系人
    if(currentIndex == -1){
        currentIndex = ui->toolBox_Person->currentIndex();
        //保存联系人信息
        userdata->save_friend(account,friend_account,ui->toolBox_Person->itemText(currentIndex));
        userdata->save_friend(friend_account,account,ui->toolBox_Person->itemText(currentIndex));
    }
    // 获取当前选中的页面部件
    QWidget *currentPage = ui->toolBox_Person->widget(currentIndex);
    //currentPage->setStyleSheet("background-color: transparent;");
    // 获取当前选中页面的布局
    QVBoxLayout *currentLayout = qobject_cast<QVBoxLayout*>(currentPage->layout());
    //设置控件之间的间距
    currentLayout->setSpacing(1);

    //检查头像路径是否正确
    QString pixmap = userdata->get_headPath(friend_account);
    QFile file(pixmap);
    if(pixmap == "" || !file.exists()){
        pixmap = ":/res/head.png";
    }
    friend_button *button = new friend_button(pixmap,userdata->get_username(friend_account),userdata->get_signature(friend_account));

    // 在当前页面插入新部件
    currentLayout->insertWidget(0, button,0,Qt::AlignLeft);


    //删除联系人
    connect(button,&friend_button::delete_friend,this,[=](){
        userdata->delete_friend(account,friend_account);
        userdata->delete_friend(friend_account,account);
        delete button;
    });
    //添加到会话列表
    connect(button,&friend_button::add_chat,this,[=](){
        if(userdata->save_chat(account,friend_account)){
            add_chat(userdata,account,friend_account);
        }
    });
    //设置联系人按钮双击事件
    connect(button,&friend_button::mouseDoubleDlick,this,[=](){
        chatbox *chat = new chatbox(userdata,udp_socket,account,friend_account);
        chat->show();
    });
    //设置联系人按钮的单击击事件
    connect(button,&friend_button::mousePress,this,[=](){
        //qDebug()<<"按钮单击了";
    });
    return true;
}

//实现初始化已有联系人
bool mainWidget::init_friend(user_data *userdata,QString account){
    //qDebug()<<"调用联系人初始化函数";
    QList<QPair<QString,QString>> L_friend1 = userdata->get_friend(account,"好友");
    QList<QPair<QString,QString>> L_friend2 = userdata->get_friend(account,"朋友");
    QList<QPair<QString,QString>> L_friend3 = userdata->get_friend(account,"家人");
    for(auto l:L_friend1){
        //qDebug()<<"username:"<<l.second<<"account"<<l.first;
        add_friend(userdata,account,l.first,0);
    }
    for(auto l:L_friend2){
        add_friend(userdata,account,l.first,1);
    }
    for(auto l:L_friend3){
        add_friend(userdata,account,l.first,2);
    }
    return true;
}

//添加到会话列表
bool mainWidget::add_chat(user_data *userdata,QString account , QString friend_account){
    QString pixmap = userdata->get_headPath(friend_account);
    if(pixmap == ""){
        pixmap = ":/res/head.png";
    }
    chat_button *button = new chat_button(pixmap,userdata->get_username(friend_account),userdata->get_lastHistory(account,friend_account));
    layout->addWidget(button);
    //聊天按钮单击
    connect(button,&chat_button::mousePress,this,[=](){

    });
    //聊天按钮双击
    connect(button,&chat_button::mouseDoubleDlick,this,[=](){
        chatbox *chat = new chatbox(userdata,udp_socket,account,friend_account);
        chat->show();
    });
    //从会话列表移除
    connect(button,&chat_button::delete_chat,this,[=](){
        userdata->delete_chat(account,friend_account);
        delete button;
    });
    //删除所有聊天记录
    connect(button,&chat_button::delete_chatHistory,this,[=](){
        userdata->delete_chatHistory(account,friend_account);
        delete button;
    });

    //更新最后一条聊天记录
    connect(this,&mainWidget::update_chatLastHistory,this,[=](){
        button->update_chatHistory(userdata->get_lastHistory(account,friend_account));
    });
}

//初始化已有聊天
bool mainWidget::init_chat(user_data* userdata,QString account){
    QList<QPair<QString,QString>> L = userdata->get_chat(account);
    for(auto s:L){
        add_chat(userdata,s.first,s.second);
    }
    return true;
}

//实现窗口拖动
void mainWidget::mousePressEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->pos();
        m_isDragging = true;
    }
    QWidget::mousePressEvent(event);
}

void mainWidget::mouseMoveEvent(QMouseEvent* event){
    if (m_isDragging) {
        QPoint diff = event->pos() - m_dragStartPosition;
        QPoint newTopLeft = this->geometry().topLeft() + diff;
        move(newTopLeft);
    }
    QWidget::mouseMoveEvent(event);
}

void mainWidget::mouseReleaseEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
    }
    QWidget::mouseReleaseEvent(event);
}

//实现边框阴影
void mainWidget::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    QPixmap pixmap(":/res/background.png");
    qDrawBorderPixmap(&painter, this->rect(), QMargins(0, 0, 0, 0), pixmap);
    QRect rect(this->rect().x()+8, this->rect().y()+12, this->rect().width()-16, this->rect().height()-30);
    painter.fillRect(rect, QColor(255, 255, 255));
}

bool mainWidget::eventFilter(QObject * watch,QEvent*event){
    if(watch == ui->label_head && event->type() == QEvent::MouseButtonPress){
        QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            //qDebug() << "label_head 被点击了";
            emit changedHead();
            return true;
        }
    }
    return QWidget::eventFilter(watch,event);
}

mainWidget::~mainWidget()
{
    delete tcp_socket;
    delete udp_socket;
    delete ui;
}

