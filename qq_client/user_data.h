#ifndef USER_DATA_H
#define USER_DATA_H
#include <QMap>
#include <QDir>
#include <QPair>
#include <QFile>
#include <QList>
#include <QDebug>
#include <QString>
#include <QSqlError>
#include <QMessageBox>
#include <QStringList>
#include <QTextStream>
#include <QErrorMessage>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QCoreApplication>
#include <QtSql/QSqlDatabase>

class user_data
{
public:
    user_data();

    //添加用户
    bool add_username(QString account,QString username,QString password);

    //遍历文件数据
    QList<QPair<QString,QString>> view_username();

    //查找
    bool find_account(QString account);
    bool find_username(QString account,QString password);

    //修改
    bool change_password(QString account,QString newpassword);
    bool change_username(QString account,QString newusername);

    //获取用户名
    QString get_username(QString account);

    //保存联系人信息
    bool save_friend(QString account,QString friend_account,QString type);

    //是否保存密码
    bool save_password(QString account,bool state);

    //查看是否选择保存密码
    bool password_state(QString account);

    //取出联系人信息
    QList<QPair<QString,QString>> get_friend(QString account,QString type);

    //查找指定联系人
    QString find_friend_account(QString account,QString friend_account);

    //删除指定好友
    bool delete_friend(QString account,QString friend_account);

    //注销用户
    bool delete_user(QString account);

    //更新用户状态
    bool updata_user(QString account,QString state);

    //获取用户状态
    QString getstate_user(QString account);

    //获取用户个性签名
    QString get_signature(QString account);

    //保存用户个性签名
    bool save_signature(QString account,QString signature);

    //保存头像路径
    bool save_headPath(QString account,QString head_path);

    //取出头像路径
    QString get_headPath(QString account);

    //取出最后一条聊天记录
    QString get_lastHistory(QString account,QString frien_account);

    //获取聊天记录
    QList<QPair<QString,QString>> get_ALLChat(QString sender_account,QString receiver_account);

    //保存有聊天的好友
    bool save_chat(QString account,QString friend_account);

    //读取有聊天记录的好友
    QList<QPair<QString,QString>> get_chat(QString account);

    //查看聊天记录是否存在
    bool is_existChat(QString account,QString friend_account);

    //删除所有聊天记录
    bool delete_chatHistory(QString account,QString friend_account);

    //从会话列表移除
    bool delete_chat(QString account,QString friend_account);

    QSqlQuery query;


};

#endif // USER_DATA_H
