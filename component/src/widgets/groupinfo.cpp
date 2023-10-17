/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 13:28:08
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 13:28:29
 * @FilePath: \EOS\component\src\widgets\groupinfo.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "groupinfo.h"

GroupInfo::GroupInfo(QWidget *parent) : QDialog(parent)
{
    initObjects();
    connect(btn,&PushButton::clicked,this,&GroupInfo::onClick);
}

QColor GroupInfo::groupColor() const
{
    return cbtn->color();
}

void GroupInfo::setBtnColor(const QColor& color)
{
    cbtn->setColor(color);
}

void GroupInfo::onClick()
{
  emit accept();
}

void GroupInfo::initObjects()
{
    tmp1 = new LineEdit;
    tmp2 = new LineEdit;
    tmp3 = new LineEdit;
    groupName = new LineEdit;
    btn = new PushButton(tr("确定"));
    cbtn = new ColorButton;

    auto lay = new QVBoxLayout;
    auto formlay = new QFormLayout;
    formlay->addRow(tr("组别: "),groupName);
    formlay->addRow(tr("xxx: "),tmp1);
    formlay->addRow(tr("xxx: "),tmp2);
    formlay->addRow(tr("xxx: "),tmp3);

    auto blay = new QHBoxLayout;
    blay->addStretch();
    blay->addWidget(cbtn);
    blay->addWidget(btn);

    lay->addLayout(formlay);
    lay->addLayout(blay);
    setLayout(lay);


}
