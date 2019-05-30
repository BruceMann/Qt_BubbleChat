#ifndef BUBBLE_H
#define BUBBLE_H

#include <QWidget>
#include <QPoint>
#include <QColor>
#include <QRect>
#include <QPixmap>

class Bubble:public QWidget
{
    Q_OBJECT
public:
    enum User_Type{
        User_System,
        User_Me,
        User_She,
        User_Time,
    };
    enum Content_Type{
        Content_Null,
        Content_Text ,
        Content_Image,
    };

   explicit Bubble(QWidget* parent=nullptr);
    ~Bubble();

    QSize calContentSize(QString content);
    QSize calTextSize(QString src);
    QSize calImageSize(QString content);
    QSize BubbleSize(QString content);

    void PrepareDrawingParameters();
    void SetContentType(Content_Type type){m_contentType = type;}
    void SetUserType(User_Type type){m_userType = type;}

    void RoundedPixmap(QPixmap &inOutPix, QSize size);

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);

private:
    QString m_content;
    User_Type m_userType = User_System;
    Content_Type m_contentType = Content_Null;

    void initPaintParam();

    //*********** paint parameter ***************
    //icon
    int IconWidth;
    int IconHeight;
    int IconHorMargin;
    int IconVerMargin;
    QRect IconRect;
    QPixmap IconPixmap;

    //bubble sharp corner
    QPoint TrianglePoints[3];
    int TriangleWidth;                                                                       //actually is trangle height
    int TriangleMargin;
    QRect TriangleRect;

    //bubble
    int minBubbleHeight;
    int BubbleHorMargin;
    int BubbleVerMargin;
    int BubbleWidth;
    int BubbleHeight;
    int BubbleRadiusX;
    int BubbleRadiusY;
    QColor BubbleColor;
    QColor BubbleBorderColor;
    QRect BubbleRect;
    //content
    int ContentWidth;
    int ContentHeight;
    int ContentHorMargin;
    int ContentVerMargin;
    QRect ContentRect;
    //*********** paint arg ***************
};

#endif // BUBBLE_H
