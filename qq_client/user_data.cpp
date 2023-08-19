#include "user_data.h"

user_data::user_data()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("120.53.84.94");
    db.setUserName("root");
    db.setPassword("12345678");
    db.setPort(3306);
    db.setDatabaseName("user_data");
    //设置数据库连接过期时间
    QString connectionOptions = QString("ConnectTimeout=3600;");
    if(!db.open()){
        QMessageBox::warning(NULL,"错误","数据库打开失败",QMessageBox::Ok);
        qDebug()<<"error open database because"<<db.lastError().text();
    }
    db.setConnectOptions(connectionOptions);
    query=QSqlQuery(db);
}

//实现添加新用户
bool user_data::add_username(QString account,QString username,QString password){
    //账号是否规范
    if(account == "" || username == "" || password == ""){
        QErrorMessage *dialog1 = new QErrorMessage();
        dialog1->setWindowTitle("注册失败");
        dialog1->showMessage("请正确输入！");
        return false;
    }else{
        for(auto s:account){
            if(s<'0'||s>'9'){
                //qDebug()<<"注册失败";
                QErrorMessage *dialog2 = new QErrorMessage();
                dialog2->setWindowTitle("注册失败");
                dialog2->showMessage("请输入正确的账号！");
                return false;
            }
        }
    }

    //查找账号是否存在
    if(find_account(account)){
        QMessageBox::information(NULL,"失败","注册失败，账号已存在",QMessageBox::Ok);
        return false;
    }

    QString s_insert = QString("insert into data(account,username,password,state) values('%1','%2','%3','%4')").arg(account).arg(username).arg(password).arg("离线");
    bool is_success = query.exec(s_insert);
    if(is_success){
        QMessageBox::information(NULL,"注册成功","注册成功，点击ok返回登录界面",QMessageBox::Ok);
        return true;
    }else{
        QMessageBox::information(NULL,"注册失败","数据库插入数据失败,点击ok继续注册",QMessageBox::Ok);
        return false;
    }
    //qDebug()<<"注册成功"<<"用户名："<<username.toUtf8().data()<<"密码："<<password.toUtf8().data();
}

//遍历用户数据
QList<QPair<QString,QString>> user_data::view_username(){
    QList<QPair<QString,QString>> List_account;
    QString s_view = QString("SELECT account,password FROM data");
    //bool is_success = query.exec(s_view);
    //qDebug()<<is_success;
    query.exec(s_view);
    while(query.next()){
        QPair<QString,QString> temp_pair;
        //qDebug()<<"acount:"<<query.value("account").toString()<<"passowrd:"<<query.value("password").toString();
        temp_pair.first = query.value(0).toString();
        temp_pair.second = query.value(1).toString();
        List_account.append(temp_pair);
    }
    return List_account;
}

//实现查找
bool user_data::find_account(QString account){
    QString s_select = QString("SELECT * FROM data WHERE account='%1'").arg(account);
    query.exec(s_select);
    if(query.next()){
//        QString str_account = query.value(0).toString();
//        qDebug()<<str_account;
        return true;
    }else{
        return false;
    }
}

bool user_data::find_username(QString account, QString password){
    bool ret = false;
    QString s_select = QString("SELECT * FROM data WHERE account = '%1'").arg(account);
    query.exec(s_select);
    if(query.next()){
        //判断账号与密码是否对应
        if(query.value(0) == account && query.value(2) == password){
            ret = true;
        }
    }
    return ret;
}

//实现修改密码
bool user_data::change_password(QString account,QString newpassword){
    QString s_update = QString("UPDATE data SET password='%1' WHERE account = '%2'").arg(newpassword).arg(account);
    return query.exec(s_update);
}

//实现修改用户名
bool user_data::change_username(QString account,QString newusername){
    QString s_update = QString("UPDATE data SET username='%1' WHERE account = '%2'").arg(newusername).arg(account);
    return query.exec(s_update);
}

//实现获取用户名
QString user_data::get_username(QString account){
    QString s_select = QString("SELECT * FROM data WHERE account = '%1'").arg(account);
    //qDebug()<<query.exec(s_select);
    query.exec(s_select);
    if(query.next()){
        return query.value(1).toString();
    }
    return "ERROR";
}

//实现保存联系人信息
bool user_data::save_friend(QString account,QString friend_account,QString type){
    QString friend_username = get_username(friend_account);
    QString s_save = QString("insert into friend(account,friend_account,friend_username,type) values('%1','%2','%3','%4')").arg(account).arg(friend_account).arg(friend_username).arg(type);
    return query.exec(s_save);
}

//实现取出联系人信息
QList<QPair<QString,QString>> user_data::get_friend(QString account,QString type){
    QList<QPair<QString,QString>> L;
    QString s_select = QString("select friend_account,friend_username from friend where type = '%1' AND account = '%2'").arg(type).arg(account);
    query.exec(s_select);
    while(query.next()){
        QPair<QString,QString> temp_pair;
        temp_pair.first = query.value(0).toString();
        temp_pair.second = query.value(1).toString();
        L.push_back(temp_pair);
        //qDebug()<<"account:"<<temp_pair.first<<"username:"<<temp_pair.second;
    }
    return L;
}

//实现查找联系人
QString user_data::find_friend_account(QString account,QString friend_account){
    QString s_select = QString("SELECT friend_username FROM friend WHERE account = '%1' AND friend_account = '%2'").arg(account).arg(friend_account);
    query.exec(s_select);
    if(query.next()){
        //qDebug()<<"查找成功";
        return query.value(0).toString();
    }else{
        //qDebug()<<"查找失败";
        return "";
    }
}

//实现删除好友
bool user_data::delete_friend(QString account,QString friend_account){
    QString s_delete = QString("delete from friend where account = '%1' AND friend_account = '%2'").arg(account).arg(friend_account);
    return query.exec(s_delete);
}

//实现注销用户
bool user_data::delete_user(QString account){
    QString s_delete_friend = QString("delete from friend where account = '%1'").arg(account);
    bool ret1 = query.exec(s_delete_friend);
    QString s_delete_user = QString("delete from data where account = '%1'").arg(account);
    bool ret2 = query.exec(s_delete_user);
    return ret1&ret2;
}

//实现是否保存密码
bool user_data::save_password(QString account,bool state){
    QString s_update = QString("UPDATE data SET password_state = '%1' WHERE account = '%2'").arg(state).arg(account);
    return query.exec(s_update);
}

//实现查看是否选择保存密码
bool user_data::password_state(QString account){
    QString s_select = QString("SELECT password_state FROM data WHERE account = '%1'").arg(account);
    query.exec(s_select);
    if(query.next()){
        return query.value(0).toBool();
    }
    return false;
}

//实现更新用户状态
bool user_data::updata_user(QString account,QString state){
    QString s_updata = QString("UPDATE data SET state='%1' WHERE account = '%2'").arg(state).arg(account);
    return query.exec(s_updata);
}

//获取联系人状态
QString user_data::getstate_user(QString account){
    QString s_select = QString("SELECT state FROM data WHERE account='%1'").arg(account);
    query.exec(s_select);
    if(query.next()){
        return query.value(0).toString();
    }else{
        return "";
    }
}

//设置用户个性签名
bool user_data::save_signature(QString account,QString signature){
    QString s_updata = QString("UPDATE data SET signature='%1' WHERE account='%2'").arg(signature).arg(account);
    return query.exec(s_updata);
}

//获取用户个性签名
QString user_data::get_signature(QString account){
    QString s_select = QString("SELECT signature FROM data WHERE account='%1'").arg(account);
    query.exec(s_select);
    if(query.next()){
        return query.value(0).toString();
    }else{
        return "";
    }
}

//保存头像路径
bool user_data::save_headPath(QString account,QString head_Path){
    QString s_save = QString("UPDATE data SET head='%1' WHERE account='%2'").arg(head_Path).arg(account);
    return query.exec(s_save);
}

//获取头像路径
QString user_data::get_headPath(QString account){
    QString s_select = QString("SELECT head FROM data WHERE account='%1'").arg(account);
    query.exec(s_select);
    if(query.next()){
        //qDebug()<<"---"<<query.value("head").toString()<<"----";
        return query.value(0).toString();
    }
    return "";
}

//获取最后一条聊天记录
QString user_data::get_lastHistory(QString account,QString friend_account){
    QString s_select = QString("SELECT message FROM chat_history WHERE (sender_account = '%1' AND receiver_account = '%2') OR (sender_account = '%3' AND receiver_account = '%4') ORDER BY timestamp DESC LIMIT 1").arg(account).arg(friend_account).arg(friend_account).arg(account);
    //qDebug()<<s_select;
    query.exec(s_select);
    if(query.next()){
        //qDebug()<<"-----"<<query.value(0).toString()<<"-----";
        return query.value(0).toString();
    }
    //query.lastError().text();
    return "";
}

//获取聊天记录
QList<QPair<QString,QString>> user_data::get_ALLChat(QString account,QString friend_account){
    QList<QPair<QString,QString>> L;
    QString s_select = QString("SELECT sender_account,message FROM chat_history WHERE (sender_account = '%1' AND receiver_account = '%2') OR (sender_account = '%3' AND receiver_account = '%4') ORDER BY timestamp ASC").arg(account).arg(friend_account).arg(friend_account).arg(account);
    query.exec(s_select);
    while(query.next()){
        QPair<QString,QString> temp_pair;
        temp_pair.first = query.value(0).toString();
        temp_pair.second = query.value(1).toString();
        L.append(temp_pair);
    }
    return L;
}

//查看聊天记录是否存在
bool user_data::is_existChat(QString account,QString friend_account){
    QString s_select = QString("SELECT * FROM chat WHERE account = '%1' AND friend_account = '%2'").arg(account).arg(friend_account);
    query.exec(s_select);
    if(query.next()){
        qDebug()<<"聊天记录不存在";
        return true;
    }
    qDebug()<<"聊天记录存在";
    return false;
}

//保存有聊天的好友
bool user_data::save_chat(QString account,QString friend_account){
    if(!is_existChat(account,friend_account)){
        QString s_update = QString("INSERT INTO chat (account,friend_account) VALUES('%1','%2')").arg(account).arg(friend_account);
        return query.exec(s_update);
    }
    return false;
}

//读取有聊天记录的好友
QList<QPair<QString,QString>> user_data::get_chat(QString account){
    QList<QPair<QString,QString>> L;
    QString s_select = QString("SELECT account,friend_account FROM chat WHERE account = '%1'").arg(account);
    query.exec(s_select);
    while(query.next()){
        QPair<QString,QString> temp_pair;
        temp_pair.first = query.value(0).toString();
        temp_pair.second = query.value(1).toString();
        L.append(temp_pair);
    }
    return L;
}

//删除所有聊天记录
bool user_data::delete_chatHistory(QString account,QString friend_account){
    QString s_delete = QString("DELETE FROM chat_history WHERE sender_account = '%1' AND receiver_account = '%2'").arg(account).arg(friend_account);
    return query.exec(s_delete);
}

//从会话列表移除
bool user_data::delete_chat(QString account,QString friend_account){
    QString s_delete = QString("DELETE FROM chat WHERE account = '%1' AND friend_account = '%2'").arg(account).arg(friend_account);
    return query.exec(s_delete);
}
