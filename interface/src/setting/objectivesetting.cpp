/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 14:39:22
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-23 09:57:15
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
    connect(ParserPointer,&ParserControl::parseResult,this,&ObjectiveSetting::onMoveMachine);
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
            int ret = QMessageBox::warning(this,WarningChinese,
                                           tr("The lens is already occupied, do you want to replace it?")
                                           ,QMessageBox::Yes|QMessageBox::No);
            if (ret == QMessageBox::No) {
                //emit objectiveSettingChanged(info);//取消了就不用触发了
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
    QVariantMap  m;
    if (location_one->isChecked())
        m[ObjectiveLocationField] = 0;
    else if (location_two->isChecked())
        m[ObjectiveLocationField] = 1;
    else if (location_three->isChecked())
        m[ObjectiveLocationField] = 2;
    else if (location_four->isChecked())
        m[ObjectiveLocationField] = 3;
    AssemblerPointer->assemble(TcpFramePool.moveMachineEvent,m);

    //LOG<<AssemblerMessage;
    SocketPointer->exec(TcpFramePool.moveMachineEvent,AssemblerMessage, true);
    auto d = ParserResult; // 在不使用同步时这里只能拿到invalid qqvariant
    if (d.toBool()) {
        LOG<<"移动电机到位置"<<m[ObjectiveLocationField].toInt();
    }
}

void ObjectiveSetting::onMoveMachine(const QString& f,const QVariant& d)
{
    if (f == TcpFramePool.moveMachineEvent && d.toBool()) {
        LOG<<"移动电机成功"; // 使用异步时也可以绑定信号来实现
    }
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
    tipinfo->setText(tip.arg(ObjectiveLocationField1).arg(info[ObjectiveLocationField1]).
                    arg(ObjectiveLocationField2).arg(info[ObjectiveLocationField2])
                             .arg(ObjectiveLocationField3).arg(info[ObjectiveLocationField3])
                             .arg(ObjectiveLocationField4).arg(info[ObjectiveLocationField4]));
}

void ObjectiveSetting::initAttributes()
{
    location_one->setChecked(true);
    br4x->setChecked(true);
    buttongroup1->buttonClicked(0);
    buttongroup2->buttonClicked(0);

    info[ObjectiveLocationField1] = Objective4x; // 初始默认4个位置的物镜类型
    info[ObjectiveLocationField2] = Objective10x;
    info[ObjectiveLocationField3] = Objective20x;
    info[ObjectiveLocationField4] = Objective40x;

    tip = tr("Objective Info:    %1 [%2]    %3 [%4]    %5 [%6]    %7 [%8]");
    //emit objectiveSettingChanged(info);
    updateTipInfo();

    objective_button[Objective4x] = br4x;
    objective_button[Objective10x] = br10x;
    objective_button[Objective20x] = br20x;
    objective_button[Objective40x] = br40x;
    objective_button[_4xPHField] = ph4x;
    objective_button[_20xPHField] = ph10x;
    objective_button[_10xPHField] = ph20x;
    objective_button[_40xPHField] = ph40x;
    objective_button[NA20x05Field] = na20x05;
    objective_button[NA20x08Field] = na20x08;
    objective_button[NA40x095Field] = na40x095;
    objective_button[NoneField] = none;
}

void ObjectiveSetting::initLayout()
{
    auto lay1 = new QHBoxLayout;
    lay1->addWidget(new Label("Objective Location: "));
    lay1->addWidget(location_one);
    lay1->addWidget(location_two);
    lay1->addWidget(location_three);
    lay1->addWidget(location_four);
    lay1->addWidget(locationbtn);
    lay1->addStretch();
    lay1->setSpacing(20);
    auto box1 = new GroupBox;
    box1->setLayout(lay1);

    auto lay2 = new QVBoxLayout;
    lay2->addWidget(new Label(tr("Objective Option: ")));

    auto lay21 = new QHBoxLayout;
    lay21->addSpacing(ObjectiveSettingLeftSpacing);
    lay21->addWidget(new Label(ObjectiveLabel4x));
    lay21->addWidget(br4x);
    lay21->addWidget(ph4x);
    lay21->addStretch();

    auto lay22 = new QHBoxLayout;
    lay22->addSpacing(ObjectiveSettingLeftSpacing);
    lay22->addWidget(new Label(ObjectiveLabel10x));
    lay22->addWidget(br10x);
    lay22->addWidget(ph10x);
    lay22->addStretch();

    auto lay23 = new QHBoxLayout;
    lay23->addSpacing(ObjectiveSettingLeftSpacing);
    lay23->addWidget(new Label(ObjectiveLabel20x));
    lay23->addWidget(br20x);
    lay23->addWidget(ph20x);
    lay23->addWidget(na20x05);
    lay23->addWidget(na20x08);
    lay23->addStretch();

    auto lay24 = new QHBoxLayout;
    lay24->addSpacing(ObjectiveSettingLeftSpacing);
    lay24->addWidget(new Label(ObjectiveLabel40x));
    lay24->addWidget(br40x);
    lay24->addWidget(ph40x);
    lay24->addWidget(na40x095);
    lay24->addWidget(none);
    lay24->addStretch();

    auto lay25 = new QHBoxLayout;
    lay25->addSpacing(ObjectiveSettingLeftSpacing);
    lay25->addWidget(savebtn);
    lay25->addStretch();

    lay2->addLayout(lay21);
    lay2->addLayout(lay22);
    lay2->addLayout(lay23);
    lay2->addLayout(lay24);
    lay2->addLayout(lay25);
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

    location_one = new QRadioButton(ObjectiveLocationField1);
    location_two = new QRadioButton(ObjectiveLocationField2);
    location_three = new QRadioButton(ObjectiveLocationField3);
    location_four = new QRadioButton(ObjectiveLocationField4);
    locationbtn = new PushButton(tr("move machine to this"));

    br4x = new QRadioButton(Objective4x);
    ph4x = new QRadioButton(_4xPHField);
    br10x = new QRadioButton(Objective10x);
    ph10x = new QRadioButton(_10xPHField);
    br20x = new QRadioButton(Objective20x);
    ph20x = new QRadioButton(_20xPHField);
    br40x = new QRadioButton(Objective40x);
    ph40x = new QRadioButton(_40xPHField);
    na20x05 = new QRadioButton(NA20x05Field);
    na20x08 = new QRadioButton(NA20x08Field);
    na40x095 = new QRadioButton(NA40x095Field);
    none = new QRadioButton(NoneField);
    savebtn = new PushButton(tr("save option on current objective location"));

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
    buttongroup2->addButton(na20x05,8);
    buttongroup2->addButton(na20x08,9);
    buttongroup2->addButton(na40x095,10);
    buttongroup2->addButton(none,11);
}
