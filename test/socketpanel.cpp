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
    testData1_2();
    testData3();
}

void SocketPanel::testData1_2()
{
    for(int i = 0; i < 10;++i) {
        if (i%2) {
            AssemblerPointer->assemble("test0x0",getTestData1());
            auto msg1 = AssemblerMessage;
            edit->append(sendText.arg(QString::fromUtf8(msg1)));
            SocketPointer->exec("test0x0",msg1);
            //LOG<<"test0x0's response = "<<ParserPointer->response();
            //edit->append(recvText.arg(SocketPointer->result()["test0x0"].toString()));
            edit->append(recvText.arg(ParserResult.toString())); // 这2个代码都是相同的
        } else {
            AssemblerPointer->assemble("test0x1",getTestData2());
            auto msg2= AssemblerMessage;
            edit->append(sendText.arg(QString::fromUtf8(msg2)));
            SocketPointer->exec("test0x1",msg2);
            //LOG<<"test0x1's response = "<<ParserPointer->response();
            //edit->append(recvText.arg(SocketPointer->result()["test0x1"].toString()));
            edit->append(recvText.arg(ParserResult.toString())); // 这2个代码都是相同的
        }
    }
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

QByteArray SocketPanel::getTestData3()
{
    QByteArray c;
    QFile file(CURRENT_PATH+"/../test/test0x0001.json");
    file.open(QIODevice::ReadOnly);
    c = file.readAll();
    file.close();
    c.append(SeparateField);
    return c;
}

void SocketPanel::testData3()
{
    edit->append(sendText.arg(QString::fromUtf8(getTestData3())));
    SocketPointer->exec(TcpFramePool.frame0x0001,getTestData3());
    edit->append(recvText.arg(ParserResult.toString()));
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

    SocketPython * pthread = new SocketPython();
    pthread->start();

    btn->click();
    QThread::sleep(1);
}