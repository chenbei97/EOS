/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-08 10:48:47
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-08 11:10:09
 * @FilePath: \EOS\interface\src\preview\summarydialog.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "summarydialog.h"

SummaryDialog::SummaryDialog(const QVariantMap &m,QWidget*parent):QDialog(parent)
{
    btn = new PushButton(tr("启动实验"));
    panel = new SummaryPanel(m);
    setData(m);
    setWindowTitle(tr("实验信息面板"));
    resize(MainWindowWidth/2,MainWindowHeight/2);

    auto blay = new QHBoxLayout;
    blay->addStretch();
    blay->addWidget(btn);
    auto lay = new QVBoxLayout;
    lay->addWidget(panel);
    lay->addLayout(blay);
    setLayout(lay);

    connect(btn,&PushButton::clicked,this,&SummaryDialog::load);
}

void SummaryDialog::setData(const QVariantMap &m)
{
    panel->setData(m);
}

void SummaryDialog::load()
{
    int ret = QMessageBox::information(this,InformationChinese,
                                       tr("请检查所有实验信息是否无误,是否启动实验?"),
                                       QMessageBox::Yes|QMessageBox::Cancel);
    if (ret == QMessageBox::Cancel) {
        return;
    }
    accept();
}