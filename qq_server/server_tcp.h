#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <cstring>
#include <fcntl.h>
#include "server_mysql.h"
#include "server_udp.h"
#include "Logger.h"
using namespace chat;
#define MAX_EVENTS 512

class TCP{
public:
    TCP(int port):port(port){}
    bool init_tcp();
    void start();
    ~TCP(){close(listen_fd);}
private:
    int listen_fd;
    int epoll_fd;
    int port;

    //设置套接字为非阻塞模式
    bool setNonBlock(int socket);
    //有新套接字连接服务器
    void accept_connect();
    //添加新套接字到epoll中
    bool add_epollEvent(int socket,uint32_t events);
    //有读事件发生
    void read_event(int socket);
    
};

