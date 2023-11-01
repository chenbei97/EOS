/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 09:14:53
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-31 09:15:21
 * @FilePath: \EOS\interface\src\preview\savebox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "savebox.h"

SaveBox::SaveBox(QWidget *parent) : GroupBox(parent)
{
    filenameedit = new LineEdit("please input filename");
    exportToFile = new CheckBox(tr("保存设置到文件?"));
    exportallbtn = new PushButton(tr("导出"));
    loadbtn = new PushButton(tr("启动实验"));
    filenameedit->hide();
    exportallbtn->hide();

    auto lay = new QHBoxLayout;
    lay->addStretch();
    lay->addWidget(filenameedit);
    lay->addWidget(exportallbtn);
    lay->addWidget(exportToFile);
    lay->addWidget(loadbtn);

    setLayout(lay);
    setTitle(tr("保存"));

    connect(exportToFile,&CheckBox::checkedChanged,this,&SaveBox::showExport);
    connect(exportallbtn,&PushButton::clicked,this,&SaveBox::exportFile);
    connect(loadbtn,&PushButton::clicked,this,&SaveBox::loadExper);
}

void SaveBox::loadExper()
{

}


void SaveBox::exportFile()
{
    auto filename = filenameedit->text().simplified();
    filename.remove(" ");//防止有空格,remove(QRegExp("\\s"));
    if (filename.isEmpty()) {
        QMessageBox::information(this,InformationChinese,tr("请先指定保存文件名称!"));
        return;
    }
    auto dir = QFileDialog::getExistingDirectory(this,tr("选择保存路径"));
    if (dir.isEmpty()) return;

    auto path = dir + +"/" + filename + ConfigFileSuffix;
    emit exportFilePath(path);
}

void SaveBox::showExport(bool enable)
{
    filenameedit->setVisible(enable);
    exportallbtn->setVisible(enable);
}

