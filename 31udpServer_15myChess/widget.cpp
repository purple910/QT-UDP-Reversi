#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    Init();
}

Widget::~Widget()
{
    delete ui;
}

//function
void Widget::Init()
{
    mySocket = new QUdpSocket(this);
    listClient.clear();
    connect(mySocket,SIGNAL(readyRead()),this,SLOT(doProcessReadyRead()));
    connect(mySocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(doProcessError(QAbstractSocket::SocketError)));


    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    ui->ip->setText(info.addresses().at(5).toString());

}
//end function

//slots
void Widget::on_btn_bind_clicked()
{
    QString ip = ui->ip->text();
    QString port = ui->port->text();
    QString msg;
    bool ret = mySocket->bind(QHostAddress(ip),port.toUInt());
    if(!ret){
        msg = QStringLiteral("绑定失败");
        return;
    }
    else {
        msg = QStringLiteral("绑定成功");
        ui->btn_bind->setEnabled(false);
    }
    ui->textEdit->append(msg);

}

void Widget::on_btn_delete_clicked()
{
    QListWidgetItem *cur = ui->listWidget->currentItem();
    int row = ui->listWidget->currentRow();
    //先删除在线列表
    for (int i = 0; i < listClient.length(); ++i) {
        if(cur->text().contains(listClient.at(i).name)){
            listClient.removeAt(i);
            break;
        }
    }
    ui->listWidget->takeItem(row);
}

void Widget::doProcessReadyRead()
{
    QByteArray ba;
    QHostAddress addr;
    quint16 port;
    int temp = 0;

    while( mySocket->hasPendingDatagrams()){

        ba.clear();
        addr.clear();
        port = 0;
        ba.resize(mySocket->pendingDatagramSize());
        mySocket->readDatagram(ba.data(),ba.length(),&addr,&port);
        QString msg = QString(ba);
//        qDebug()<<"msg"<<msg;

        //上线数据 inline#name#inlineEnd
        if(msg.contains("inline")){
            QStringList list = msg.split("#");
            member mb;
            mb.addr = addr;
            mb.port = port;
            mb.name = list.at(1);
            listClient.append(mb);
            //记录上线
            QString str = QString::fromLocal8Bit("%1 [%2:%3]上线")
                    .arg(mb.name)
                    .arg(mb.addr.toString())
                    .arg(mb.port);
            ui->textEdit->append(str);

            //显示到在线列表中
            ui->listWidget->clear();
            for (int i = 0; i < listClient.length(); ++i) {
                QString label = QString::fromLocal8Bit("%1 [%2:%3]")
                        .arg(listClient.at(i).name)
                        .arg(listClient.at(i).addr.toString())
                        .arg(listClient.at(i).port);
                ui->listWidget->addItem(label);
            }
            mySocket->writeDatagram(msg.toUtf8(),mb.addr,mb.port);
        }
        //需要转发的消息 msg#from#to#content#msgEnd
        else if(msg.contains("msg")){
            QStringList list = msg.split("#");
            QString fromName = list.at(1);
            QString toName = list.at(2);
            QString content = list.at(3);
            //查找接受数据的用户地址
            for (int i = 0; i < listClient.length(); ++i) {
                if(listClient.at(i).name == toName){
                    QString str = QString::fromLocal8Bit("%1 说:%2")
                            .arg(fromName).arg(content);
                    mySocket->writeDatagram(str.toUtf8(),listClient.at(i).addr,listClient.at(i).port);
                }
            }
        }
        //下线数据 unline#name#unlineEnd
        else if(msg.contains("unline")){
            QStringList list = msg.split("#");
            QString name = list.at(1);
            QHostAddress addr;
            quint16 port;
            QString n;
            for (int i = 0; i < listClient.length(); ++i) {
                if(name == listClient.at(i).name){
                    n = listClient.at(i).name;
                    addr = listClient.at(i).addr;
                    port = listClient.at(i).port;
                    listClient.removeAt(i);                    
                    break;
                }                
            }
            //发送到对方
            temp = 2;
            if (name == this->myName){
                temp = 2;
            }else if (name == this->toName) {
                temp = 1;
            }

            //记录下线
            QString str = QString::fromLocal8Bit("%1 [%2:%3]下线")
                    .arg(n)
                    .arg(addr.toString())
                    .arg(port);
            ui->textEdit->append(str);

            //更新列表
            ui->listWidget->clear();
            for (int i = 0; i < listClient.length(); ++i) {
                QString label = QString::fromLocal8Bit("%1 [%2:%3]")
                        .arg(listClient.at(i).name)
                        .arg(listClient.at(i).addr.toString())
                        .arg(listClient.at(i).port);
                ui->listWidget->addItem(label);
            }

        }

        //棋盘数据 data#from#to#x#y#role#dataEnd
        else if(msg.contains("data#")){
            QStringList list = msg.split("#");
            if(list.length()<=0){
                return;
            }
            QString toName = list.at(2);
//            QString x = list.at(3);
//            QString y = list.at(4);
//            QString role = list.at(5);
            for (int i = 0; i < listClient.length(); ++i) {
                if(listClient.at(i).name == toName){
                    mySocket->writeDatagram(msg.toUtf8(),listClient.at(i).addr,listClient.at(i).port);
                    break;
                }
            }
        }
        //棋盘初始话 init#from#to#role#initEnd
        else if (msg.contains("init#")) {
            QStringList list = msg.split("#");
            QString fromname = list.at(1);
            QString toname = list.at(2);
            QString role = list.at(3);
            this->myName = fromname;
            this->toName = toname;
            for (int i = 0; i < listClient.length(); ++i) {
                if(listClient.at(i).name == toname){
                    QString buf = QString("init#%1#%2#%3#initEnd").arg(fromname).arg(toname).arg(role);
                    mySocket->writeDatagram(buf.toUtf8(),listClient.at(i).addr,listClient.at(i).port);
                    QString str = QString::fromLocal8Bit("%1:期盼初始化").arg(fromname);
                    ui->textEdit->append(str);
                    break;
                }
            }
        }
        //拒绝error#from#to#errorEnd
        else if (msg.contains("error#")) {
            QStringList list = msg.split("#");
            for (int i = 0; i < listClient.length(); ++i) {
                if(listClient.at(i).name == list.at(1)){
                    mySocket->writeDatagram(msg.toUtf8(),listClient.at(i).addr,listClient.at(i).port);
                }
            }
        }
        //重新开始 undo#status#fromname#toname#undoEnd
        else if(msg.contains("undo#")){
            QStringList list = msg.split("#");
            for (int i = 0; i < listClient.length(); ++i) {
                if(listClient.at(i).name == list.at(3)){
                    mySocket->writeDatagram(msg.toUtf8(),listClient.at(i).addr,listClient.at(i).port);
                }
            }
        }
        else{
            qDebug()<<"msg:"<<msg<<endl;
        }

        if(temp != 0){
            QStringList list = msg.split("#");
            if(temp == 2){
                this->myName.clear();
                if(this->toName != nullptr){
                    for (int j = 0; j < listClient.length(); ++j) {
                        if(this->toName == listClient.at(j).name){
                            this->toName.clear();
                            mySocket->writeDatagram(msg.toUtf8(),listClient.at(j).addr,listClient.at(j).port);
                        }
                    }
                }
            }else{
                this->toName.clear();
                if(this->myName != nullptr){
                    for (int j = 0; j < listClient.length(); ++j) {
                        if(this->myName == listClient.at(j).name){
                            this->myName.clear();
                            mySocket->writeDatagram(msg.toUtf8(),listClient.at(j).addr,listClient.at(j).port);
                        }
                    }
                }
            }
        }
    }
}

void Widget::doProcessError(QAbstractSocket::SocketError err)
{
    qDebug()<<"error:"<<err<<endl;
}

//end slots
