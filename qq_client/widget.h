#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFile>
#include <QRect>
#include <QTime>
#include <QEvent>
#include <QMovie>
#include <QDebug>
#include <QLabel>
#include <QMenu>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QScreen>
#include <QApplication>
#include <QSettings>
#include <QLineEdit>
#include <QPushButton>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPushButton>
#include <qdrawutil.h>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QListView>
#include <QStyledItemDelegate>
#include "mainwidget.h"
#include "tcp.h"

#include "user_data.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

    //窗口拖动
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    QPoint m_dragStartPosition;
    bool m_isDragging = false;

    //实现边框阴影
    void paintEvent(QPaintEvent *event);

    //ini文件对象
    QSettings *settings = nullptr;

    //用户数据
    user_data *userdata = new user_data();

    bool add_user = true;

    //更新数据
    bool updata_user();

    //保存最后一次登录信息
    void saveLast_login(QString account);

    //加载最后一次登录信息
    void getLast_login();

    //登录按钮动画
    void animation_login();

    //界面消失动画
    void animation_widget_out(QWidget *target);
    void animation_widget_out_1(QWidget *target);
    //界面出现动画
    void animation_widget_in(QWidget *target);
    void animation_mainwidget_in(QWidget *target);

    //获取可用桌面大小
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect desktop = screen->geometry();

    //默认程序窗口大小
    QRect default_rect = QRect();

    //主界面
    mainWidget *mainwidget = nullptr;

    ~Widget();

   //事件过滤器实现输入密码或者输入账号时图标以及文本框字体颜色变换
             //实现是否显示密码以及其图标的切换
   bool eventFilter(QObject *watched, QEvent *event);

   //是否显示密码
   bool view_password = false;

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
