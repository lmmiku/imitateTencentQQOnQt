#include "server_mysql.h"
DB_Mysql* DB_Mysql::m_instance = new DB_Mysql();

auto DB_Mysql::instance()->DB_Mysql*{
    return m_instance;
}

DB_Mysql::DB_Mysql():username("root"),password("12345678"),ip("120.53.84.94"),port("3306"),DB_Name("user_data"){
        driver = sql::mysql::get_mysql_driver_instance();
        try{
            conn = driver->connect(get_connect_string(), username, password);
            conn->setSchema(DB_Name);
            if(conn->isValid()){
                isconnected = true;
                std::cout<<"数据库连接成功"<<std::endl;
                debug("%s","数据库连接成功");
            }
        }catch(sql::SQLException& e){
                debug("%s","数据库连接失败");
        }
}

std::string DB_Mysql::get_timestamp(){
    std::time_t now= std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);

    std::ostringstream oss;
    oss<<std::put_time(timeinfo,"%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string DB_Mysql::get_connect_string(){
    return "tcp://"+ip+":"+port;
}

std::pair<std::string,int> DB_Mysql::get_target_ip(std::string account){
    sql::PreparedStatement *pre_stmt = conn->prepareStatement("SELECT udp_ip,udp_port FROM client_info WHERE account = ?");
    pre_stmt->setString(1, account);
    sql::ResultSet *res;
    std::pair<std::string,int> pair;
    try{
        res = pre_stmt->executeQuery();
    }catch(sql::SQLException &e){
        error("get %s ip error: %s",account,e.what());
    }
    if(res->next()){
        pair.first = res->getString("udp_ip");
        pair.second = res->getInt("udp_port");
    }
    return pair;
}

int64_t DB_Mysql::save_chat_history(std::string send_account,std::string rece_account,std::string message,std::string status){
    sql::PreparedStatement *pre_stmt = conn->prepareStatement("INSERT INTO chat_history (sender_account,receiver_account,message,timestamp,status) VALUES(?,?,?,?,?)");
    pre_stmt->setString(1,send_account);
    pre_stmt->setString(2,rece_account);
    pre_stmt->setString(3,message);
    pre_stmt->setDateTime(4,get_timestamp());
    pre_stmt->setString(5,status);
    try{
        pre_stmt->execute();
        //获取插入消息的id值
        sql::Statement *stmt = conn->createStatement();
        sql::ResultSet *res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
        if(res->next()){
            //std::cout<<"插入新消息的id值:"<<res->getInt64(1)<<std::endl;
            return res->getInt64(1);
        }
        return -1;
    }catch(sql::SQLException &e){
        error("save_chat_history sender:%s receiver:%s message:%s error: %s",send_account,rece_account,message,e.what());
        return false;
    }
    return true;
}

bool DB_Mysql::fail_to_success(int64_t id){
    sql::PreparedStatement *pre_stmt = conn->prepareStatement("UPDATE chat_history SET status = ? WHERE id = ?");
    pre_stmt->setString(1,"success");
    pre_stmt->setInt64(2,id);
    try{
        pre_stmt->execute();
    }catch(sql::SQLException&e){
        error("%ld fail to success error: %s",id,e.what());
        return false;
    }
    return true;
}

bool DB_Mysql::exist_account(std::string account){
    sql::PreparedStatement *pre_stmt = conn->prepareStatement("SELECT * FROM client_info WHERE account = ?");
    pre_stmt->setString(1,account);
    sql::ResultSet *res;
    try{
        res = pre_stmt->executeQuery();
    }catch(sql::SQLException &e){
        error("select %s whether exist error: %s",account,e.what());
    }
    if(res->next()){
        return true;
    }
    return false;
}

bool DB_Mysql::updata_client_info(std::string account,std::string tcp_ip,int tcp_port,int tcp_socket,std::string status){
    sql::PreparedStatement *pre_stmt;
    if(exist_account(account)){
        pre_stmt = conn->prepareStatement("UPDATE client_info SET tcp_ip = ?, tcp_port = ?, tcp_socket = ?, status = ? WHERE account = ?");
        pre_stmt->setString(1,tcp_ip);
        pre_stmt->setInt(2,tcp_port);
        pre_stmt->setInt(3,tcp_socket);
        pre_stmt->setString(4,status);
        pre_stmt->setString(5,account);
    }else{
        pre_stmt = conn->prepareStatement("INSERT INTO client_info (account,tcp_ip,tcp_port,tcp_socket,status) VALUES(?,?,?,?,?)");
        pre_stmt->setString(1,account);
        pre_stmt->setString(2,tcp_ip);
        pre_stmt->setInt(3,tcp_port);
        pre_stmt->setInt(4,tcp_socket);
        pre_stmt->setString(5,status);
    }
    try{
        pre_stmt->execute();
    }catch(sql::SQLException &e){
        error("update %s info tcp_ip:%s tcp_port:%d tcp_socket:%d error: %s",account,tcp_ip,tcp_port,tcp_socket,e.what());
        return false;
    }
    return true;
}

std::tuple<std::string,int,std::string> DB_Mysql::get_chat_history(int64_t id){
    std::tuple<std::string,int,std::string> tuple;
    sql::PreparedStatement *pre_stmt = conn->prepareStatement("SELECT chat_history.message,client_info.udp_ip,client_info.udp_port FROM chat_history JOIN client_info ON chat_history.receiver_account = client_info.account WHERE id = ?");
    pre_stmt->setInt64(1,id);
    try{
        sql::ResultSet *res = pre_stmt->executeQuery();
        if(!res->next()){
            return tuple;
        }
        //std::cout<<"message:"<<res->getString(1)<<"  "<<"udp_ip:"<<res->getString(2)<<"  "<<"udp_pot:"<<res->getInt(3);
        tuple = std::make_tuple(res->getString(2),res->getInt(3),res->getString(1));
        return tuple;
    }catch(sql::SQLException &e){
        //std::cout<<"sql fail to select chat_history:"<<e.what()<<std::endl;
        std::tuple<std::string,int,std::string> tuple;
        return tuple;
    }
}

bool DB_Mysql::updata_client_info(std::string account,std::string udp_ip,int udp_port){
    sql::PreparedStatement *pre_stmt;
    if(exist_account(account)){
        pre_stmt = conn->prepareStatement("UPDATE client_info SET udp_ip = ?, udp_port = ? WHERE account = ?");
        pre_stmt->setString(1,udp_ip);
        pre_stmt->setInt(2,udp_port);
        pre_stmt->setString(3,account);
    }else{
        pre_stmt = conn->prepareStatement("INSERT INTO client_info (account,udp_ip,udp_port) VALUES(?,?,?)");
        pre_stmt->setString(1,account);
        pre_stmt->setString(2,udp_ip);
        pre_stmt->setInt(3,udp_port);
    }
    try{
        pre_stmt->execute();
    }catch(sql::SQLException &e){
        error("update %s info udp_ip:%s udp_port:%d error: %s",account,udp_ip,udp_port,e.what());
        return false;
    }
    return true;
}

bool DB_Mysql::is_online(std::string account){
    sql::PreparedStatement *pre_stmt = conn->prepareStatement("SELECT status FROM client_info WHERE account = ?");
    pre_stmt->setString(1,account);
    sql::ResultSet *res;
    try{
        res = pre_stmt->executeQuery();
    }catch(sql::SQLException &e){
        error("%s whether online error: %s",account,e.what());
    }
    if(res->next() && (res->getString("status") == "在线")){
        return true;
    }
    return false;
}

int DB_Mysql::get_target_socket(std::string account){
    sql::PreparedStatement *pre_stmt = conn->prepareStatement("SELECT tcp_socket FROM client_info WHERE account = ?");
    pre_stmt->setString(1,account);
    sql::ResultSet *res;
    try{
        res = pre_stmt->executeQuery();
    }catch(sql::SQLException &e){
        error("get %s socket error: %s",account,e.what());
    }
    if(res->next()){
        //std::cout<<res->getInt("tcp_socket");
        return res->getInt("tcp_socket");
    }
    return -1;
}

bool DB_Mysql::setdefault_info(int socket){
    sql::PreparedStatement *pre_stmt = conn->prepareStatement("UPDATE client_info SET tcp_ip = ?, tcp_port = ?, tcp_socket = ?, status = ? WHERE tcp_socket = ?");
    pre_stmt->setString(1,"0.0.0.0");
    pre_stmt->setInt(2,-1);
    pre_stmt->setInt(3,-1);
    pre_stmt->setString(4,"离线");
    pre_stmt->setInt(5,socket);  
    try{
        pre_stmt->execute();
    }catch(sql::SQLException &e){
        error("%d setdefault_info error: %s",socket,e.what());
        return false;
    }
    return true;
}

DB_Mysql::~DB_Mysql(){
    delete conn;
}

