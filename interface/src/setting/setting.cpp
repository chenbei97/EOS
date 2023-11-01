/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 14:13:03
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-31 14:14:01
 * @FilePath: \EOS\interface\src\setting\setting.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "setting.h"

Setting::Setting(QWidget *parent) : QWidget(parent)
{
    initObjects();
    initAttributes();
    initConnections();
    initLayout();
    toggleSetting(2);
}

void Setting::emitSignals()
{
    objectivesetting->emitSignals();
}

void Setting::toggleSetting(int option)
{
    if (option == 0) {
        b1->show();
        b2->hide();
        objectivesetting->hide();
    } else if (option == 1) {
        b1->hide();
        b2->show();
        objectivesetting->hide();
    } else if (option == 2) {
        b1->hide();
        b2->hide();
        objectivesetting->show();
    }
}

void Setting::initLayout()
{
    auto lay = new QVBoxLayout;
    lay->addWidget(loginbtn);
    lay->addWidget(b1);
    lay->addWidget(databasebtn);
    lay->addWidget(b2);
    lay->addWidget(objectivebtn);
    lay->addWidget(objectivesetting);
    //lay->addStretch();
    setLayout(lay);
}

void Setting::initAttributes()
{
    b1->hide();
    b2->hide();
    objectivesetting->hide();

    buttongroup->addButton(loginbtn,0);
    buttongroup->addButton(databasebtn,1);
    buttongroup->addButton(objectivebtn,2);

}

void Setting::initObjects()
{
    loginbtn = new QRadioButton(tr("登录设置"));
    databasebtn = new QRadioButton(tr("数据库设置"));
    objectivebtn = new QRadioButton(tr("物镜设置"));
    buttongroup = new QButtonGroup;

    b1 = new GroupBox;
    b2 = new GroupBox;
    objectivesetting = new ObjectiveSetting;
}

void Setting::initConnections()
{
    connect(buttongroup,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&Setting::toggleSetting);
    connect(objectivesetting,&ObjectiveSetting::objectiveSettingChanged,this,&Setting::objectiveSettingChanged);
    //objectivesetting->emitSignals();
}