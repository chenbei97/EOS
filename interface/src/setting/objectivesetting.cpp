/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 14:39:22
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-31 14:41:42
 * @FilePath: \EOS\interface\src\setting\objectivesetting.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "objectivesetting.h"

ObjectiveSetting::ObjectiveSetting(QWidget*parent):GroupBox(parent)
{
    initObjects();
    initLayout();

    connect(buttongroup1,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&ObjectiveSetting::locationChanged);
    connect(buttongroup2,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&ObjectiveSetting::objectiveChanged);

//    connect(location_one,&QRadioButton::clicked,this,&ObjectiveSetting::locationChanged);
//    connect(location_two,&QRadioButton::clicked,this,&ObjectiveSetting::locationChanged);
//    connect(location_three,&QRadioButton::clicked,this,&ObjectiveSetting::locationChanged);
//    connect(location_four,&QRadioButton::clicked,this,&ObjectiveSetting::locationChanged);

//    connect(br4x,&QRadioButton::clicked,this,[=]{LOG<<"333";emit objectiveChanged(ObjectiveSettingFields.indexOf(Bright4xField));});
//    connect(ph4x,&QRadioButton::clicked,this,[=]{emit objectiveChanged(ObjectiveSettingFields.indexOf(PH4xField));});
//    connect(br10x,&QRadioButton::clicked,this,[=]{emit objectiveChanged(ObjectiveSettingFields.indexOf(Bright10xField));});
//    connect(ph10x,&QRadioButton::clicked,this,[=]{emit objectiveChanged(ObjectiveSettingFields.indexOf(PH10xField));});
//    connect(br20x,&QRadioButton::clicked,this,[=]{emit objectiveChanged(ObjectiveSettingFields.indexOf(Bright20xField));});
//    connect(ph20x,&QRadioButton::clicked,this,[=]{emit objectiveChanged(ObjectiveSettingFields.indexOf(PH20xField));});
//    connect(br40x,&QRadioButton::clicked,this,[=]{emit objectiveChanged(ObjectiveSettingFields.indexOf(Bright40xField));});
//    connect(ph40x,&QRadioButton::clicked,this,[=]{emit objectiveChanged(ObjectiveSettingFields.indexOf(PH40xField));});
}

void ObjectiveSetting::initLayout()
{
    auto lay1 = new QHBoxLayout;
    lay1->addWidget(new Label(tr("相机位置: ")));
    lay1->addWidget(location_one);
    lay1->addWidget(location_two);
    lay1->addWidget(location_three);
    lay1->addWidget(location_four);
    lay1->addWidget(locationbtn);
    lay1->addStretch();
    lay1->setSpacing(20);
    auto box1 = new GroupBox;
    box1->setLayout(lay1);

    auto lay2 = new QHBoxLayout;
    lay2->addWidget(new Label(tr("镜头设置: ")));
    lay2->addWidget(br4x);
    lay2->addWidget(ph4x);
    lay2->addWidget(br10x);
    lay2->addWidget(ph10x);
    lay2->addWidget(br20x);
    lay2->addWidget(ph20x);
    lay2->addWidget(br40x);
    lay2->addWidget(ph40x);
    lay2->addWidget(none);
    lay2->addWidget(savebtn);
    lay2->addStretch();
    lay2->setSpacing(20);
    auto box2 = new GroupBox;
    box2->setLayout(lay2);

    auto lay = new QVBoxLayout;
    lay->addWidget(box1);
    lay->addWidget(box2);
    lay->addStretch();
    lay->setSpacing(20);
    setLayout(lay);
}

void ObjectiveSetting::initObjects()
{
    buttongroup1 = new QButtonGroup;
    buttongroup2 = new QButtonGroup;

    location_one = new QRadioButton(CameraLocationField1);
    location_two = new QRadioButton(CameraLocationField2);
    location_three = new QRadioButton(CameraLocationField3);
    location_four = new QRadioButton(CameraLocationField4);
    locationbtn = new PushButton(tr("移动电机到该位置"));

    br4x = new QRadioButton(Bright4xField);
    ph4x = new QRadioButton(PH4xField);
    br10x = new QRadioButton(Bright10xField);
    ph10x = new QRadioButton(PH10xField);
    br20x = new QRadioButton(Bright20xField);
    ph20x = new QRadioButton(PH20xField);
    br40x = new QRadioButton(Bright40xField);
    ph40x = new QRadioButton(PH40xField);
    none = new QRadioButton(NoneField);
    savebtn = new PushButton(tr("保存当前镜头设置"));

    buttongroup1->addButton(location_one,0);
    buttongroup1->addButton(location_two,1);
    buttongroup1->addButton(location_three,2);
    buttongroup1->addButton(location_four,3);

    buttongroup2->addButton(br4x,0);
    buttongroup2->addButton(ph4x,1);
    buttongroup2->addButton(br10x,2);
    buttongroup2->addButton(ph10x,3);
    buttongroup2->addButton(br20x,4);
    buttongroup2->addButton(ph20x,5);
    buttongroup2->addButton(br40x,6);
    buttongroup2->addButton(ph40x,7);
    buttongroup2->addButton(none,8);

    location_one->setChecked(true);
    br4x->setChecked(true);
    buttongroup1->buttonClicked(0);
    buttongroup2->buttonClicked(0);
}
