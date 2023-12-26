/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-25 16:40:57
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-25 17:09:01
 * @FilePath: \EOS\component\src\widgets\viewapply.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "viewapply.h"

ViewApply::ViewApply(QWidget *parent): QDialog(parent)
{
    auto lay = new QHBoxLayout;
    current = new PushButton(ApplyHoleActTitle);
    group = new PushButton(ApplyGroupActTitle);
    all = new PushButton(ApplyAllActTitle);
    lay->addWidget(current);
    lay->addWidget(group);
    lay->addWidget(all);
    setLayout(lay);
    connect(current,&PushButton::clicked,this,&ViewApply::apply);
    connect(group,&PushButton::clicked,this,&ViewApply::apply);
    connect(all,&PushButton::clicked,this,&ViewApply::apply);
}

int ViewApply::mode() const
{
    return mmode;
}

void ViewApply::apply()
{
    if (sender() == current) {
        mmode = 0;
    } else if (sender() == group) {
        mmode = 1;
    } else if (sender() == all) {
        mmode = 2;
    }
    accept();
}