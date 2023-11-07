#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QFont>
#include <QList>
#include <QPair>
#include <QFile>
#include <QTimer>
#include <QLabel>
#include <QPoint>
#include <QThread>
#include <QAction>
#include <QPixmap>
#include <QPainter>
#include <QBitmap>
#include <QToolBox>
#include <QGroupBox>
#include <QTabWidget>
#include <QToolButton>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QFileDialog>
#include <QVBoxLayout>
#include <qdrawutil.h>
#include <qpropertyanimation.h>
#include <QGraphicsEffect>
#include <QGraphicsDropShadowEffect>

#include "tcp.h"
#include "user_data.h"
#include "friend_button.h"
#include "chat_button.h"
#include "chatbox.h"

namespace Ui {
class mainWidget;
}

class mainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit mainWidget(QWidget *parent = nullptr);
    mainWidget(user_data* userdata,QString account);
    ~mainWidget();

    user_data* userdata;
    //窗口拖动
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    QPoint m_dragStartPosition;
    bool m_isDragging = false;
    int toolBox_index = 0;

    //tcp套接字
    Tcp *tcp_socket = nullptr;
    //udp套接字
    udp *udp_socket = nullptr;

    //添加联系人
    bool add_friend(user_data *userdata,QString account ,QString frend_account,int currentIndex=-1);

    //初始化已有联系人
    bool init_friend(user_data *userdata,QString account);

    void paintEvent(QPaintEvent *e);

    bool eventFilter(QObject *watched, QEvent *event);

    //添加到会话列表(chat_button)
    bool add_chat(user_data *userdata,QString account , QString friend_account);
    //初始化已有会话
    bool init_chat(user_data *userdata,QString account);

    //消失动画
    void animation_out(QWidget *target);

    //存放消息界面
    QList<QPair<QString,chatbox*>> chatBoxList;

    QVBoxLayout *layout;
signals:
    //返回主界面信号
    void back_widget();
    //注销自己账号信号
    void deleteUser();
    //改变头像信号
    void changedHead();
    //更新最后一条聊天记录
    void update_chatLastHistory();
    //退出信号
    void exitApplixation();
private:
    Ui::mainWidget *ui;
};

#endif // MAINWIDGET_H
