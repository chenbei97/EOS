/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 16:21:26
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 16:25:15
 * @FilePath: \EOS\component\include\socket\tcpsocket.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_TCPSOCKET_H
#define EOSI_TCPSOCKET_H

#include "socket.h"
#include "parsecontrol.h"

class COMPONENT_IMEXPORT TcpSocket: public QObject
{
    Q_OBJECT
public:
    static TcpSocket& instance();
    void connectToHost(const QString &hostName = LocalHost, quint16 port = SocketPort);
    void exec(const QString& f,const QByteArray& c,bool user_sync = true);
    QVariantMap result() const;
private:
    explicit TcpSocket(QObject *parent = nullptr);
    ~TcpSocket();
    void onReadyReadSlot();
    void processMsgQueue();
    QTcpSocket * socket = nullptr;
    QByteQueue msgQueue;
    QString frame;
    EventLoop loop;
    QTimer timer;
signals:
};
#endif //EOSI_TCPSOCKET_H
