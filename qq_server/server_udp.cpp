#include "server_udp.h"


UDP* UDP::m_instance = new UDP();

auto UDP::instance()->UDP*{
    return m_instance;
}

void UDP::bind(uint16_t udp_port){
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(udp_port);

    if(::bind(udp_socket,(struct sockaddr*)&server_address,sizeof(server_address))<0){
        error("bind error:%s",strerror(errno));
    }
}

bool UDP::send(const std::string send_account,const std::string rece_account,const std::string &message, const std::string &ip, uint16_t port){
    if(!DB_Mysql::instance()->is_online(rece_account)){
        int64_t id = DB_Mysql::instance()->save_chat_history(send_account,rece_account,message,"fail");
        //[]当key不存在，插入一个新的键值对，并返回引用，如果存在，则自动更新值
        std::lock_guard<std::mutex> lock(mutex);
        map_failsend[rece_account].push_back(id);
        return false;
    }
    sockaddr_in dest_addr{AF_INET,htons(port),inet_addr(ip.c_str())};
    // struct sockaddr_in dest_addr;
    // dest_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    // dest_addr.sin_family = AF_INET;
    // dest_addr.sin_port = htons(port);

    ssize_t send_bytes = ::sendto(udp_socket, message.c_str(), message.size(), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

    if (send_bytes == -1){
        int64_t id = DB_Mysql::instance()->save_chat_history(send_account,rece_account,message,"fail");
        std::lock_guard<std::mutex> lock(mutex);
        map_failsend[rece_account].push_back(id);
        error("sendto fail:%s",strerror(errno));
        return false;
    }
    DB_Mysql::instance()->save_chat_history(send_account,rece_account,message,"success");
    return true;
}

void UDP::fail_send(int64_t id){
    //std::cout<<"调用fail_send"<<id<<std::endl;
    std::tuple<std::string,int,std::string> tuple = DB_Mysql::instance()->get_chat_history(id);
    std::string message = std::get<2>(tuple);
    sockaddr_in dest_addr{AF_INET,htons(std::get<1>(tuple)),inet_addr(std::get<0>(tuple).c_str())};
    int ret_send = ::sendto(udp_socket,message.c_str(),message.size(),0,(sockaddr*)&dest_addr,sizeof(dest_addr));
    if(ret_send ==-1){
        //没有能力再去处理发送失败消息再次发送失败的情况，看运气吧
        error("发送信息两次失败，已放弃");
        return;
    }
    DB_Mysql::instance()->fail_to_success(id);
}

void UDP::fail_message(){
    //std::cout<<"调用fail_message"<<std::endl;
    while(true){
        std::vector<std::string> temp_account;
        std::unique_lock<std::mutex> lock(mutex);
        cond_var.wait(lock,[&](){
            bool online = false;
            for(const auto& pair: map_failsend){
                //std::cout<<"----"<<pair.first<<std::endl;
                if(DB_Mysql::instance()->is_online(pair.first)){
                    temp_account.push_back(pair.first);
                    online = true;
                }
            }
            //std::cout<<"online:"<<online<<std::endl;
            return online;
        });
        for(auto account:temp_account){
            for(auto id:map_failsend[account]){
                fail_send(id);
            }
        }
        for(auto account:temp_account){
            map_failsend.erase(account);
        }
        lock.unlock();
    }
}

std::tuple<std::string,std::string,std::string> UDP::parse_json_data(const char*rece_data){
    std::tuple<std::string,std::string,std::string> result;
    rapidjson::Document json_data;
    //解析客户端发来的json数据
    if(json_data.Parse(rece_data).HasParseError()||(!json_data.IsObject())){
        debug("update client info:%s",GetParseError_En(json_data.GetParseError()));
        return result;
    }
    if (json_data.HasMember("source_account") && json_data.HasMember("target_account") && json_data.HasMember("message") &&
        json_data["source_account"].IsString() && json_data["target_account"].IsString() && json_data["message"].IsString())
    {
        result = std::make_tuple(json_data["source_account"].GetString(),json_data["target_account"].GetString(),json_data["message"].GetString());
    }
    return result;   
}

void UDP::starter(){
    std::thread fail_thread([&](){
        fail_message();
    });
    fail_thread.detach();
    std::thread listener_thread([&](){
        while(true){
            char buffer[BUFSIZ];
            //清空buffer，防止接收到的数据后面还有上一次接收的部分数据
            memset(buffer,0,BUFSIZ);
            struct sockaddr_in client_addr;
            socklen_t client_addr_length = sizeof(client_addr);
            
            ssize_t rece_bytes = ::recvfrom(udp_socket,buffer,sizeof(buffer),0,(struct sockaddr*)&client_addr,&client_addr_length);
            if(rece_bytes == -1){
                error("recefrom error:%s",strerror(errno));
            }
            //判断接收的数据大小是否符合预期，防止数据包传输过程错误
            if(rece_bytes >= BUFSIZ){
                error("rece_data lenth greather to rece_bytes ");
                continue;
            }
            std::string client_ip = inet_ntoa(client_addr.sin_addr);
            int port = ntohs(client_addr.sin_port);
            //解析json数据,如果不是json数据，则更新client_info中的udp列值，是的话则进行消息转发
            std::tuple<std::string,std::string,std::string> json_data = parse_json_data(buffer);
            if(std::get<0>(json_data).empty()&&std::get<1>(json_data).empty()&&std::get<2>(json_data).empty()){
                //std::cout<<"----"<<buffer<<"----"<<client_ip<<"----"<<port<<"----";
                DB_Mysql::instance()->updata_client_info(buffer,client_ip,port);
            }else{
                std::string send_account = std::get<0>(json_data);
                std::string rece_account = std::get<1>(json_data);
                std::string message = std::get<2>(json_data);
                std::pair<std::string,int> info = DB_Mysql::instance()->get_target_ip(rece_account);
                bool ret_send = send(send_account,rece_account,message,info.first,info.second);
            }
        }});
    //分离线程，当主线程结束时，分离的线程如果仍在运行，则是未定义行为，它可能在主线程结束后继续运行，也可能终止
    listener_thread.detach();
}

UDP::UDP():udp_socket(socket(AF_INET, SOCK_DGRAM, 0)){
    if (udp_socket == -1)
    {
        error("socket error:%s",strerror(errno));
    }
};
UDP::~UDP(){
    close(udp_socket);
}
