#ifndef CHAT_BUBBLE_H
#define CHAT_BUBBLE_H

#include <QWidget>

namespace Ui {
class chat_bubble;
}

class chat_bubble : public QWidget
{
    Q_OBJECT

public:
    explicit chat_bubble(QWidget *parent = nullptr);
    chat_bubble(QString head_path,QString message);
    ~chat_bubble();

private:
    Ui::chat_bubble *ui;
};

#endif // CHAT_BUBBLE_H
