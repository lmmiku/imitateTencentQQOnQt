#include "widget.h"
#include "ui_widget.h"
#include "tcp.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //去掉边框
    this->setWindowFlags(Qt::WindowType::FramelessWindowHint);

    ui->label_top->lower();

    //背景透明
    this->setAttribute(Qt::WA_TranslucentBackground);

    updata_user();

    //设置comboBox的默认页面为-1，防止点击默认第一个没有反应
    ui->comboBox_account->setCurrentIndex(-1);

    //安装事件过滤器
    ui->comboBox_account->installEventFilter(this);
    ui->password->installEventFilter(this);
    ui->account->installEventFilter(this);
    ui->btn_passwordview->installEventFilter(this);

    //上层背景动图
    QMovie *movie = new QMovie(this);
    movie->setFileName(":/res/mian.gif");
    movie->start();
    ui->label_top->setMovie(movie);

    //头像改为圆形
    ui->label_head->setMask(QRegion(ui->label_head->rect(),QRegion::RegionType::Ellipse));

    //最小化按钮功能
    connect(ui->btn_min,&QPushButton::clicked,[=](){
        this->showMinimized();
    });

    //关闭按钮功能
    connect(ui->btn_close,&QPushButton::clicked,[=](){
        this->close();
    });

    //注册账号功能
    connect(ui->btn_resiger,&QPushButton::clicked,[=](){
        //注册界面实现与布局
        QWidget *widget_resiger = new QWidget();
        widget_resiger->setWindowIcon(QIcon(":/res/title_Icon.jpg"));
        QVBoxLayout *vLayout = new QVBoxLayout(widget_resiger);
            QLabel *Label_username = new QLabel("用户名:");
            QLineEdit *LineEdit_username = new QLineEdit;
            QLabel *Label_account = new QLabel("账号(0~9的纯数字):");
            QLineEdit *LineEdit_account = new QLineEdit;
            QLabel *Label_password = new QLabel("密码:");
            QLineEdit *LineEdit_password = new QLineEdit;
            QPushButton *btn_ok = new QPushButton("注册");
        LineEdit_password->setFixedSize(240,25);
        vLayout->addWidget(Label_username);
        vLayout->addWidget(LineEdit_username);
        vLayout->addWidget(Label_account);
        vLayout->addWidget(LineEdit_account);
        vLayout->addWidget(Label_password);
        vLayout->addWidget(LineEdit_password);
        vLayout->addWidget(btn_ok);
        widget_resiger->show();
        //注册按钮的功能实现
        connect(btn_ok,&QPushButton::clicked,[=](){
            //判断注册用户是否合法
            if(userdata->add_username(LineEdit_account->text(),LineEdit_username->text(),LineEdit_password->text())){
                widget_resiger->hide();
                //更新combobox中的值
                updata_user();
                //ui->account设置刚注册的账号
                ui->account->setText(LineEdit_account->text());
            }else{

            }
        });
    });

    //找回密码功能
    connect(ui->btn_find_password,&QPushButton::clicked,[=](){
        //qDebug()<<"点击了找回密码按钮";
        //找回界面实现与布局
        QWidget *widget_findps = new QWidget();
        widget_findps->setWindowIcon(QIcon(":/res/title_Icon.jpg"));
        widget_findps->setWindowTitle("找回密码");
        QLineEdit *lineEdit = new QLineEdit;
        QPushButton *btn = new QPushButton;
        QHBoxLayout *hLayout = new QHBoxLayout(widget_findps);
        hLayout->addWidget(lineEdit);
        hLayout->addWidget(btn);
        lineEdit->setPlaceholderText("输入账号");
        btn->setText("找回");
        lineEdit->setFixedSize(240,25);
        widget_findps->show();
        //找回按钮功能实现
        connect(btn,&QPushButton::clicked,[=](){
            //判断账号是否存在
            if(userdata->find_account(lineEdit->text())){
                //设置新密码界面实现
                QWidget *widget_setPassword = new QWidget;
                widget_setPassword->setWindowIcon(QIcon(":/res/title_Icon.jpg"));
                widget_setPassword->setWindowTitle("设置新密码");
                QLabel *label1 = new QLabel("输入新密码：");
                QLabel *label2 = new QLabel("确认密码");
                QLineEdit *lineEdit_newps1 = new QLineEdit;
                QLineEdit *lineEdit_newps2 = new QLineEdit;
                QPushButton *btn_OK = new QPushButton("确认");
                lineEdit_newps1->setFixedSize(240,25);
                QVBoxLayout *vLayout = new QVBoxLayout(widget_setPassword);
                vLayout->addWidget(label1);
                vLayout->addWidget(lineEdit_newps1);
                vLayout->addWidget(label2);
                vLayout->addWidget(lineEdit_newps2);
                vLayout->addWidget(btn_OK);
                widget_setPassword->show();
                //确认按钮功能实现
                connect(btn_OK,&QPushButton::clicked,this,[=](){
                    if(userdata->change_password(lineEdit->text(),lineEdit_newps1->text())){
                        widget_setPassword->close();
                        widget_findps->close();
                        ui->password->setText("");
                        //更新combobox中的值
                        updata_user();
                    }else{
                        QMessageBox::information(NULL,"失败","数据库更新信息失败",QMessageBox::Ok);
                    }
                });
            }else{
                QErrorMessage *dialog = new QErrorMessage();
                dialog->setWindowTitle("错误");
                dialog->showMessage("请检查账号输入是否正确");
            }
        });
    });

    //选择账号功能
    connect(ui->comboBox_account,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,[=](){
        int index = ui->comboBox_account->currentIndex();
        ui->account->setText(ui->comboBox_account->itemText(index));
        ui->label_head->setPixmap(QPixmap(userdata->get_headPath(ui->comboBox_account->itemText(index))));
        //判断是否选择保存密码
        if(userdata->password_state(ui->comboBox_account->itemText(index))){
            ui->checkBox_savepassword->setChecked(true);
            ui->password->setText(ui->comboBox_account->itemData(index).toString());
        }else{
            ui->checkBox_savepassword->setChecked(false);
            ui->password->setText("");
        }
    });

    //二维码功能
    connect(ui->btn_qrcode,&QPushButton::clicked,[=](){
        //qDebug()<<"点击了二维码";
        QPixmap pixmap_qrcode(":/res/qrcode.png");
        QWidget *widget_qrcode = new QWidget();
        widget_qrcode->setWindowIcon(QIcon(":/res/title_Icon.jpg"));
        widget_qrcode->setWindowTitle("二维码");
        widget_qrcode->show();
        widget_qrcode->setFixedSize(pixmap_qrcode.size());
        widget_qrcode->move(this->pos());
        QLabel *label_qrcode = new QLabel(widget_qrcode);
        label_qrcode->setPixmap(pixmap_qrcode);
        label_qrcode->setFixedSize(pixmap_qrcode.size());
        label_qrcode->show();
    });

    //登录功能
    connect(ui->btn_login,&QPushButton::clicked,[=](){
        //判断是否选择保存密码
        if(ui->checkBox_savepassword->isChecked()){
            //qDebug()<<ui->account->text()<<"选中保存密码";
            userdata->save_password(ui->account->text(),true);
        }else if(!ui->checkBox_savepassword->isChecked()){
            //qDebug()<<"未选中保存密码";
            userdata->save_password(ui->account->text(),false);
        }
        //检查用户是否合法
        if(userdata->find_username(ui->account->text(),ui->password->text())){
            mainWidget *mainwidget = new mainWidget(userdata,ui->account->text());
            //接受mainwidget的返回当前界面信号
            connect(mainwidget,&mainWidget::back_widget,this,[=](){
                this->show();
            });
            connect(mainwidget,&mainWidget::deleteUser,this,[=](){
                this->show();
                ui->account->clear();
                ui->password->clear();
            });
            this->hide();
            mainwidget->show();
        }else{
            QErrorMessage *dialog = new QErrorMessage();
            dialog->setWindowTitle("错误");
            dialog->showMessage("账号不存在或密码错误！");
        }
    });
}

//实现更新数据函数(将数据存入QComboBox)
bool Widget::updata_user(){
    ui->comboBox_account->clear();
    QList<QPair<QString,QString>> list_account;
    list_account = userdata->view_username();
    for(auto &s:list_account){
        QString pixmap = userdata->get_headPath(s.first);
        if(pixmap == ""){
            pixmap = ":/res/head.png";
        }
        ui->comboBox_account->addItem(QIcon(pixmap),s.first,QVariant(s.second));
    }
}

//实现窗口拖动
void Widget::mousePressEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton) {
        m_dragStartPosition = event->pos();
        m_isDragging = true;
    }
    QWidget::mousePressEvent(event);
}

void Widget::mouseMoveEvent(QMouseEvent* event){
    if (m_isDragging) {
        QPoint diff = event->pos() - m_dragStartPosition;
        QPoint newTopLeft = this->geometry().topLeft() + diff;
        move(newTopLeft);
    }
    QWidget::mouseMoveEvent(event);
}

void Widget::mouseReleaseEvent(QMouseEvent* event){
    if (event->button() == Qt::LeftButton) {
        m_isDragging = false;
    }
    QWidget::mouseReleaseEvent(event);
}

//实现边框阴影
void Widget::paintEvent(QPaintEvent *e){
    QPainter painter(this);
    QPixmap pixmap(":/res/background.png");
    qDrawBorderPixmap(&painter, this->rect(), QMargins(0, 0, 0, 0), pixmap);
    QRect rect(this->rect().x()+8, this->rect().y()+8, this->rect().width()-16, this->rect().height()-16);
    painter.fillRect(rect, QColor(255, 255, 255));
}

//实现事件过滤器
bool Widget::eventFilter(QObject *watched, QEvent *event){
    //账号框文本以及其图标切换
    if(watched == ui->comboBox_account || watched == ui->account){
        //判断控件是否获得焦点
        if(event->type() == QEvent::FocusIn){
            //qDebug()<<"事件过滤器";
            ui->label_username->setPixmap(QPixmap(":/res/username-select.png"));
            ui->account->setStyleSheet("QLineEdit#account{color:black}");
            //设置头像
            //ui->label_head->setPixmap(QPixmap(userdata->get_headPath(ui->account->text())));
        }else if(event->type() == QEvent::FocusOut){
            ui->label_username->setPixmap(QPixmap(":/res/username.png"));
            ui->account->setStyleSheet("QLineEdit#account{color:darkgrey}");
            //设置头像
            QString headPath = userdata->get_headPath(ui->account->text());
            QFile file(headPath);
            if(headPath == ""||!file.exists()){
                headPath = ":/res/head.png";
            }
            ui->label_head->setPixmap(QPixmap(headPath));
        }

    }
    //密码框文本以及其图标切换
    if(watched == ui->password){
        if(event->type() == QEvent::FocusIn){
            //qDebug()<<"事件过滤器";
            ui->label_password->setPixmap(QPixmap(":/res/password-select.png"));
            if(!view_password){
                ui->btn_passwordview->setIcon(QIcon(":/res/password1-hover.png"));
            }else{
                ui->btn_passwordview->setIcon(QIcon(":/res/password2-hover.png"));
            }
            ui->password->setStyleSheet("QLineEdit#password{color:black}");

        }else if(event->type() == QEvent::FocusOut){
            if(!view_password){
                ui->btn_passwordview->setIcon(QIcon(":/res/password1.png"));
            }else{
                ui->btn_passwordview->setIcon(QIcon(":/res/password2.png"));
            }
            ui->label_password->setPixmap(QPixmap(":/res/password.png"));
            ui->password->setStyleSheet("QLineEdit#password{color:darkgrey}");
        }
    }
    //查看密码的按钮的图片以及输入模式切换
    if(watched == ui->btn_passwordview){
        if(event->type() == QEvent::MouseButtonRelease && !view_password){
            ui->btn_passwordview->setIcon(QIcon(":/res/password2.png"));
            ui->password->setEchoMode(QLineEdit::Normal);
            view_password = true;
            return QWidget::eventFilter(watched, event);
        }else if(event->type() == QEvent::MouseButtonRelease && view_password){
            ui->btn_passwordview->setIcon(QIcon(":/res/password1.png"));
            ui->password->setEchoMode(QLineEdit::Password);
            view_password = false;
            return QWidget::eventFilter(watched, event);
        }
    }
    return QWidget::eventFilter(watched, event);
}

Widget::~Widget()
{
    delete ui;
}
