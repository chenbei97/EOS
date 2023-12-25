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
    group = new PushGroup(3);
    lay->addWidget(group);
    setLayout(lay);

    group->setText(QStringList()<<ApplyHoleActTitle<<ApplyGroupActTitle<<ApplyAllActTitle);
    group->setChecked(1,true);
    connect(group,&PushGroup::pushClicked,this,&ViewApply::apply);
}

int ViewApply::mode() const
{
    return group->checkedID();
}

void ViewApply::apply(int id)
{
    group->setChecked(id,true);
    Q_UNUSED(id);
    accept();
}