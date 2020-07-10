#ifndef CHESSFORM_H
#define CHESSFORM_H

#include <QWidget>
#include "chess.h"
#include <QPainter>
#include <QStringList>
#include <QMessageBox>
#include <QDebug>
#include <QPalette>
#include <QFont>
#include <QHostInfo>

//network
#include <QUdpSocket>

namespace Ui {
class ChessForm;
}

class ChessForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChessForm(QWidget *parent = nullptr);
    ~ChessForm();

protected:
    void paintEvent(QPaintEvent *event) ;
    void closeEvent(QCloseEvent*);

private slots:

    void on_P_VS_P_clicked();

    void on_P_VS_C_clicked();

    void doProcessChessData(int i,int j);

    void on_N_VS_N_clicked();

    void doProcessReadyRead();
    void doProcessError(QAbstractSocket::SocketError);

    void on_btn_bind_clicked();

    void on_pushButton_clicked();

private:
    Ui::ChessForm *ui;    
    enum PKType{PVP, PVC, NVN};

    PKType currentPK;                                                                     //当前游戏模式
    chess *myChess;
    int formChessData[8][8];                                                              //保存棋子数据的容器
    chess::ChessType currentRole;                                                         //当前谁先下子
    void Init();                                                                          //初始化函数
    int judgeRule(int x, int y, void *chess, chess::ChessType currentRole, bool eatChess);//吃子规则
    void RoleInit(const QString whiteFilename, const QString blackFilename);              //黑子先还是白子先
    void setRole(chess::ChessType role);                                                  //设置谁先下
    void setChessInit();                                                                  //对棋盘初始化
    void RoleChange();                                                                    //角色切换
    void ChessShow();                                                                     //数据统计
    void RebootRole(chess::ChessType role);                                               //机器下子


    QUdpSocket* mySocket;
    bool isPeople;
    bool isDown;

};

#endif // CHESSFORM_H
