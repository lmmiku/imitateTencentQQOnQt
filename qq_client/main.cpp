#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowIcon(QIcon(":/res/title_Icon.jpg"));
    w.show();
    return a.exec();
}
