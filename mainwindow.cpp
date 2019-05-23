#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include "bubble.h"

QString sourcepath("D:/testImage/20180505215402.jpg");
//QString sourcepath("D:/testImage/c05b4755fc1f200de9c71c26fc60272b.jpg");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton->setHidden(true);
    resize(1000, 800);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString msg = ui->textBrowser->toPlainText();
    ui->textBrowser->setText("");
    QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳

    bool isSending = true; // 发送中

    qDebug()<<"addMessage" << msg << time << ui->listWidget->count();
    if(ui->listWidget->count()%2==0) {
        if(isSending) {
            dealMessageTime(time);

//            QString img_path = QString("<img src=\"%1\"/>").arg(sourcepath);
//            ui->textBrowser->insertHtml(img_path);

            QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget());
            messageW->setImagePath(sourcepath);
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            dealMessage(messageW, item, sourcepath,QNChatMessage::Content_Image, time, QNChatMessage::User_Me);
        } else {
            bool isOver = true;
            for(int i = ui->listWidget->count() - 1; i > 0; i--) {
                QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(ui->listWidget->item(i));
                if(messageW->text() == msg) {
                    isOver = false;
                    messageW->setTextSuccess();
                }
            }
            if(isOver) {
                dealMessageTime(time);

                QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget());
                QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
                dealMessage(messageW, item, msg,QNChatMessage::Content_Text, time, QNChatMessage::User_Me);
                messageW->setTextSuccess();
            }
        }
    } else {
        if(msg != "") {
            dealMessageTime(time);

            QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget());
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            dealMessage(messageW, item, msg,QNChatMessage::Content_Text, time, QNChatMessage::User_She);
        }
    }
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
}

void MainWindow::dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString content, QNChatMessage::Content_Type contentType, QString time,  QNChatMessage::User_Type type)
{
    messageW->setFixedWidth(this->width());
    QSize size = messageW->fontRect(content,contentType);
    item->setSizeHint(size);
    messageW->setText(content, time, size, type);
    ui->listWidget->setItemWidget(item, messageW);
}

void MainWindow::dealMessageTime(QString curMsgTime)
{
    bool isShowTime = false;
    if(ui->listWidget->count() > 0) {
        QListWidgetItem* lastItem = ui->listWidget->item(ui->listWidget->count() - 1);
        QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(lastItem);
        int lastTime = messageW->time().toInt();
        int curTime = curMsgTime.toInt();
        qDebug() << "curTime lastTime:" << curTime - lastTime;
        isShowTime = ((curTime - lastTime) > 60); // 两个消息相差一分钟
//        isShowTime = true;
    } else {
        isShowTime = true;
    }
    if(isShowTime) {
        QNChatMessage* messageTime = new QNChatMessage(ui->listWidget->parentWidget());
        QListWidgetItem* itemTime = new QListWidgetItem(ui->listWidget);

        QSize size = QSize(this->width(), 40);
        messageTime->resize(size);
        itemTime->setSizeHint(size);
        messageTime->setText(curMsgTime, curMsgTime, size, QNChatMessage::User_Time);
        ui->listWidget->setItemWidget(itemTime, messageTime);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    ui->textBrowser->resize(this->width() - 20, ui->widget->height() - 20);
    ui->textBrowser->move(10, 10);

    ui->pushButton->move(ui->textBrowser->width()+ui->textBrowser->x() - ui->pushButton->width() - 10,
                         ui->textBrowser->height()+ui->textBrowser->y() - ui->pushButton->height() - 10);

    for(int i = 0; i < ui->listWidget->count(); i++) {
        QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(ui->listWidget->item(i));
        QListWidgetItem* item = ui->listWidget->item(i);

        dealMessage(messageW, item, messageW->text(),QNChatMessage::Content_Text, messageW->time(), messageW->userType());
    }
}


void MainWindow::on_pbBubbleSend_clicked()
{
    QString msg = ui->textBrowser->toPlainText();
    ui->textBrowser->setText("");

    Bubble* bubble = new Bubble(ui->listWidget->parentWidget());
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    bubble->setFixedWidth(this->width());
    bubble->SetContentType(Bubble::Content_Text);
    bubble->SetUserType(Bubble::User_She);
    QSize size = bubble->BubbleSize(msg);
    qDebug()<<size;
    item->setSizeHint(size);
    bubble->PrepareDrawingParameters();
    ui->listWidget->setItemWidget(item,bubble);
}

void MainWindow::on_MeSend_clicked()
{
    QString msg = ui->textBrowser->toPlainText();
    ui->textBrowser->setText("");

    Bubble* bubble = new Bubble(ui->listWidget->parentWidget());
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    bubble->setFixedWidth(this->width());
    bubble->SetContentType(Bubble::Content_Text);
    bubble->SetUserType(Bubble::User_Me);
    QSize size = bubble->BubbleSize(msg);
    qDebug()<<size;
    item->setSizeHint(size);
    bubble->PrepareDrawingParameters();
    ui->listWidget->setItemWidget(item,bubble);
}

void MainWindow::on_pbSheSendPic_clicked()
{
    QString shePic("D:/testImage/20180505215402.jpg");
    Bubble* bubble = new Bubble(ui->listWidget->parentWidget());
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    bubble->setFixedWidth(this->width());
    bubble->SetContentType(Bubble::Content_Image);
    bubble->SetUserType(Bubble::User_She);
    QSize size = bubble->BubbleSize(shePic);
    qDebug()<<size;
    item->setSizeHint(size);
    bubble->PrepareDrawingParameters();
    ui->listWidget->setItemWidget(item,bubble);
}

void MainWindow::on_pbMeSendPic_clicked()
{
    QString mePic("D:/testImage/c05b4755fc1f200de9c71c26fc60272b.jpg");
    Bubble* bubble = new Bubble(ui->listWidget->parentWidget());
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    bubble->setFixedWidth(this->width());
    bubble->SetContentType(Bubble::Content_Image);
    bubble->SetUserType(Bubble::User_Me);
    QSize size = bubble->BubbleSize(mePic);
    qDebug()<<size;
    item->setSizeHint(size);
    bubble->PrepareDrawingParameters();
    ui->listWidget->setItemWidget(item,bubble);
}
