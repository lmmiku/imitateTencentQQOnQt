#ifndef ENTER_H
#define ENTER_H

#include <QWidget>
#include <QIcon>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QPalette>
#include <QBrush>

QT_BEGIN_NAMESPACE
namespace Ui { class Enter; }
QT_END_NAMESPACE

class Enter : public QWidget
{
    Q_OBJECT

public:
    Enter(QWidget *parent = nullptr);
    ~Enter();

    void paintEvent(QPaintEvent *event);

private:
    Ui::Enter *ui;
};
#endif // ENTER_H
