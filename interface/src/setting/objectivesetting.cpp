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
    initAttributes();
    initLayout();

    connect(this,&ObjectiveSetting::objectiveSettingChanged,this,&ObjectiveSetting::updateTipInfo);
    connect(locationbtn,&PushButton::clicked,this,&ObjectiveSetting::moveMachine);
    connect(savebtn,&PushButton::clicked,this,&ObjectiveSetting::saveSetting);
    connect(buttongroup1,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&ObjectiveSetting::updateCheckedState);
}

void ObjectiveSetting::emitSignals()
{ // 想把初始状态更新给ui界面需要手动触发,在ObjectiveSetting和Setting构造函数内触发都不起作用
    emit objectiveSettingChanged(info);
}

void ObjectiveSetting::saveSetting()
{
    // 1.获取当前要配置的位置和镜头
    auto current_loc = buttongroup1->checkedButton()->text();
    auto current_objective = buttongroup2->checkedButton()->text();

    // 2.选择的和之前选择的一样,直接返回
    if (info[current_loc] == current_objective) return;

    // 3. 选了无镜头,更新当前位置数据和tipinfo即可
    if (current_objective == NoneField) {
        info[current_loc] = NoneField; // 选的无镜头,那么直接覆盖无镜头就行,不会对其他位置产生影响
        emit objectiveSettingChanged(info);
        return;
    }

    // 4. 遍历其它位置选过的镜头,如果有选过,提示占用
    foreach(auto other_objective,info.values()) {
        if (current_objective == other_objective) {
            int ret = QMessageBox::warning(this,WarningChinese,tr("该镜头已被占用,是否更换?")
                                           ,QMessageBox::Yes|QMessageBox::No);
            if (ret == QMessageBox::No) {
                emit objectiveSettingChanged(info);
                return;
            }
        }
    }

    // 5. 没有被占用或者继续更换就更新数据和tipinfo
    info[current_loc] = current_objective;
    foreach(auto loc,info.keys(current_objective)) {
        if (loc != current_loc) {
            info[loc] = NoneField; // 其它位置选了当前镜头设置的都清除掉
        }
    }
    emit objectiveSettingChanged(info);
}

void ObjectiveSetting::moveMachine()
{

}

void ObjectiveSetting::updateCheckedState()
{
    // 根据当前位置找到选择的镜头信息
    auto current_loc = buttongroup1->checkedButton()->text();
    auto current_objective = info[current_loc];

    // 根据镜头信息找到是哪个radiobutton
    objective_button[current_objective]->setChecked(true);

}

void ObjectiveSetting::updateTipInfo()
{
    tipinfo->setText(tip.arg(CameraLocationField1).arg(info[CameraLocationField1]).
                    arg(CameraLocationField2).arg(info[CameraLocationField2])
                             .arg(CameraLocationField3).arg(info[CameraLocationField3])
                             .arg(CameraLocationField4).arg(info[CameraLocationField4]));
}

void ObjectiveSetting::initAttributes()
{
    location_one->setChecked(true);
    br4x->setChecked(true);
    buttongroup1->buttonClicked(0);
    buttongroup2->buttonClicked(0);

    info[CameraLocationField1] = Bright4xField;
    info[CameraLocationField2] = PH4xField;
    info[CameraLocationField3] = Bright10xField;
    info[CameraLocationField4] = PH10xField;

    tip = tr("配置信息:    %1 [%2]    %3 [%4]    %5 [%6]    %7 [%8]");
    //emit objectiveSettingChanged(info);
    updateTipInfo();

    objective_button[Bright4xField] = br4x;
    objective_button[Bright10xField] = br10x;
    objective_button[Bright20xField] = br20x;
    objective_button[Bright40xField] = br40x;
    objective_button[PH4xField] = ph4x;
    objective_button[PH10xField] = ph10x;
    objective_button[PH20xField] = ph20x;
    objective_button[PH40xField] = ph40x;
    objective_button[NoneField] = none;
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

    auto lay3 = new QHBoxLayout;
    lay3->addWidget(tipinfo);
    lay3->addStretch();
    auto box3 = new GroupBox;
    box3->setLayout(lay3);

    auto lay = new QVBoxLayout;
    lay->addWidget(box1);
    lay->addWidget(box2);
    lay->addWidget(box3);
    lay->addStretch();
    lay->setSpacing(20);
    setLayout(lay);
}

void ObjectiveSetting::initObjects()
{
    buttongroup1 = new QButtonGroup;
    buttongroup2 = new QButtonGroup;
    tipinfo = new Label;

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
}
