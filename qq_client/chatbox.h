#ifndef CHATBOX_H
#define CHATBOX_H

#include <QWidget>
#include <QFile>
#include <QMenu>
#include <QList>
#include <QAction>
#include <QDialog>
#include <QDebug>
#include <QPainter>
#include <QPalette>
#include <QMouseEvent>
#include <QPushButton>
#include <QPaintEvent>
#include <qdrawutil.h>
#include <QGridLayout>
#include <QColorDialog>
#include <QGraphicsEffect>
#include <QFileDialog>
#include <QCoreApplication>
#include <QFile>
#include "user_data.h"
#include "tcp.h"
#include "udp.h"
#include "chat_bubble.h"


namespace Ui {
class chatbox;
}

class chatbox : public QWidget
{
    Q_OBJECT

public:
    explicit chatbox(QWidget *parent = nullptr);
    chatbox(user_data *userdata,udp*udp_socket,Tcp*tcp_socket,QString account,QString friend_account);

    ~chatbox();
    //窗口拖动
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    QPoint m_dragStartPosition;
    bool m_isDragging = false;

    //显示所有聊天记录
    void view_chatHistory(user_data *userdata,QString account,QString friend_account);

    //边框阴影
    void paintEvent(QPaintEvent *e);

    QVBoxLayout *layout;

signals:
    //删除好友信号
    void delete_User();
private:
    Ui::chatbox *ui;
};

#endif // CHATBOX_H
