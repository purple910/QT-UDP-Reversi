#include "chess.h"

chess::chess(QWidget *parent)
    : QWidget(parent)
{
    Init();
    InitChess();
}

chess::~chess()
{
}

//------------------events-------------------
void chess::paintEvent(QPaintEvent *){
    QPainter painter(this);
    //画背景
    QRect rec(QPoint(0,0),QPoint(this->width(),this->height()));
//    QRect rec;
//    rec.setTopLeft(QPoint(0,0));
//    rec.setBottomRight(QPoint(this->width(),this->height()));
    QPixmap pix(bgFilename);
//    QPixmap pix;
//    pix.load("../15myChess/image/1.jpg");
    painter.drawPixmap(rec,pix);

    //画线
    QPen pen;
    pen.setColor(lineColor);
    pen.setStyle(lineStyle);
    pen.setWidth(lineWidth);
    painter.setPen(pen);
    for (int i=0; i<=8; i++) {
        //x
        painter.drawLine(startX,startY+i*gridHeight,gridWidth*9,startY+i*gridHeight);
        //y
        painter.drawLine(startX*i+startX,startY,gridWidth*i+startX,9*gridHeight);
    }

    //画棋子
    QString chessFileName;
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            if(chessDate[i][j] == White){
                chessFileName = ":/new/prefix1/image/wc.png";
            }
            else if (chessDate[i][j] == Black) {
                chessFileName = ":/new/prefix1/image/bc.png";
            }
            else {
                chessFileName.clear();
                continue;
            }
            painter.drawPixmap(startX+i*gridWidth,startY+j*gridHeight,gridWidth,gridHeight,QPixmap(chessFileName));

        }

    }

}
void chess::resizeEvent(QResizeEvent *event){
    gridWidth = event->size().width() / GRIDCOUNT;
    gridHeight = event->size().height() / GRIDCOUNT;
    startX = gridWidth;
    startY = gridHeight;
}
void chess::mousePressEvent(QMouseEvent *event){
    int x = event->x();
    int y = event->y();
    //x
    if(x >= startX && (x <= startX + 8*gridWidth)){
        //y
        if(y >= startY && (y <= startY + 9*gridHeight)){
            int i=0,j=0;
            i=(x-startX)/gridWidth;
            j=(y-startY)/gridHeight;
//            chessDate[i][j] = Black;
//            this->update();
            SignalSendChessDate(i,j);
        }
    }
}
//------------------end event-------------------


//------------------functions-------------------
//private
void chess::Init(){
    bgFilename.clear();
    bgFilename = ":/new/prefix1/image/bk.jpg";
    lineColor = QColor(255,128,255);
    lineStyle = Qt::SolidLine;
    lineWidth = 3;
}
void chess::InitChess(){
    //初始化棋盘数据
    memset(chessDate,0,sizeof (int)*64);
//    chessDate[3][3]=Black;
//    chessDate[4][3]=White;
//    chessDate[3][4]=White;
//    chessDate[4][4]=Black;
}
//public
void chess::setBackGroundImage(const QString filename){
    this->bgFilename = filename;
    this->update();
}
void chess::setLine(const QColor color, const Qt::PenStyle style, const int width){
    this->lineColor = color;
    this->lineStyle = style;
    this->lineWidth = width;
    this->update();
}
void chess::setChessStatus(void *p){
    memcpy(chessDate,p,sizeof (int)*64);
    this->update();
}
//------------------end function-------------------


//------------------slots-------------------

//------------------end slots-------------------
