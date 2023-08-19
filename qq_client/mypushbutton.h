#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include <QString>
#include <QDebug>

class myPushbutton : public QPushButton
{
public:
    myPushbutton(QString FileName);


    QString FileNamePath;
};

#endif // MYPUSHBUTTON_H
