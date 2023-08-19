#include "udp.h"

udp::udp(QWidget *parent) :
    QWidget(parent)
{
}

udp::udp(QString account){
    udp_socket->bind(QHostAddress::Any,8000);
    udp_socket->writeDatagram(account.toUtf8(),QHostAddress(server_ip),server_port);
    connect(udp_socket,&QUdpSocket::readyRead,this,[=](){
        emit rece_data();
    });
}

void udp::udp_send(QString account ,QString friend_account,QString message){
//    if(count-- == 0 || count == 20){
//        udp_socket->writeDatagram(account.toUtf8(),QHostAddress(server_ip),server_port);
//    }
    //准备json数据
    QJsonObject json;
    json["source_account"] = account;
    json["target_account"] = friend_account;
    json["message"] = message;

    //将json转为QByteArray
    QJsonDocument json_document(json);
    QByteArray jsonData = json_document.toJson();
    qDebug()<<QString::fromUtf8(jsonData);
    udp_socket->writeDatagram(jsonData,QHostAddress(server_ip),server_port);
    emit send_data();
}

QString udp::udp_rece(){
    QByteArray datagram;
    datagram.resize(udp_socket->pendingDatagramSize());
    udp_socket->readDatagram(datagram.data(),datagram.size());
    qDebug()<<QString::fromUtf8(datagram);
    return QString::fromUtf8(datagram);
}

udp::~udp()
{

}
