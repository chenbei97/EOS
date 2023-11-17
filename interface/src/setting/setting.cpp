/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 14:13:03
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-17 16:39:58
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
    toggleSetting(3);
}

void Setting::emitSignals()
{
    objectivesetting->emitSignals();
}

void Setting::toggleSetting(int option)
{ // 这里用通知者模式比较好,避免大量的if_else判断
    switch (option)
    {
        case 0:
            notifier->notify(b1);
            break;
        case 1:
            notifier->notify(b2);
            break;
        case 2:
            notifier->notify(objectivesetting);
            break;
        case 3:
            notifier->notify(wellsetting);
            break;
    }
//    if (option == 0) {
//        b1->show();
//        b2->hide();
//        objectivesetting->hide();
//        wellsetting->hide();
//        loginbtn->setChecked(true);
//    } else if (option == 1) {
//        b1->hide();
//        b2->show();
//        objectivesetting->hide();
//        wellsetting->hide();
//        databasebtn->setChecked(true);
//    } else if (option == 2) {
//        b1->hide();
//        b2->hide();
//        objectivesetting->show();
//        wellsetting->hide();
//        objectivebtn->setChecked(true);
//    } else if (option == 3) {
//        b1->hide();
//        b2->hide();
//        objectivesetting->hide();
//        wellsetting->show();
//        wellbtn->setChecked(true);
//    }
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
    lay->addWidget(wellbtn);
    lay->addWidget(wellsetting);
    //lay->addStretch();
    setLayout(lay);
}

void Setting::initAttributes()
{
    b1->hide();
    b2->hide();
    objectivesetting->hide();
    wellsetting->hide();

    buttongroup->addButton(loginbtn,0);
    buttongroup->addButton(databasebtn,1);
    buttongroup->addButton(objectivebtn,2);
    buttongroup->addButton(wellbtn,3);

    b1->setSettingID(0);
    b2->setSettingID(1);
    objectivesetting->setSettingID(2);
    wellsetting->setSettingID(3);

    notifier->addToList(b1,loginbtn);// 要在设置ID之后去添加
    notifier->addToList(b2,databasebtn);
    notifier->addToList(objectivesetting,objectivebtn);
    notifier->addToList(wellsetting,wellbtn);
}

void Setting::initObjects()
{
    loginbtn = new QRadioButton(tr("Login Setting"));
    databasebtn = new QRadioButton(tr("Database Setting"));
    objectivebtn = new QRadioButton(tr("Objective Setting"));
    wellbtn = new QRadioButton(tr("Well Setting"));
    buttongroup = new QButtonGroup;

    b1 = new GroupBox;
    b2 = new GroupBox;
    objectivesetting = new ObjectiveSetting;
    wellsetting = new WellSetting;

    notifier = new SettingNotifier;
}

void Setting::initConnections()
{
    connect(buttongroup,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&Setting::toggleSetting);
    connect(objectivesetting,&ObjectiveSetting::objectiveSettingChanged,this,&Setting::objectiveSettingChanged);
    //objectivesetting->emitSignals();
}