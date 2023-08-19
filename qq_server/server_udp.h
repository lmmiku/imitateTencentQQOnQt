#pragma once
#include <iostream>
#include <vector>
#include <unistd.h>
#include <string>
#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <thread>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include "server_mysql.h"
#include "Logger.h"
using namespace chat;
class UDP{
public:
    static UDP* instance();

    void bind(uint16_t udp_port);

    bool send(const std::string send_account,const std::string rece_account,const std::string&message,const std::string&ip,uint16_t port);

    void starter();

    //解析接收到的json数据(如果是json数据，返回一个三元组，否则返回nullptr)
    std::tuple<std::string,std::string,std::string> parse_json_data(const char *rece_data);
    //处理发送失败的消息(离线消息)
    void fail_message();

    //创建互斥锁以及条件变量，完成线程间通信和同步
    std::mutex mutex;
    std::condition_variable cond_var;
    //存储发送失败的消息
    std::unordered_map<std::string,std::vector<int64_t>> map_failsend;

    //失败的消息发送
    void fail_send(int64_t id);

private:
    UDP();
    UDP(const UDP&UDP_) = delete;
    UDP& operator=(const UDP& UDP_) = delete;
    ~UDP();

private:
    static UDP* m_instance;
    int udp_socket;
    struct sockaddr_in server_address;
};