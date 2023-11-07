#include "tcp.h"

Tcp::Tcp(QWidget *parent) :
    QWidget(parent)
{

    tcp_socket = new QTcpSocket(this);

    connect(tcp_socket,&QTcpSocket::stateChanged,this,[=](QAbstractSocket::SocketState socket_state){
        switch(socket_state)
        {
        case QAbstractSocket::ConnectedState:
        case QAbstractSocket::ConnectingState:
        case QAbstractSocket::ClosingState:
            state=true;
            emit link_success();
            break;
        case QAbstractSocket::UnconnectedState:
            state=false;
            emit link_fail();
            break;
        default:
            break;
        }
        emit link_changed();
    });


    connect(tcp_socket,&QTcpSocket::readyRead,this,[=](){
        emit read_data();
    });

}

QByteArray Tcp::tcp_read(){
    QByteArray byte = tcp_socket->readAll();
    return byte;
}

void Tcp::tcp_connect(QString account){
    tcp_socket->connectToHost(serverIP,serverPort);
    tcp_socket->write(account.toUtf8());
}

int Tcp::tcp_write(QByteArray data){


}

void Tcp::tcp_disconnect(){
    //有点问题，不能直接断开连接
    tcp_socket->disconnectFromHost();
}


Tcp::~Tcp()
{

}
