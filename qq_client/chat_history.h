#ifndef CHAT_HISTORY_H
#define CHAT_HISTORY_H

#include <QWidget>
#include <QFile>
#include<QString>
#include<QCoreApplication>
#include<QDebug>
#include<QDir>

namespace Ui {
class chat_history;
}

class chat_history : public QWidget
{
    Q_OBJECT

public:
    explicit chat_history(QWidget *parent = nullptr);
    ~chat_history();

    //存储聊天记录
    void storage_chat_history(QString data);
    //读取聊天记录
    QString read_chat_history();

    QString dirPath;

private:
    Ui::chat_history *ui;
};
#endif // CHAT_HISTORY_H
