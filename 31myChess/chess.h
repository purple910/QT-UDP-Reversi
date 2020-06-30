#ifndef CHESS_H
#define CHESS_H

#include <QWidget>
#include <QPainter>
#include <QResizeEvent>
#include <QDebug>
#include <QMouseEvent>

#define GRIDCOUNT 10

class chess : public QWidget
{
    Q_OBJECT

public:
    chess(QWidget *parent = nullptr);
    ~chess();
    //棋子类型
    enum ChessType{Empty=0,White,Black};

    //改变背景图片
    void setBackGroundImage(const QString filename);
    //改变线的属性
    void setLine(const QColor color,const Qt::PenStyle style,const int width);
    //改变棋盘的显示值
    void setChessStatus(void *p);


protected:
    void paintEvent(QPaintEvent *event) ;
    void resizeEvent(QResizeEvent *event) ;
    void mousePressEvent(QMouseEvent *event) ;

private:
    QString bgFilename;
    QColor lineColor;
    Qt::PenStyle lineStyle;
    int lineWidth;
    //格子宽高，起始点
    int gridWidth,gridHeight,startX,startY;
    //保存棋子数据的容器
    int chessDate[8][8];

    void Init();
    //初始化棋盘
    void InitChess();

signals:
    //发送棋子信息
    void SignalSendChessDate(int i,int j);

};
#endif // CHESS_H
