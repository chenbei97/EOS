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
    btn = new PushButton(tr("start exper"));
    panel = new SummaryPanel(m);
    setData(m);
    setWindowTitle(tr("exper information panel"));
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
                                       tr("Please check if all experimental information is correct "
                                          "and whether to start the experiment?"),
                                       QMessageBox::Yes|QMessageBox::Cancel);
    if (ret == QMessageBox::Cancel) {
        return;
    }
    accept();
}