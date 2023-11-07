#ifndef CHAT_BUTTON_H
#define CHAT_BUTTON_H

#include <QWidget>
#include <QString>
#include <QMouseEvent>
#include <QMenu>
#include <QPainter>
#include <QBitmap>
#include <QAction>
namespace Ui {
class chat_button;
}

class chat_button : public QWidget
{
    Q_OBJECT

public:
    explicit chat_button(QWidget *parent = nullptr);

    chat_button(QString Icon,QString username,QString chat);

    //更新最后一条聊天记录
    void update_chatHistory(QString lase_message);

    //鼠标事件(如果有必要，可以使用QTimer对单击和双击事件进行区分)
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    //实现鼠标离开和进入控件改变背景颜色的功能
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    //修改最后一条聊天记录
    void changed_lastChat();
    ~chat_button();
    //鼠标右键点击出现一个子菜单
    QMenu *menu= nullptr;
signals:
    void mouseDoubleDlick();
    void mousePress();
    void delete_chat();
    void delete_chatHistory();

private:
    Ui::chat_button *ui;
};

#endif // CHAT_BUTTON_H
