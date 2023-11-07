#include "widget.h"
#include <QApplication>
#include <QPropertyAnimation>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowIcon(QIcon(":/res/title_Icon.jpg"));
    QPropertyAnimation *animation= new QPropertyAnimation(&w,"geometry");
    QScreen* screen = QGuiApplication::primaryScreen();
    animation->setDuration(300);
    animation->setStartValue(QRect(w.x()+screen->geometry().width()/3, w.y()+screen->geometry().height()/4, w.width(),0));
    animation->setEndValue(QRect(w.x()+screen->geometry().width()/3, w.y()+screen->geometry().height()/4, w.width(),w.height()));
    animation->setEasingCurve(QEasingCurve::InQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    w.show();
    return a.exec();
}
