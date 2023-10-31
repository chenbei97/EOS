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
    exportToFile = new CheckBox(tr("导出到文件?"));
    saveallbtn = new PushButton(tr("保存所有设置"));
    loadbtn = new PushButton(tr("启动实验"));
    filenameedit->hide();

    auto lay = new QHBoxLayout;
    lay->addStretch();
    lay->addWidget(filenameedit);
    lay->addWidget(exportToFile);
    lay->addWidget(saveallbtn);
    lay->addWidget(loadbtn);

    setLayout(lay);
    setTitle(tr("保存"));

    connect(exportToFile,&CheckBox::checkedChanged,this,[=](bool enable){filenameedit->setVisible(enable);});

}