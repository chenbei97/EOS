/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 16:58:10
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 16:58:38
 * @FilePath: \EOS\component\src\widgets\socketpanel.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "socketpanel.h"

void SocketPanel::onSend()
{
    edit->clear();
    askActivateCodeSync();
    //testData1_2();
    //QThread::sleep(1);

}

QVariantMap SocketPanel::getTestData1()
{ // 规定 发送帧头+坐标x,y,返回帧头+坐标x,y+路径
    QVariantMap m;
    m["test0x0"] = "test0x0";
    m["x"] = 10;
    m["y"] = 5;
    return m;
}

QVariantMap SocketPanel::getTestData2()
{ // 规定 发送帧头+equip,返回帧头+equip
    QVariantMap m;
    m["test0x1"] = "test0x1";
    m["equip"] = QString("xxx%1%2%3xxx").arg(rand()).arg(rand()).arg(rand());
    return m;
}

void SocketPanel::testData1_2()
{ // 测试data1和data2交替,不过socket.h定义的帧头不包含test0x0和text0x1所以没有回复
    for(int i = 0; i < 10;++i) {
        if (i%2) {
            AssemblerPointer->assemble("test0x0",getTestData1());
            auto msg1 = AssemblerMessage;
            edit->append(sendText.arg(QString::fromUtf8(msg1)));
            SocketPointer->exec("test0x0",msg1,true);
            edit->append(recvText.arg(ParserResult.toString())); // 这2个代码都是相同的
        } else {
            AssemblerPointer->assemble("test0x1",getTestData2());
            auto msg2= AssemblerMessage;
            edit->append(sendText.arg(QString::fromUtf8(msg2)));
            SocketPointer->exec("test0x1",msg2,true);
            edit->append(recvText.arg(ParserResult.toString())); // 这2个代码都是相同的
        }
    }
}

SocketPanel::SocketPanel(QWidget *parent): QWidget(parent)
{
    auto lay = new QVBoxLayout(this);
    edit = new TextEdit;
    btn = new PushButton(tr("发送"));

    auto blay = new QHBoxLayout;
    blay->addStretch();
    blay->addWidget(btn);

    lay->addWidget(edit);
    lay->addLayout(blay);
    setLayout(lay);

    connect(btn,&QPushButton::clicked,this,&SocketPanel::onSend);
    SocketInit;
    connect(ParserPointer,&ParserControl::parseResult,this,&SocketPanel::askActivateCodeASync);

    SocketPython * pthread = new SocketPython();
    pthread->start();

    btn->click();
    //QThread::sleep(1);

}

void SocketPanel::askActivateCodeSync()
{// 可以通过同步拿到消息
    edit->append(sendText.arg(QString::fromUtf8(assembleAskConnectedStateEvent(QVariantMap()))));
    SocketPointer->exec(TcpFramePool.askConnectedStateEvent,assembleAskConnectedStateEvent(QVariantMap()),true);
    //if (ParserResult.toBool())
   if (SocketPointer->result()[TcpFramePool.askConnectedStateEvent].toBool())
        edit->append(recvText.arg("[synchronous] socket is connect successful!"));
    else edit->append(recvText.arg("[synchronous] socket is connect failed!"));

    edit->append(sendText.arg(QString::fromUtf8(assembleAskActivateCodeEvent(QVariantMap()))));
    SocketPointer->exec(TcpFramePool.askActivateCodeEvent,assembleAskActivateCodeEvent(QVariantMap()),true);
    edit->append(recvText.arg("[synchronous] activate code is "+ParserResult.toString()));
}

void SocketPanel::askActivateCodeASync(const QString &f, const QVariant &d)
{ // 可以通过异步拿到消息
    //LOG<<"frame = "<<f<<" d = "<<d;
    if (f == TcpFramePool.askConnectedStateEvent && d.toBool()) {
        edit->append(recvText.arg("[asynchronous] socket is connect successful!"));
    }
    if (f == TcpFramePool.askActivateCodeEvent ) {
        edit->append(recvText.arg("[asynchronous] activate code is "+d.toString()));
    }
}