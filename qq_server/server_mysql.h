#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "Logger.h"
using namespace chat;
class DB_Mysql{
public:
    std::string get_connect_string();

    static DB_Mysql* instance();
    //查看用户是否在线
    bool is_online(std::string account);
    //保存聊天记录
    int64_t save_chat_history(std::string send_account,std::string rece_account,std::string message,std::string status);
    //获取发送失败的聊天记录
    std::tuple<std::string,int,std::string> get_chat_history(int64_t id);
    //查找client_info中客户端是否存在
    bool exist_account(std::string account);
    //更新用户的客户端信息（client_info,用于tcp通信）
    bool updata_client_info(std::string account,std::string tcp_ip,int tcp_port,int tcp_socket,std::string status);
    //更新用户客户端信息（client_info，用于udp通信）
    bool updata_client_info(std::string account,std::string udp_ip,int udp_port);
    //获取目标客户端的IP+port地址(用于udp通信)
    std::pair<std::string,int> get_target_ip(std::string account);
    //将聊天记录中status改为成功
    bool fail_to_success(int64_t id);
    //获取目标客户端的tcp_socket地址(用于tcp通信)
    int get_target_socket(std::string account);
    //获取当前时间戳（用于保存聊天记录）
    std::string get_timestamp();
    //用户下线，将ip，tcp_socket，tcp_port，置为-1
    bool setdefault_info(int socket);

private:
    DB_Mysql();
    DB_Mysql(const DB_Mysql &DB_Mysql_) = delete;
    DB_Mysql& operator=(const DB_Mysql &DB_Mysql_) = delete;
    ~DB_Mysql();
private:
    bool isconnected = false;
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* conn;
    //数据库信息
    std::string username;
    std::string password;
    std::string ip;
    std::string port;
    std::string DB_Name;
    static DB_Mysql* m_instance;
};

