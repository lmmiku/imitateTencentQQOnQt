#include "widget.h"
#include "ui_widget.h"
#include "tcp.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //需要加载ui后赋值
    default_rect = this->rect();
    //去掉边框
    this->setWindowFlags(Qt::WindowType::FramelessWindowHint);

    ui->label_top->lower();

    //背景透明
    this->setAttribute(Qt::WA_TranslucentBackground);

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
    QBitmap mask(ui->label_head->size());
    mask.fill(Qt::color0);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing, true);  // 启用抗锯齿
    painter.setBrush(Qt::color1);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(mask.rect());
    painter.end();
    ui->label_head->setMask(mask);

    //初始化配置文件对象
    this->settings = new QSettings("qq.ini",QSettings::IniFormat);

    //更新下拉框数据
    this->updata_user();

    //加载最后一次登录信息
    this->getLast_login();

    //设置comboBox的默认页面为-1，防止点击默认第一个没有反应
    ui->comboBox_account->setCurrentIndex(-1);

    //最小化按钮功能
    connect(ui->btn_min,&QPushButton::clicked,[=](){
        this->showMinimized();
    });

    //关闭按钮功能
    connect(ui->btn_close,&QPushButton::clicked,[=](){
        this->animation_widget_out_1(this);
        QTimer::singleShot(330,this,[=](){
            this->close();
        });
    });

    //更多按钮功能
    QMenu *menu =new QMenu(this);
    QAction *action_delete = new QAction("删除登录信息",this);
    action_delete->setIcon(QIcon(":/res/cancle_out.png"));
    connect(action_delete,&QAction::triggered,this,[=](){
        settings->beginGroup(QString("userdata_%1").arg(ui->account->text()));
        settings->remove("");
        settings->endGroup();
        this->updata_user();
        ui->account->clear();
        ui->password->clear();
        ui->comboBox_account->setCurrentIndex(-1);
    });
    menu->addAction(action_delete);
    ui->btn_more->setMenu(menu);
    ui->btn_more->setStyleSheet("QPushButton::menu-indicator{image:none;}");

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
                //updata_user();
                //ui->account设置刚注册的账号
                ui->account->setText(LineEdit_account->text());
                ui->password->clear();
                QString  pixmap = userdata->get_headPath(LineEdit_account->text());
                if(pixmap == ""){
                    pixmap = ":/res/head.png";
                }
                ui->label_head->setPixmap(QPixmap(pixmap));
                ui->password->text().clear();
            }else{

            }
        });
    });

    //找回密码功能
    connect(ui->btn_find_password,&QPushButton::clicked,[=](){
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
                        this->updata_user();
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
        QString pixmap = userdata->get_headPath(ui->comboBox_account->itemText(index));
        if(pixmap == ""){
            pixmap = ":/res/head.png";
        }
        ui->label_head->setPixmap(QPixmap(pixmap));
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
        ui->btn_login->setEnabled(false);
        //判断是否选择保存密码
        if(ui->checkBox_savepassword->isChecked()){
            userdata->save_password(ui->account->text(),true);
        }else if(!ui->checkBox_savepassword->isChecked()){
            userdata->save_password(ui->account->text(),false);
        }
        //登录按钮动画
        this->animation_login();
        QTimer::singleShot(250,this,[=](){
            //检查用户是否合法
            if((userdata->find_username(ui->account->text(),ui->password->text()))&&(userdata->getstate_user(ui->account->text()) != "在线")){
                mainwidget = new mainWidget(userdata,ui->account->text());
                //接受mainwidget的返回当前界面信号
                connect(mainwidget,&mainWidget::back_widget,this,[=](){
                    //this->show();
                    this->animation_widget_in(this);
                    QTimer::singleShot(400,[=](){
                        this->updata_user();
                        ui->comboBox_account->setCurrentIndex(-1);
                        this->getLast_login();
                        delete mainwidget;
                        mainwidget = nullptr;
                        ui->btn_login->setEnabled(true);
                    });
                });
                connect(mainwidget,&mainWidget::deleteUser,this,[=](){
                    this->animation_widget_in(this);
                    QTimer::singleShot(400,[=](){
                        settings->beginGroup(QString("userdata_%1").arg(ui->account->text()));
                        settings->remove("");
                        settings->endGroup();
                        this->updata_user();
                        ui->comboBox_account->setCurrentIndex(-1);
                        delete mainwidget;
                        mainwidget = nullptr;
                        ui->btn_login->setEnabled(true);
                    });
                });
                connect(mainwidget,&mainWidget::exitApplixation,this,[=](){
                    this->close();
                });

                //保存账号密码到配置文件中
                settings->setValue(QString("userdata_%1/account").arg(ui->account->text()),QString(ui->account->text()));
                settings->setValue(QString("userdata_%1/password").arg(ui->account->text()),QString(ui->password->text()));
                //settings->setValue(QString("userdata_%1/password").arg(ui->account->text()),QString(ui->account->text()));
                saveLast_login(ui->account->text());

                //界面消失
                this->animation_widget_out(this);
                QTimer::singleShot(400,this,[=](){
                    this->animation_mainwidget_in(mainwidget);
                    mainwidget->show();
                });
            }else{
                QErrorMessage *dialog = new QErrorMessage();
                dialog->setWindowTitle("错误");
                dialog->showMessage("账号（密码错误）或账号已登陆");
                ui->btn_login->setEnabled(true);
            }
        });
    });
}

//实现保存最后一次登录信息
void Widget::saveLast_login(QString account){
    QStringList accounts = settings->childGroups();
    for(auto &account_:accounts){
        settings->beginGroup(account_);
        QString account_temp = settings->value("account").toString();
        if(!account.compare(account_temp)){
            settings->setValue(QString("last_login"),QString("true"));
        }else{
            settings->setValue(QString("last_login"),QString("false"));
        }
        settings->endGroup();
    }
}

//加载最后一次登录信息
void Widget::getLast_login(){
    QStringList accounts = settings->childGroups();
    for(auto &account_:accounts){
        settings->beginGroup(account_);
        QString state = settings->value("last_login").toString();
        if("true" == state){
            QString account_login = settings->value("account").toString();
            QString headPath = "";
            ui->account->setText(account_login);
            if(userdata->password_state(account_login)){
                ui->password->setText(settings->value("password").toString());
                ui->checkBox_savepassword->setChecked(true);
            }
            if("" == userdata->get_headPath(account_login)){
                headPath = ":/res/head.png";
            }
            ui->label_head->setPixmap(userdata->get_headPath(account_login));
            settings->endGroup();
            break;
        }
        settings->endGroup();
    }
}

//实现更新数据函数(将数据存入QComboBox)
bool Widget::updata_user(){
    ui->comboBox_account->clear();
    ui->comboBox_account->setIconSize(QSize(31,31));
    //从ini配置文件中读取已登陆过的账号密码
    QStringList accounts = this->settings->childGroups();
    for(auto& account_:accounts){
        //进入对应分组
        settings->beginGroup(account_);
        QString account = settings->value("account").toString();
        QString password = settings->value("password").toString();
        settings->endGroup();

        QString pixmap = userdata->get_headPath(account);
        if(pixmap == ""){
            pixmap = ":/res/head.png";
        }
        //圆形遮罩
        QPixmap round_pixmap(pixmap);
        QBitmap mask(round_pixmap.size());
        mask.fill(Qt::color0);
        QPainter painter(&mask);
        painter.setRenderHint(QPainter::Antialiasing,true);
        painter.setBrush(Qt::color1);
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(mask.rect());
        painter.end();
        round_pixmap.setMask(mask);

        ui->comboBox_account->addItem(QIcon(round_pixmap),account,QVariant(password));
    }
    return true;
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

//登录按钮动画实现
void Widget::animation_login(){
    QPropertyAnimation * animation_In = new QPropertyAnimation(ui->btn_login,"geometry");
    animation_In->setDuration(200);
    animation_In->setStartValue(QRect(ui->btn_login->x(),ui->btn_login->y(),ui->btn_login->width(),ui->btn_login->height()));
    animation_In->setEndValue(QRect(ui->btn_login->x(),ui->btn_login->y()+3,ui->btn_login->width(),ui->btn_login->height()));
    animation_In->setEasingCurve(QEasingCurve::OutQuad);
    animation_In->setTargetObject(ui->btn_login);
    animation_In->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(210,this,[=](){
        QPropertyAnimation * animation_Out = new QPropertyAnimation(ui->btn_login,"geometry");
        animation_Out->setDuration(200);
        animation_Out->setStartValue(QRect(ui->btn_login->x(),ui->btn_login->y(),ui->btn_login->width(),ui->btn_login->height()));
        animation_Out->setEndValue(QRect(ui->btn_login->x(),ui->btn_login->y()-3,ui->btn_login->width(),ui->btn_login->height()));
        animation_Out->setEasingCurve(QEasingCurve::OutQuad);
        animation_Out->start(QAbstractAnimation::DeleteWhenStopped);
    });

}

//界面消失动画
void Widget::animation_widget_out(QWidget *target){
    QPropertyAnimation *animation= new QPropertyAnimation(target,"geometry");
    animation->setDuration(320);
    animation->setStartValue(QRect(target->x(), target->y(), target->width(),target->height()));
    animation->setEndValue(QRect(target->x()+target->width(), target->y(), 0,target->height()));
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Widget::animation_widget_out_1(QWidget *target){
    QPropertyAnimation *animation= new QPropertyAnimation(target,"geometry");
    animation->setDuration(320);
    animation->setStartValue(QRect(target->x(), target->y(), target->width(),target->height()));
    animation->setEndValue(QRect(target->x(), target->y(), target->width(),0));
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

//界面出现动画
void Widget::animation_widget_in(QWidget *target){
    int end_x = mainwidget->x()-default_rect.width();
    int end_y = target->y();
    if(end_x < desktop.x()){
        end_x = 0;
    }
    if(end_y >desktop.height()){
        end_y = desktop.height()-this->height();
    }
    QPropertyAnimation *animation= new QPropertyAnimation(target,"geometry");
    animation->setDuration(320);
    animation->setStartValue(QRect(mainwidget->x(), target->y(), 0,target->height()));
    animation->setEndValue(QRect(end_x, end_y, default_rect.width(),target->height()));
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

//mainwidget界面出现动画
void Widget::animation_mainwidget_in(QWidget *target){
    QPropertyAnimation *animation = new QPropertyAnimation(target,"geometry");
    int start_x = this->x();int start_y = this->y();
    int end_x = this->x()+mainwidget->width();int end_y = this->y()+mainwidget->height();
    if((end_x+mainwidget->width())>desktop.width()){
        end_x = desktop.width()-mainwidget->width();
    }
    if(end_y+mainwidget->y()>desktop.height()){
        end_y = desktop.height()-mainwidget->height();
    }else{
        end_y = 0;
    }
    animation->setDuration(320);
    animation->setStartValue(QRect(start_x,start_y,0,mainwidget->height()));
    animation->setEndValue(QRect(end_x,end_y,mainwidget->width(),mainwidget->height()));
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

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
            ui->label_username->setPixmap(QPixmap(":/res/username-select.png"));
            ui->account->setStyleSheet("QLineEdit#account{color:black}");
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
