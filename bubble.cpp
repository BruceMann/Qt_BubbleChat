#include "bubble.h"
#include <QFontMetrics>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

Bubble::Bubble(QWidget *parent):QWidget(parent)
{
    QFont te_font = this->font();
    te_font.setFamily("MicrosoftYaHei");
    te_font.setPointSize(12);
    this->setFont(te_font);

    TrianglePoints[3] = {QPoint(0,0)};

     initPaintParam();
}

Bubble::~Bubble()
{

}

QSize Bubble::calTextSize(QString src)
{
    QFontMetricsF fm(this->font());
    int textWdith = this->width() - 2*(ContentHorMargin+IconWidth+IconHorMargin+TriangleMargin+TriangleWidth);
    int lineHeight = fm.lineSpacing();
    int nCount = src.count("\n");
    int nMaxWidth = 0;
    if(nCount == 0) {
        nMaxWidth = fm.width(src);
        QString value = src;
        if(nMaxWidth > textWdith) {
            nMaxWidth = textWdith;
            int size = textWdith / fm.width(" ");
            int num = fm.width(value) / textWdith;
            int ttmp = num*fm.width(" ");
            num = ( fm.width(value) ) / textWdith;
            nCount += num;
            QString temp = "";
            for(int i = 0; i < num; i++) {
                temp += value.mid(i*size, (i+1)*size) + "\n";
            }
            src.replace(value, temp);
        }
    } else {
        for(int i = 0; i < (nCount + 1); i++) {
            QString value = src.split("\n").at(i);
            nMaxWidth = fm.width(value) > nMaxWidth ? fm.width(value) : nMaxWidth;
            if(fm.width(value) > textWdith) {
                nMaxWidth = textWdith;
                int size = textWdith / fm.width(" ");
                int num = fm.width(value) / textWdith;
                num = ((i+num)*fm.width(" ") + fm.width(value)) / textWdith;
                nCount += num;
                QString temp = "";
                for(int i = 0; i < num; i++) {
                    temp += value.mid(i*size, (i+1)*size) + "\n";
                }
                src.replace(value, temp);
            }
        }
    }
    return QSize(nMaxWidth, (nCount + 1) * lineHeight);
}

QSize Bubble::calImageSize(QString content)
{
    QPixmap pix(content);
    QSize size = pix.rect().size();
    size = size.width()*size.height()>320*480?QSize(480,320):size;
    return size;
}

QSize Bubble::BubbleSize(QString content)
{
    m_content = content;
    QSize size = calContentSize(content);
    ContentHeight = size.height();
    ContentWidth = size.width();
    BubbleWidth = ContentWidth + 2*ContentHorMargin;
    BubbleHeight = ContentHeight + 2*ContentVerMargin;

    int wid = BubbleWidth+2*(IconWidth+IconHorMargin+TriangleMargin+TriangleWidth);
    int hei =  BubbleHeight +2* BubbleVerMargin;
    if(hei<minBubbleHeight){
        hei = minBubbleHeight;
    }

    return QSize(wid,hei);
}

void Bubble::PrepareDrawingParameters()
{
    if(m_userType == User_She){
        IconPixmap = QPixmap(":/images/Customer Copy.png");
        IconRect = QRect(IconVerMargin,IconHorMargin,IconWidth,IconHeight);
        TriangleRect = QRect(IconRect.x()+IconRect.width(),0,TriangleMargin+TriangleWidth,this->height());
        TrianglePoints[0] = QPoint(TriangleRect.x()+TriangleMargin,30);
        TrianglePoints[1] = QPoint(TriangleRect.x()+TriangleMargin+TriangleWidth,25);
        TrianglePoints[2] = QPoint(TriangleRect.x()+TriangleMargin+TriangleWidth,35);
        BubbleBorderColor = QColor(0,255,0);
        BubbleColor = QColor(234, 0, 0);
        BubbleRect.setRect(TriangleRect.x()+TriangleRect.width(),BubbleHorMargin,BubbleWidth,BubbleHeight);
        ContentRect.setRect(BubbleRect.x()+ContentHorMargin,BubbleRect.y()+ContentVerMargin,ContentWidth,ContentHeight);
    }else if(m_userType == User_Me){
        IconPixmap = QPixmap(":/images/CustomerService.png");
        IconRect = QRect(this->width() - IconVerMargin - IconWidth ,IconHorMargin,IconWidth,IconHeight);
        TriangleRect = QRect(IconRect.x()-TriangleMargin-TriangleWidth,0,TriangleMargin+TriangleWidth,this->height());
        TrianglePoints[0] = QPoint(TriangleRect.x()  ,25);
        TrianglePoints[1] = QPoint(TriangleRect.x()  ,35);
        TrianglePoints[2] = QPoint(TriangleRect.x() + TriangleWidth,30);
        BubbleRect.setRect(TriangleRect.x() - BubbleWidth,BubbleHorMargin,BubbleWidth,BubbleHeight);
        BubbleBorderColor = QColor(0,255,255);
        BubbleColor = QColor(75,164,242);
        ContentRect.setRect(BubbleRect.x()+ContentHorMargin,BubbleRect.y()+ContentVerMargin,ContentWidth,ContentHeight);
    }else if(m_userType == User_Time){

    }

    this->update();
}

QSize Bubble::calContentSize(QString content)
{
    if(m_contentType == Content_Text){
        return calTextSize(content);
    }else if(m_contentType == Content_Image){
        return calImageSize(content);
    }
    return QSize();
}

void Bubble::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);//消锯齿
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::gray));

    //Icon
    painter.drawPixmap(IconRect,IconPixmap);

    //bubble border
    painter.setBrush(QBrush(BubbleBorderColor));
    painter.drawRoundedRect(BubbleRect,BubbleRadiusX,BubbleRadiusY);

    //bubble sharp corner
    QPen pen;
    pen.setColor(BubbleBorderColor);
    painter.setPen(pen);
    painter.drawPolygon(TrianglePoints,3);

    //content
    if(m_contentType==Content_Text){
        QPen penText;
        penText.setColor(QColor(51,51,51));
        painter.setPen(penText);
        QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        painter.setFont(this->font());
        painter.drawText(ContentRect, m_content,option);
    }else if(m_contentType==Content_Image){
        QPixmap image(m_content);
        painter.drawPixmap(ContentRect,image);
    }
}

void Bubble::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        qDebug()<<m_content;
    }
}

void Bubble::initPaintParam()
{
    minBubbleHeight = 80;
    BubbleHorMargin = BubbleVerMargin = 20;
    IconWidth = IconHeight = 40;
    IconHorMargin = IconVerMargin = 20;
    TriangleWidth = 6;
    TriangleMargin = 5;
    BubbleHeight = BubbleWidth = 20;
    BubbleRadiusX = BubbleRadiusY = 4;
    ContentVerMargin = ContentHorMargin =12;
    ContentWidth = ContentHeight = 18;
}

