#include "server_tcp.h"
#include "server_udp.h"
#include "server_mysql.h"
#include "Logger.h"
#include <iostream>
using namespace chat;
int main(){
    Logger::instance()->open("./qq_server.log");
    TCP *tcp = new TCP(8081);
    tcp->init_tcp();
    std::thread tcp_thread([&](){
        tcp->start();
    });
    UDP::instance()->bind(8080);
    std::thread udp_thread([&](){
        UDP::instance()->starter();
    });
    
    while(1){}
}
