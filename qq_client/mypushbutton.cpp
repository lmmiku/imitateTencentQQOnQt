#include "mypushbutton.h"

myPushbutton::myPushbutton(QString FileName){
        QPixmap pix;
        FileNamePath=FileName;
        bool flag=pix.load(FileName);
        if(!flag){
            qDebug()<<FileName<<"图片加载失败";
        }
        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px}");        //不规则按钮
    //    this->setMask(pix.mask());
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));
    }
