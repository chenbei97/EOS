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
    filenameedit = new LineEdit(PleaseInputFileNameField);
    exportToFile = new CheckBox(tr(SaveSettingToFileField));
    exportallbtn = new PushButton(tr(ExportField));
    loadbtn = new PushButton(tr(StartExperField));
    stopbtn = new PushButton(tr(StopExperField ));
    filenameedit->hide();
    exportallbtn->hide();

    auto lay1 = new QHBoxLayout;
    lay1->addWidget(exportToFile);
    lay1->addWidget(filenameedit);
    lay1->addWidget(exportallbtn);
    lay1->addStretch();

    auto lay2 = new QHBoxLayout;
    lay2->addWidget(loadbtn);
    lay2->addWidget(stopbtn);
    //lay2->addStretch();

    auto lay = new QVBoxLayout;
    lay->addLayout(lay1);
    lay->addLayout(lay2);
    setLayout(lay);
    setTitle(tr(SaveBoxTitle));

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
    auto dir = QFileDialog::getExistingDirectory(this,tr(SelectSavePathField));
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

void SaveBox::setChildWidgetsEnabled(bool enable)
{ // 启动实验后只有stop使能其他禁用,结束实验其它恢复,stopbtn不受影响
    filenameedit->setEnabled(enable);
    exportallbtn->setEnabled(enable);
    exportToFile->setEnabled(enable);
    loadbtn->setEnabled(enable);
    stopbtn->setEnabled(true);
}
