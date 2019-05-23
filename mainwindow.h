#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "qnchatmessage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString content,QNChatMessage::Content_Type contentType, QString time, QNChatMessage::User_Type type);
    void dealMessageTime(QString curMsgTime);
protected:
    void resizeEvent(QResizeEvent *event);
private slots:
    void on_pushButton_clicked();

    void on_pbBubbleSend_clicked();

    void on_MeSend_clicked();

    void on_pbSheSendPic_clicked();

    void on_pbMeSendPic_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

