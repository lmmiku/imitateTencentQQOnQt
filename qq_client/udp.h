#ifndef UDP_H
#define UDP_H

#include <QWidget>
#include <QUdpSocket>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
class udp : public QWidget
{
    Q_OBJECT

public:
    explicit udp(QWidget *parent = nullptr);
    ~udp();
    udp(QString account);
    //发送数据
    void udp_send(QString account ,QString friend_account,QString message);
    //接受数据
    QString udp_rece();

    //每发送20条消息更新一下数据库的信息
    int count = 20;
    QString server_ip = "120.53.84.94";
    quint16 server_port = 8080;
    QUdpSocket* udp_socket = new QUdpSocket();




signals:
    void rece_data();
    //通知chat_button更新最后一条聊天记录
    void send_data();
};

#endif // UDP_H
