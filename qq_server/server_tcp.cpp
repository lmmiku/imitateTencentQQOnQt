#include "server_tcp.h"
bool TCP::init_tcp(){
    listen_fd = socket(AF_INET,SOCK_STREAM,0);
    if(listen_fd == -1){
        error("failed to create socket");
        return false;
    }
    //将listen_fd设置为非阻塞模式
    if(!setNonBlock(listen_fd)){
        return false;
    }    
    //端口复用
    int reuse = 1;
    setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    sockaddr_in server_addr{AF_INET,htons(port),htonl(INADDR_ANY)};

    if(bind(listen_fd,(sockaddr*)&server_addr,sizeof(server_addr))==-1){
        error("failed to bind");
        return false;
    }

    if(listen(listen_fd,SOMAXCONN) == -1){
        error("failed to listen");
        return false;
    }

    //创建epoll
    epoll_fd = epoll_create1(0);
    if(epoll_fd == -1){
        error("failed to epoll_create");
    }

    if(!add_epollEvent(listen_fd,EPOLLIN | EPOLLET)){
        return false;
    }

    return true;
}

void TCP::start(){
    epoll_event events[MAX_EVENTS];
    while(true){
        int events_num = epoll_wait(epoll_fd,events,MAX_EVENTS,-1);
        if(events_num == -1){
            error("failed to epoll_wait");
            break;
        }
        //分离线程，当主线程结束时，分离的线程如果仍在运行，则是未定义行为，它可能在主线程结束后继续运行，也可能终止
        for (int i = 0; i < events_num; i++){
            if(events[i].data.fd == listen_fd){
                std::thread accept_(&TCP::accept_connect,this);
                accept_.detach();
            }else{
                std::thread read_([=](){
                    this->read_event(events[i].data.fd);
                });
                read_.detach();
            }
        }
    }
}

bool TCP::setNonBlock(int socket){
    int flags = fcntl(socket,F_GETFL,0);
    if(flags == -1){
        error("failed to get socket flags");
        return false;
    }

    if((fcntl(socket,F_SETFL,flags|O_NONBLOCK) == -1)){
        error("failed to set socket flags");
        return false;
    }
    return true;
}

void TCP::accept_connect(){
    while(true){
        sockaddr_in client_addr{};
        socklen_t client_addr_len = sizeof(client_addr);
        //非阻塞模式
        int client_fd = accept(listen_fd,(sockaddr *)&client_addr,&client_addr_len);
        if(client_fd == -1){
            if(errno == EWOULDBLOCK || errno == EAGAIN){
                break;
            }else{
                error("failed to accpet");
                break;
            }
        }

        do{
            char account_[20];
            int ret_read = read(client_fd,account_,sizeof(account_));
            if(ret_read == -1){
                error("accetp_read to account failed");
            }
            account_[ret_read] = '\0';
            //std::cout<<account_<<std::endl;
            if(!(DB_Mysql::instance()->updata_client_info(std::string(account_),std::string(inet_ntoa(client_addr.sin_addr)),ntohs(client_addr.sin_port),client_fd,"在线"))){
                error("update client_info failed");
            }
            UDP::instance()->cond_var.notify_one();
        }while(0);

        if(!setNonBlock(client_fd)){
            close(client_fd);
            continue;
        }

        if(!add_epollEvent(client_fd,EPOLLIN|EPOLLET)){
            close(client_fd);
            continue;
        }
    }
}

bool TCP::add_epollEvent(int socket,uint32_t events){
    epoll_event event;
    event.data.fd = socket;
    event.events = events;

    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,socket,&event) == -1){
        error("failed to add socekt to epoll");
        return false;
    }
    return true;
}

void TCP::read_event(int socket){
    ssize_t read_bytes;
    char buffer[BUFSIZ];
    while(true){
        read_bytes = read(socket,buffer,BUFSIZ);
        if(read_bytes == -1){
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                //数据全部读取完成
                break;
            }else if(errno == ECONNRESET){
                //客户端意外终止
                DB_Mysql::instance()->setdefault_info(socket);
                close(socket);
                break;
            }else{
                error("failed to read: %s",strerror(errno));
                break;
            }
        }else if(read_bytes == 0){
            //客户端连接关闭
            DB_Mysql::instance()->setdefault_info(socket);
            close(socket);
            //std::cout<<"connect closed"<<std::endl;
            break;
        }
    }
}