#ifndef FRIEND_BUTTON_H
#define FRIEND_BUTTON_H

#include <QWidget>
#include <QMenu>
#include <QEvent>
#include <QMouseEvent>
#include <QBitmap>
#include <QPainter>
#include <QDebug>

namespace Ui {
class friend_button;
}

class friend_button : public QWidget
{
    Q_OBJECT

public:
    explicit friend_button(QWidget *parent = nullptr);

    friend_button(QString Icon,QString username,QString signature);
    ~friend_button();
    //鼠标事件(如果有必要，可以使用QTimer对单击和双击事件进行区分)
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);
    //实现鼠标进入和离开控件改变背景颜色的功能
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    //修改button_Icon
    void updata_Icon(QString Icon);

    //鼠标右键点击出现一个子菜单
    QMenu *menu= nullptr;

signals:
    void mouseDoubleDlick();
    void mousePress();
    void delete_friend();
    void add_chat();
private:
    Ui::friend_button *ui;
};

#endif // FRIEND_BUTTON_H
