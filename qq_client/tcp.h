#ifndef TCP_H
#define TCP_H

#include <QWidget>
#include<QTcpSocket>
#include<QTcpServer>
#include <QNetworkInterface>
#include <QDebug>
#include <QtNetwork/QAbstractSocket>

class Tcp : public QWidget
{
    Q_OBJECT

public:
    explicit Tcp(QWidget *parent = nullptr);

    //发送文件
    int tcp_write(QByteArray data);

    QByteArray tcp_read();
    //断开连接
    void tcp_disconnect();
    //连接服务器
    void tcp_connect(QString account);
    QTcpSocket *tcp_socket = nullptr;
    //服务器连接状态
    bool state = false;
    // 服务器IP地址
    QString serverIP = "120.53.84.94";
    // 服务器端口号
    int serverPort = 8081;
    ~Tcp();

signals:
    void read_data();
    void link_success();
    void link_fail();
    void link_changed();

};

#endif // TCP_H
