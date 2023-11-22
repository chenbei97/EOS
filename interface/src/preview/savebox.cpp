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
    exportToFile = new CheckBox(tr("save settings to file?"));
    exportallbtn = new PushButton(tr("export"));
    loadbtn = new PushButton(tr("start exper"));
    stopbtn = new PushButton(tr("stop exper"));
    filenameedit->hide();
    exportallbtn->hide();

    auto lay = new QHBoxLayout;
    lay->addStretch();
    lay->addWidget(filenameedit);
    lay->addWidget(exportallbtn);
    lay->addWidget(exportToFile);
    lay->addWidget(stopbtn);
    lay->addWidget(loadbtn);

    setLayout(lay);
    setTitle(tr("Save"));

    connect(exportToFile,&CheckBox::checkedChanged,this,&SaveBox::showExport);
    connect(exportallbtn,&PushButton::clicked,this,&SaveBox::exportFile);
    connect(loadbtn,&PushButton::clicked,this,&SaveBox::loadExper);
    connect(stopbtn,&PushButton::clicked,this,&SaveBox::stopExper);
}

void SaveBox::exportFile()
{
    auto filename = filenameedit->text().simplified();
    filename.remove(" ");//防止有空格,remove(QRegExp("\\s"));
    if (filename.isEmpty()) {
        QMessageBox::information(this,InformationChinese,
                                 tr("Please specify the name of the saved file first!"));
        return;
    }
    auto dir = QFileDialog::getExistingDirectory(this,tr("Select Save Path"));
    if (dir.isEmpty()) return;

    auto path = dir + +"/" + filename + EOSSuffix;
    if (pathExisted(path)) {
        int ret = QMessageBox::warning(this,WarningChinese,
                                       tr("There is already a setting with the same name. Do you want to overwrite it?"),
                                       QMessageBox::Yes|QMessageBox::No);
        if (ret == QMessageBox::No)
            return;
    }
    emit exportFilePath(path);
}

void SaveBox::showExport(bool enable)
{
    filenameedit->setVisible(enable);
    exportallbtn->setVisible(enable);
}

