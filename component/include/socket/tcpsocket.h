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
#include "requestqueuethread.h"
#include "parsecontrol.h"

#define WaitMessageBoxMoveMachineMsg  "please wait moving machine..."
#define WaitMessageBoxAutoFocusMsg  "please wait executing auto focus,...\n[has focused %1 times]"
#define WaitMessageBoxWaitFocusMsg  "please wait auto focus ready finished..."
#define WaitMessageBoxStartExperimentMsg  "please wait starting experiment..."
#define WaitMessageBoxStopExperimentMsg  "please wait stoping experiment..."
#define WaitMessageBoxToggleChannelMsg  "please wait toggling/closing channel..."
#define WaitMessageBoxSlideStitchMsg  "please wait stitching slide image...\n[has stitched %1 images]"
#define WaitMessageBoxWaitStitchMsg  "please wait stitching slide image...]"
#define use_queuethread 1

class COMPONENT_IMEXPORT TcpSocket: public QObject
{
    Q_OBJECT
public:
    static TcpSocket& instance();
    bool isConnected() const;
    QTcpSocket::SocketState socketState() const;
    void connectToHost(const QString &hostName = LocalHost, quint16 port = SocketPort);
    void exec(const QString& f,const QByteArray& c,bool use_sync);
    void exec(const QString& f,const QByteArray& c);
    void exec(const QString& f,const QByteArray& c,unsigned count);
    void exec_queue(const QString& f,const QByteArray& c);
    QVariantMap result() const;
    void resetWaitText();
    void setWaitText(const QString& text);
private:
    explicit TcpSocket(QObject *parent = nullptr);
    ~TcpSocket();
    void onReadyReadSlot();
    void processMsgQueue();
    Q_INVOKABLE void processRequestQueue();
    void processRequestMsg(const QRequestMsg& msg);
    QTcpSocket * socket = nullptr;
    WaitMessageBox * waitdlg = nullptr;
    QByteQueue msgQueue;
    QRequestQueue requestQueue;
    RequestQueueThread * requestThread;
    QByteArray message;
    QString frame;
    EventLoop loop;
    QTimer looptimer;
    QTimer requesttimer;
signals:
};
#endif //EOSI_TCPSOCKET_H
