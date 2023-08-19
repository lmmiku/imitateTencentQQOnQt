#include "tcp.h"

Tcp::Tcp(QWidget *parent) :
    QWidget(parent)
{

    client = new QTcpSocket(this);

    connect(client,&QTcpSocket::stateChanged,this,[=](QAbstractSocket::SocketState socket_state){
        switch(socket_state)
        {
        case QAbstractSocket::ConnectedState:
        case QAbstractSocket::ConnectingState:
        case QAbstractSocket::ClosingState:
            state=true;//isOpen即为连接标志
            emit link_success();
            break;
        case QAbstractSocket::UnconnectedState:
            state=false;
            emit link_fail();
            break;
        default:
            break;
        }
        //qDebug()<<"服务器连接状态改变";
        emit link_changed();
    });


    connect(client,&QTcpSocket::readyRead,this,[=](){
        emit read_data();
    });

    /*  //别使用这两个信号与槽，会变得不幸
    //连接失败
    connect(client, &QTcpSocket::errorOccurred, this, [=](QAbstractSocket::SocketError socketError){
        emit link_success();
        state = false;
        qDebug() << "Error occurred:" << socketError;
        qDebug() << "Error message:" << client->errorString();
    });
    //连接成功
    connect(client,&QTcpSocket::connected,this,[=](){
        qDebug()<<"服务器连接成功";
        state = true;
        emit link_success();
    });*/

}

QString Tcp::tcp_read(){
    return QString::fromUtf8(client->readAll());
}

void Tcp::tcp_connect(QString account){
    client->connectToHost(serverIP,serverPort);
    client->write(account.toUtf8());
}

int Tcp::tcp_write(QString data){
//    if(client == nullptr){
//        qDebug()<<"=================================================";
//    }
    return client->write(data.toUtf8());
}

void Tcp::tcp_disconnect(){
    //有点问题，不能直接断开连接
    client->disconnectFromHost();
}


Tcp::~Tcp()
{

}
