/*#include "tcp.h"

tcp::tcp(QObject *parent) : QObject(parent)
{
    ser = new QTcpServer;
    ser->listen(QHostAddress::AnyIPv4,8888);
    connect(ser,SIGNAL(newConnection()),this,SLOT(newconnected()));

    ser->waitForNewConnection();
}
void tcp::newconnected()
{
    qDebug()<<"connected";
    clientsock = ser->nextPendingConnection();
    connect(clientsock,SIGNAL(readyRead()),this,SLOT(readdata()));
}

void tcp::readdata()
{
    QByteArray buf = clientsock->readAll();
    clientsock->write(buf);
    emit tcp_rec(buf);
}*/
#include "tcp.h"
tcp::tcp(QObject *parent) : QObject(parent), ser(new QTcpServer)
{
    if (!ser->listen(QHostAddress::AnyIPv4, 8888)) {
        qDebug() << "Failed to start server:" << ser->errorString();
        return;
    }
    clientsock = new QTcpSocket;
    process = new QProcess;
    connect(ser, SIGNAL(newConnection()), this, SLOT(newconnected()));
}

void tcp::newconnected()
{
    QTcpSocket *client = ser->nextPendingConnection();
    if (client) {
        QMutexLocker locker(&mutex);
        clients.append(client);
        clientsock = client;
        connect(client, SIGNAL(readyRead()), this, SLOT(readdata()));
        connect(client, SIGNAL(disconnected()), this, SLOT(removeClient()));
        qDebug() << "New client connected:" << client->peerAddress().toString();
	client->write("connet_success");
    }
}
int i=-0;
void tcp::readdata()
{
    if(clientsock){
    QByteArray buf = clientsock->readAll();
    if (!buf.isEmpty()) {
        qDebug() << "Received data:" << buf;
     //   client->write(buf);
         if(buf.startsWith("sudo reboot")){
           QProcess::execute("sudo reboot");
         }else if(buf.startsWith("sudo shutdown now")){
           QProcess::execute("sudo shutdown -h now");
         }else if(buf.startsWith("restart")){
	   QProcess::execute("sudo systemctl restart my-program.service");
         }else{
          emit tcp_rec(buf);
          }
    }
    }
}

void tcp::removeClient()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) {
        qDebug() << "Invalid sender";
        return;
    }

    QMutexLocker locker(&mutex);
    clients.removeAll(client);
    client->deleteLater();
    qDebug() << "Client disconnected:" << client->peerAddress().toString();
    
    if (client == clientsock) {  // ?? clientsock ??
//        delete clientsock;
        clientsock = nullptr;
    }
}
void tcp::send_para_tcp(const QByteArray &send_para){
    qDebug()<<"hello5";
    if (!clientsock || !clientsock->isOpen()) {
    qDebug()<<"hell7o";
        qDebug() << "Client is not connected";
        return;
    }
    qDebug()<<"tcp rev"<<send_para;
    qDebug()<<"hell7o";
    clientsock->write(send_para);
    qDebug()<<"hell7o";
}
