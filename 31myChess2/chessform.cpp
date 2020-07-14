#include "chessform.h"
#include "ui_chessform.h"

ChessForm::ChessForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChessForm)
{
    ui->setupUi(this);

    Init();
}

ChessForm::~ChessForm()
{
    delete ui;
}

//-----------------functon--------------------
void ChessForm::Init(){
    isPeople = false;
    isDown = false;
    isUndo = false;
    mySocket = new QUdpSocket(this);
    connect(mySocket,SIGNAL(readyRead()),this,SLOT(doProcessReadyRead()));
    connect(mySocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(doProcessError(QAbstractSocket::SocketError)));

    QStringList array;
    array.append(QString::fromLocal8Bit("白子先"));
    array.append(QStringLiteral("黑子先"));
    ui->comboBox->addItems(array);

    RoleInit(":/new/prefix1/image/wc.png", ":/new/prefix1/image/bc.png");

    currentRole = chess::Empty;
    //申请棋盘类空间
    myChess = new chess();
    connect(myChess, SIGNAL(SignalSendChessDate(int , int )), this, SLOT(doProcessChessData(int,int)));

    //把棋盘显示在当前容器中
    ui->gridLayout->addWidget(myChess);

    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    ui->ip->setText(info.addresses().at(5).toString());
}
/*吃子规则*/
 int ChessForm::judgeRule(int x, int y, void *chess, chess::ChessType currentRole, bool eatChess)
 {
     int dir[8][2] = {{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};//棋盘的八个方向
     int temp_x = x,temp_y = y;                         //临时保存棋盘数组坐标的值
     int i = 0,eatNum = 0;                              //初始化数据
     typedef int (*p)[8];                               //自定义类型
     p ChessFlag = p(chess);                            //类型转换

     if (ChessFlag[temp_x][temp_y] != chess::Empty)     //如果此方格内已经有棋子，返回
         return 0;
     //棋盘的8个方向
     for (i=0; i<8; i++)
     {
         temp_x += dir[i][0];
         temp_y += dir[i][1];                           //准备判断相邻棋子
         //如果没有出界,且相邻棋子是对方的棋子，才有吃子的可能
         if ( (temp_x<8 && temp_x>=0 && temp_y<8 && temp_y>=0) &&
              (ChessFlag[temp_x][temp_y] != currentRole) &&
              (ChessFlag[temp_x][temp_y] != chess::Empty) )
         {
            temp_x += dir[i][0];
            temp_y += dir[i][1];                        //继续判断下一个
            while (temp_x<8 && temp_x>=0 && temp_y<8 && temp_y>=0)
            {
                if (ChessFlag[temp_x][temp_y] == chess::Empty)  //遇到空位跳出
                    break;
                if (ChessFlag[temp_x][temp_y] == currentRole)   //找到自己的棋子，代表可以吃子
                {
                    if (eatChess == true)                       //确定吃子
                    {
                       ChessFlag[x][y] = currentRole;           //开始标记为自己的棋子
                       temp_x -= dir[i][0];
                       temp_y -= dir[i][1];                     //后退一步
                       while ( (temp_x != x) || (temp_y != y) ) //只要没有回到开始的位置就执行
                       {
                           ChessFlag[temp_x][temp_y] = currentRole;  //标志为自己的棋子
                           temp_x -= dir[i][0];
                           temp_y -= dir[i][1];                     //继续后退一步
                           eatNum++;                                //累计
                       }
                    }
                    else                                        //不吃子，只能是判断这个位置能不能吃子
                    {
                        temp_x -= dir[i][0];
                        temp_y -= dir[i][1];                     //后退一步
                        while ( (temp_x != x) || (temp_y != y) ) //只计算可以吃子的个数
                        {
                            temp_x -= dir[i][0];
                            temp_y -= dir[i][1];                     //继续后退一步
                            eatNum++;                                //累计
                        }
                    }
                    break;
                }                                                    //没有找到自己的棋子，就向前走一步
                temp_x += dir[i][0];
                temp_y += dir[i][1];                                 //向前走一步
            }
         }                                                           //如果这个方向不能吃子,就换一个方向
          temp_x = x;
          temp_y = y;
     }
     return eatNum;                                                  //返回能吃子的个数
 }
/*设置黑白棋的棋子显示图片*/
void ChessForm::RoleInit(const QString whiteFilename, const QString blackFilename)
{
     ui->lbl1->setPixmap(QPixmap(whiteFilename));
     ui->lbl1->setScaledContents(true);

     ui->lbl2->setPixmap(QPixmap(blackFilename));
     ui->lbl2->setScaledContents(true);
}
/*设置谁先下*/
void ChessForm::setRole(chess::ChessType role)
{
    currentRole = role;
//    qDebug()<<"chess: "<<role<<endl;
    if (role == chess::Black)
    {
        ui->lbl2->setVisible(true);
        ui->lbl1->setVisible(false);
    }
    else
    {
        ui->lbl2->setVisible(false);
        ui->lbl1->setVisible(true);
    }
    ui->LCD1->display(0);
    ui->LCD2->display(0);
}
/*对棋盘初始化*/
void ChessForm::setChessInit(){
  //初始化棋盘数据
    for (int i=0; i<8; i++)
    {
        for (int j=0; j<8; j++)
        {
          formChessData[i][j] = chess::Empty;
        }
    }
    formChessData[3][3] = chess::Black;
    formChessData[4][3] = chess::White;
    formChessData[3][4] = chess::White;
    formChessData[4][4] = chess::Black;

    myChess->setChessStatus(formChessData);

    ui->LCD1->display(2);
    ui->LCD2->display(2);
}
/*角色切换*/
void ChessForm::RoleChange()
{
    if (currentRole == chess::White)
    {
        currentRole = chess::Black;
    }
    else
    {
        currentRole = chess::White;
    }

    if (currentRole == chess::Black)
    {
        ui->lbl2->setVisible(true);
        ui->lbl1->setVisible(false);
    }
    else
    {
        ui->lbl2->setVisible(false);
        ui->lbl1->setVisible(true);
    }
}
/*统计棋子数量*/
 void ChessForm::ChessShow()
 {
     int whitecount = 0,blackcount = 0;
     for (int i=0; i<8; i++)
    {
         for (int j=0; j<8; j++)
          {
                if (formChessData[i][j] == chess::White)
                {
                     whitecount++;
                }

                if (formChessData[i][j] == chess::Black)
                {
                     blackcount++;
                }
          }
    }
     ui->LCD1->display(whitecount);
     ui->LCD2->display(blackcount);

     if (whitecount == 0)
     {
         QMessageBox::information(this, QStringLiteral("比赛结束"),QStringLiteral("黑棋胜利,重新开始。"));
         setChessInit();
     }

     if (blackcount == 0)
     {
         QMessageBox::information(this, QStringLiteral("比赛结束"),QStringLiteral("白棋胜利,重新开始。"));
         setChessInit();
     }

     int Result = whitecount + blackcount;
     if (Result == 64)
     {
         if (whitecount > blackcount)
         {
            QMessageBox::information(this, QStringLiteral("比赛结束"),QStringLiteral("白棋胜利,重新开始。"));
         }
         else if (whitecount < blackcount)
         {
            QMessageBox::information(this, QStringLiteral("比赛结束"),QStringLiteral("黑棋胜利,重新开始。"));
         }
         else
         {
            QMessageBox::information(this, QStringLiteral("比赛结束"),QStringLiteral("平局,重新开始。"));
         }

         setChessInit();
     }
 }
 /*机器下子*/
  void ChessForm::RebootRole(chess::ChessType role)
  {
      int ret;
      int oldret =  0;
      int temp_i;
      int temp_j;
      int flag = 0;
     //判断能否下子
      for (int i=0; i<8; i++)
      {
          for (int j=0; j<8; j++)
          {
             if ( formChessData[i][j] == chess::Empty)
             {
                 //判断能否吃子
                 ret = judgeRule(i, j, formChessData, role,false);
                 if (ret > oldret)
                 {
                     flag++;
                     temp_i = i;
                     temp_j = j;
                     oldret = ret;
                 }
             }
          }
      }

      if (flag)
      {
          //吃子
         judgeRule(temp_i, temp_j, formChessData, role,true);
          //切换界面显示
         RoleChange();
      }
      else
      {
          RoleChange();
      }
       myChess->setChessStatus(formChessData);
  }
 //---------------------end--------------------


//-----------------events---------------------
 void ChessForm::paintEvent(QPaintEvent *){
     QPainter painter(this);
     painter.drawPixmap(this->rect(),QPixmap(":/new/prefix1/image/1.jpg"));
 }

 void ChessForm::closeEvent(QCloseEvent *event)
 {
     int temp = QMessageBox::warning(this,QStringLiteral("提示"),QStringLiteral("是否退出游戏？"),QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel,QMessageBox::Yes);
     if(temp == QMessageBox::Yes){
         QString myIp = ui->ip->text();
         QString myPort = ui->port->text();
         QString myName = ui->fromname->text();
         //下线数据  unline#name#unlineEnd
         QString msg = QString("unline#%1#unline").arg(myName);
         mySocket->writeDatagram(msg.toUtf8(),QHostAddress(myIp),myPort.toUInt());
     }
     else {
         event->ignore();
     }
 }
//--------------------end---------------------


//------------------slots---------------------
/*人对人*/
void ChessForm::on_P_VS_P_clicked()
{
    currentPK = PVP;
//    qDebug()<<ui->comboBox->currentText().contains(QString::fromLocal8Bit("白"))<<endl;
    //把界面初始化
    if (ui->comboBox->currentText().contains(QString::fromLocal8Bit("白")))
    {
        qDebug()<<"white"<<endl;
        setRole(chess::White);
    }

    if (ui->comboBox->currentText().contains(QString::fromLocal8Bit("黑")))
    {
        qDebug()<<"balck"<<endl;
        setRole(chess::Black);
    }
    //把棋盘初始化
    setChessInit();
}
/*人对机器*/
void ChessForm::on_P_VS_C_clicked()
{
    currentPK = PVC;
      //把界面初始化
      if (ui->comboBox->currentText().contains(QString::fromLocal8Bit("白")))
      {
//          qDebug()<<"白";
          setRole(chess::White);
      }

      if (ui->comboBox->currentText().contains(QString::fromLocal8Bit("黑")))
      {
          setRole(chess::Black);
      }
      //把棋盘初始化
      setChessInit();

}
/*判断下棋模式*/
void ChessForm::doProcessChessData(int i, int j)
{
//    qDebug()<<"i:"<<i<<"\tj:"<<j<<endl;
    //判断能否下子
    if(currentPK != NVN){
        int ret = judgeRule(i, j, formChessData, currentRole,true);
        if (ret)
        {
            //把数据传送给棋盘，用于更新棋盘界面
            myChess->setChessStatus(formChessData);
            if (currentPK == PVP)
            {
                RoleChange();
                ChessShow();
            }
            else if (currentPK == PVC)
            {
    //            qDebug()<<"PVC";
                RoleChange();
                RebootRole(currentRole);
                ChessShow();
            }
        }
    }
    else {
        if(isDown){
            int ret = judgeRule(i, j, formChessData, currentRole,true);
            if (ret){
                QString myIp = ui->ip->text();
                QString myPort = ui->port->text();
                QString myName = ui->fromname->text();
                QString toName = ui->toname->text();
                //把数据传送给棋盘，用于更新棋盘界面
                myChess->setChessStatus(formChessData);
                //通知对方更新 data#from#to#x#y#role#dataEnd
                QString msg = QString("data#%1#%2#%3#%4#%5#dataEnd")
                        .arg(myName).arg(toName).arg(i).arg(j).arg(currentRole);
                mySocket->writeDatagram(msg.toUtf8(),QHostAddress(myIp),myPort.toUInt());

                //数据统计
                ChessShow();
                isDown = false;

                ui->label_a->clear();
                ui->label_a->setText(toName);
                QFont font ( "Microsoft YaHe", 15, 75);
                ui->label_a->setFont(font);
                QPalette pa;
                pa.setColor(QPalette::WindowText,Qt::blue);
                ui->label_a->setPalette(pa);
            }
        }
    }

}
/*网络对战*/
void ChessForm::on_N_VS_N_clicked()
{
    //接收方
    if(isPeople){
        isDown = false;
        //把界面初始化
        if(ui->comboBox->currentText().contains(QStringLiteral("白"))){
            setRole(chess::Black);
        }else {
            setRole(chess::White);
        }
        qDebug()<<QStringLiteral("clieck 接收方");
    }
    else {
        isDown = true;
        //把界面初始化
        if(ui->comboBox->currentText().contains(QStringLiteral("白"))){
            setRole(chess::White);
        }else {
            setRole(chess::Black);
        }
        qDebug()<<QStringLiteral("clieck 开战方");
    }

    currentPK = NVN;
    //把棋盘初始化
//    setChessInit();

    //通知对方棋盘初始化 init#from#to#role#initEnd
    QString myIp = ui->ip->text();
    QString myPort = ui->port->text();
    QString myName = ui->fromname->text();
    QString toName = ui->toname->text();

    QString msg = QString::fromLocal8Bit("init#%1#%2#%3#initEnd")
            .arg(myName).arg(toName).arg(currentRole);
    mySocket->writeDatagram(msg.toUtf8(),QHostAddress(myIp),myPort.toUInt());


    isPeople = true;
}
//读取信息
void ChessForm::doProcessReadyRead()
{
    QByteArray ba;
    QHostAddress addr;
    quint16 port;
    while (mySocket->hasPendingDatagrams()) {
        ba.clear();
        ba.resize(mySocket->pendingDatagramSize());
        addr.clear();
        port = 0;
        mySocket->readDatagram(ba.data(),ba.length(),&addr,&port);
//        qDebug()<<QString(ba);
        QString str = QString(ba);
        //判断连接
        if(str.contains("inline#")){
            QMessageBox::information(this,"info",QStringLiteral("连接成功"),QMessageBox::Yes);
            ui->btn_bind->setEnabled(false);
        }
        //棋盘初始化数据init#from#to#role#initEnd
        else if(str.contains("init#")){
            QStringList list = str.split("#");
            //开战方
            if(isPeople){
                qDebug()<<QStringLiteral("init 开战方");
                setChessInit();
                ui->N_VS_N->setEnabled(false);
            }
            else{
                if(QString(list.at(3)).toInt() == chess::White){
                    currentRole = chess::Black;
                }
                else if (QString(list.at(3)).toInt() == chess::Black) {
                    currentRole = chess::White;
                }

                int ret=  QMessageBox::information(this,QStringLiteral("提示"),QString::fromLocal8Bit("%1:请求开战").arg(list.at(1)),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
                if(ret == QMessageBox::Yes){
        //            qDebug()<<"current:"<<currentRole;
                    currentPK = NVN;
                    setRole(currentRole);
                    //初始化
                    setChessInit();

                    if(QString(list.at(1)).toInt() == chess::White){
                        ui->comboBox->setCurrentIndex(0);
                    }
                    else if (QString(list.at(1)).toInt() == chess::Black) {
                        ui->comboBox->setCurrentIndex(1);
                    }
                    qDebug()<<QStringLiteral("init 接收方");
                    isPeople = true;
                    ui->toname->setText(list.at(1));

                    QString myName = ui->fromname->text();
                    ui->label_a->clear();
                    ui->label_a->setText(myName);
                    QFont font ( "Microsoft YaHe", 15, 75);
                    ui->label_a->setFont(font);
                    QPalette pa;
                    pa.setColor(QPalette::WindowText,Qt::red);
                    ui->label_a->setPalette(pa);
                    on_N_VS_N_clicked();
                    ui->N_VS_N->setEnabled(false);
                }
                else {
//                    return;
                    qDebug()<<QStringLiteral("拒绝请求");
                    QString msg = QString::fromLocal8Bit("error#%1#%2#errorEnd").arg(list.at(1)).arg(list.at(2));
                    QString myIp = ui->ip->text();
                    QString myPort = ui->port->text();
                    mySocket->writeDatagram(msg.toUtf8(),QHostAddress(myIp),myPort.toUInt());
                }
            }
        }
        //下棋数据 data#from#to#x#y#role#dataEnd
        else if(str.contains("data#")){
            QStringList list = str.split("#");
            int x = QString(list.at(3)).toInt();
            int y = QString(list.at(4)).toInt();
            int role = QString(list.at(5)).toInt();
            //更新界面
            int ret = judgeRule(x,y,formChessData,(chess::ChessType)role,true);
            if(ret){
                //把数据传送到棋盘类
                myChess->setChessStatus(formChessData);
                //数据统计
                ChessShow();

                isDown = true;

                QString myName = ui->fromname->text();
                ui->label_a->clear();
                ui->label_a->setText(myName);
                QFont font ( "Microsoft YaHe", 15, 75);
                ui->label_a->setFont(font);
                QPalette pa;
                pa.setColor(QPalette::WindowText,Qt::red);
                ui->label_a->setPalette(pa);
            }
        }
        //退出
        else if (str.contains("unline#")) {
            QMessageBox::warning(this,QStringLiteral("提示"),QStringLiteral("对方已经退出!"),QMessageBox::Yes);
            this->close();
        }
        //拒绝请战
        else if (str.contains("error#")) {
            QMessageBox::warning(this,QStringLiteral("提示"),QString::fromLocal8Bit("%1:对方拒绝请求"),QMessageBox::Yes);
            delete ui->gridLayout->takeAt(0);
            Init();
            ui->LCD1->display(0);
            ui->LCD2->display(0);
            ui->lbl2->setVisible(true);
            ui->lbl1->setVisible(true);
        }
        //重新开始   undo#status#fromname#toname#undoEnd
        else if (str.contains("undo#")) {
            QStringList list = str.split("#");
            QString status = list.at(1);
            int s = status.toInt();
            if(isUndo){
                if(s == 1){
                    setChessInit();
                    if(currentRole == chess::Black){
                        if(ui->comboBox->currentText().contains(QStringLiteral("白"))){
                            isDown = false;
                        }else {
                            isDown = true;
                        }
                    }else {
                        if(ui->comboBox->currentText().contains(QStringLiteral("白"))){
                            isDown = true;
                        }else {
                            isDown = false;
                        }
                    }
                }
            }else {
                isUndo = true;
                int te = QMessageBox::information(this,"提示","对方请求重新开始?",QMessageBox::Yes|QMessageBox::No);
                if(te == QMessageBox::Yes){
                    setChessInit();
                    if(currentRole == chess::Black){
                        if(ui->comboBox->currentText().contains(QStringLiteral("白"))){
                            isDown = false;
                        }else {
                            isDown = true;
                        }
                    }else {
                        if(ui->comboBox->currentText().contains(QStringLiteral("白"))){
                            isDown = true;
                        }else {
                            isDown = false;
                        }
                    }

                    QString myIp = ui->ip->text();
                    QString myPort = ui->port->text();
                    QString myName = ui->fromname->text();
                    QString toName = ui->toname->text();
                    QString msg = QString("undo#%1#%2#%3#undoEnd")
                            .arg(1).arg(myName).arg(toName);
                    mySocket->writeDatagram(msg.toUtf8(),QHostAddress(myIp),myPort.toUInt());
                }else {
                    QString myIp = ui->ip->text();
                    QString myPort = ui->port->text();
                    QString myName = ui->fromname->text();
                    QString toName = ui->toname->text();
                    QString msg = QString("undo#%1#%2#%3#undoEnd")
                            .arg(0).arg(myName).arg(toName);
                    mySocket->writeDatagram(msg.toUtf8(),QHostAddress(myIp),myPort.toUInt());
                }
            }
        }

    }
}
//错误信息
void ChessForm::doProcessError(QAbstractSocket::SocketError err)
{
    qDebug()<<err;
}
//绑定服务器
void ChessForm::on_btn_bind_clicked()
{
    QString myIp = ui->ip->text();
    QString myPort = ui->port->text();
    QString myName = ui->fromname->text();
    //上线数据
    QString msg = QString("inline#%1#unlineEnd").arg(myName);
    mySocket->writeDatagram(msg.toUtf8(),QHostAddress(myIp),myPort.toUInt());


}
//test
void ChessForm::on_pushButton_clicked()
{
    int temp = QMessageBox::information(this,"info",QStringLiteral("是否重新开始"),QMessageBox::Yes|QMessageBox::No);
    if (currentPK != NVN){
        if(temp == QMessageBox::Yes){
            delete ui->gridLayout->takeAt(0);
            Init();
            ui->LCD1->display(0);
            ui->LCD2->display(0);
            ui->lbl2->setVisible(true);
            ui->lbl1->setVisible(true);
        }
    }else if(currentPK == NVN) {
        if(temp == QMessageBox::Yes){
            isUndo = true;
            QString myIp = ui->ip->text();
            QString myPort = ui->port->text();
            QString myName = ui->fromname->text();
            QString toName = ui->toname->text();
            //通知对方重新开始 undo#status#fromname#toname#undoEnd
            QString msg = QString("undo#%1#%2#%3#undoEnd")
                    .arg(0).arg(myName).arg(toName);
            mySocket->writeDatagram(msg.toUtf8(),QHostAddress(myIp),myPort.toUInt());
        }
    }
}

//---------------------end--------------------
