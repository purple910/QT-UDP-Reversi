#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QHostAddress>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

struct member
{
   QHostAddress addr;
   quint16 port;
   QString name;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btn_bind_clicked();

    void on_btn_delete_clicked();

    void doProcessReadyRead();
    void doProcessError(QAbstractSocket::SocketError);

private:
    Ui::Widget *ui;
    QUdpSocket *mySocket;
    QList<member> listClient;

    void Init();

};
#endif // WIDGET_H
