/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 14:13:03
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 10:11:59
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
{ // 这里用通知者模式比较好,避免大量的if_else判断,注意option从1开始
    switch (option)
    {
        case 1:
            notifier->notify(b1);
            break;
        case 2:
            notifier->notify(b2);
            break;
        case 3:
            notifier->notify(objectivesetting);
            break;
        case 4:
            notifier->notify(wellsetting);
            break;
    }
}

void Setting::initLayout()
{
    auto lay = new QVBoxLayout;
    lay->addWidget(buttongroup->button(1));
    lay->addWidget(b1);
    lay->addWidget(buttongroup->button(2));
    lay->addWidget(b2);
    lay->addWidget(buttongroup->button(3));
    lay->addWidget(objectivesetting);
    lay->addWidget(buttongroup->button(4));
    lay->addWidget(wellsetting);
    //lay->addStretch();
    setLayout(lay);
}

void Setting::initAttributes()
{
    buttongroup->setText(QStringList()<<tr("Login Setting")<<tr("Database Setting")
        <<tr("Objective Setting")<<tr("Well Setting")); // 注意顺序1,2,3,4

    b1->hide();
    b2->hide();
    objectivesetting->hide();
    wellsetting->hide();

    b1->setID(1);
    b2->setID(2);
    objectivesetting->setID(3);
    wellsetting->setID(4);

    notifier->addToList(b1,static_cast<RadioButton*>(buttongroup->button(1)));// 要在设置ID之后去添加
    notifier->addToList(b2,static_cast<RadioButton*>(buttongroup->button(2)));
    notifier->addToList(objectivesetting,static_cast<RadioButton*>(buttongroup->button(3)));
    notifier->addToList(wellsetting,static_cast<RadioButton*>(buttongroup->button(4)));
}

void Setting::initObjects()
{
    buttongroup = new ButtonGroup(4,ButtonGroup::RadioBtn);
    b1 = new GroupBox;
    b2 = new GroupBox;
    objectivesetting = new ObjectiveSetting;
    wellsetting = new WellSetting;
    notifier = new SettingNotifier;
}

void Setting::initConnections()
{
    connect(buttongroup,&ButtonGroup::buttonClicked,this,&Setting::toggleSetting);
    connect(objectivesetting,&ObjectiveSetting::objectiveSettingChanged,this,&Setting::objectiveSettingChanged);
    //objectivesetting->emitSignals();
}