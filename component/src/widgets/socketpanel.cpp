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
    SocketPointer->exec("0x0000","hello!");
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
}