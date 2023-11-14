/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 09:33:24
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-31 09:43:48
 * @FilePath: \EOS\interface\src\preview\historybox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "historybox.h"

HistoryBox::HistoryBox(QWidget *parent) : GroupBox(parent)
{
    checkbox = new CheckBox(tr("apply historical config?"));
    btn = new PushButton(tr("select config file"));
    btn->hide();

    auto lay = new QHBoxLayout;
    lay->addWidget(checkbox);
    lay->addWidget(btn);
    lay->addStretch();
    lay->setSpacing(10);

    setLayout(lay);
    setTitle(tr("Config"));
    connect(checkbox,&CheckBox::checkedChanged,this,&HistoryBox::onCheckBox);
    connect(btn,&PushButton::clicked,this,&HistoryBox::onBtn);
}

void HistoryBox::onBtn()
{
    auto path = QFileDialog::getOpenFileName(this,tr("Select Configuration File"),"",
                                            "config (*.eos)");
    if (path.isEmpty()) return;

    emit importFilePath(path);
}

void HistoryBox::onCheckBox(bool isChecked)
{
    isChecked? btn->show():btn->hide();
}