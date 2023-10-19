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
    AssemblerPointer->assemble("test0x0",getTestData1());
    edit->append(sendText.arg(QString::fromUtf8(AssemblerMessage)));
    SocketPointer->exec("test0x0",AssemblerMessage);
    //edit->append(recvText.arg(QString::fromUtf8(AssemblerMessage)));
}

QVariantMap SocketPanel::getTestData1()
{ // 规定 发送帧头+坐标x,y,返回帧头+坐标x,y+路径
    QVariantMap m;
    m["test0x0"] = "test0x0";
    m["x"] = 10;
    m["y"] = 5;
    return m;
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
}