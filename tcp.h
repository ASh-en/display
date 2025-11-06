/*#ifndef TCP_H
#define TCP_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
class tcp : public QObject
{
    Q_OBJECT
public:
    explicit tcp(QObject *parent = nullptr);

signals:

public slots:
    void newconnected();
    void readdata();
signals:
    void tcp_rec(const QByteArray& buf);

private:
    QTcpServer *ser;
    QTcpSocket *clientsock;
};

#endif // TCP_H*/
#ifndef TCP_H
#define TCP_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QMutex>
#include <QThread>
#include <QDebug>
#include <QProcess>
class tcp : public QObject
{
    Q_OBJECT
public:
    explicit tcp(QObject *parent = nullptr);

signals:
    void tcp_rec(const QByteArray& buf);

public slots:
    void newconnected();
    void readdata();
    void removeClient();
    void send_para_tcp(const QByteArray &send_para);
private:
    QTcpServer *ser;
    QList<QTcpSocket*> clients;
    QMutex mutex;
    QTcpSocket *clientsock;
    QProcess *process;
};

#endif // TCP_H
