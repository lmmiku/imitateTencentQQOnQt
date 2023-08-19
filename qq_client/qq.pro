QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chat_bubble.cpp \
    chat_button.cpp \
    friend_button.cpp \
    main.cpp \
    mainwidget.cpp \
    tcp.cpp \
    udp.cpp \
    user_data.cpp \
    widget.cpp \
    chatbox.cpp

HEADERS += \
    chat_bubble.h \
    chat_button.h \
    friend_button.h \
    mainwidget.h \
    tcp.h \
    udp.h \
    user_data.h \
    widget.h \
    chatbox.h

FORMS += \
    chat_bubble.ui \
    chat_button.ui \
    friend_button.ui \
    mainwidget.ui \
    widget.ui \
    chatbox.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
