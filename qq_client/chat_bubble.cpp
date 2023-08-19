#include "chat_bubble.h"
#include "ui_chat_bubble.h"

chat_bubble::chat_bubble(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chat_bubble)
{
    ui->setupUi(this);
}

chat_bubble::chat_bubble(QString head_path,QString message):ui(new Ui::chat_bubble){
    ui->setupUi(this);
    ui->Icon->setPixmap(head_path);
    ui->Icon->setMask(QRegion(ui->Icon->rect(),QRegion::RegionType::Ellipse));
    ui->chatHistory->setText(message);
}

chat_bubble::~chat_bubble()
{
    delete ui;
}
