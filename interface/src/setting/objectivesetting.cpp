/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 14:39:22
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 10:11:56
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
    connect(locationgroup,&RadioGroup::radioClicked,this,&ObjectiveSetting::updateCheckedState);
}

void ObjectiveSetting::emitSignals()
{ // 想把初始状态更新给ui界面需要手动触发,在ObjectiveSetting和Setting构造函数内触发都不起作用
    emit objectiveSettingChanged(info);
}

void ObjectiveSetting::saveSetting()
{
    // 1.获取当前要配置的位置和镜头
    auto current_loc = locationgroup->checkedText();
    auto current_objective = objectivegroup->checkedText();

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
    auto checkid = locationgroup->checkedID();
    checkid--;
    m[ObjectiveLocationField] = checkid;
    AssemblerPointer->assemble(TcpFramePool.moveMachineEvent,m);

    SocketPointer->exec(TcpFramePool.moveMachineEvent,AssemblerMessage);
    if (ParserResult.toBool()) {
        LOG<<"move machine to "<<m[ObjectiveLocationField].toInt()<<"successful!";
    } else LOG<<"move machine to "<<m[ObjectiveLocationField].toInt()<<"failed!";
}

void ObjectiveSetting::updateCheckedState()
{
    // 根据当前位置找到选择的镜头信息
    auto current_loc = locationgroup->checkedText();
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
    objectivegroup->setText(ObjectiveSettingFields);
    locationgroup->setText(ObjectiveLocationFields);
    
    objectivegroup->emitSignals(0);
    locationgroup->emitSignals(0);

    info[ObjectiveLocationField1] = Objective4x; // 初始默认4个位置的物镜类型
    info[ObjectiveLocationField2] = Objective10x;
    info[ObjectiveLocationField3] = Objective20x;
    info[ObjectiveLocationField4] = Objective40x;

    tip = tr("Objective Info:    %1 [%2]    %3 [%4]    %5 [%6]    %7 [%8]");
    //emit objectiveSettingChanged(info);
    updateTipInfo();

    objective_button[Objective4x] = static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(Objective4x)+1));
    objective_button[Objective10x] = static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(Objective10x)+1));
    objective_button[Objective20x] = static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(Objective20x)+1));
    objective_button[Objective40x] = static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(Objective40x)+1));
    objective_button[_4xPHField] = static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(_4xPHField)+1));
    objective_button[_10xPHField] = static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(_10xPHField)+1));
    objective_button[_20xPHField] = static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(_20xPHField)+1));
    objective_button[_40xPHField] = static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(_40xPHField)+1));
    objective_button[NA20x05Field] = static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(NA20x05Field)+1));
    objective_button[NA20x08Field] = static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(NA20x08Field)+1));
    objective_button[NA40x095Field] = static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(NA40x095Field)+1));
    objective_button[NoneField] = static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(NoneField)+1));
}

void ObjectiveSetting::initLayout()
{
    auto lay1 = new QHBoxLayout;
    lay1->addWidget(new Label("Objective Location: "));
    lay1->addWidget(locationgroup);
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
    lay21->addWidget(static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(Objective4x)+1)));
    lay21->addWidget(static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(_4xPHField)+1)));
    lay21->addStretch();

    auto lay22 = new QHBoxLayout;
    lay22->addSpacing(ObjectiveSettingLeftSpacing);
    lay22->addWidget(new Label(ObjectiveLabel10x));
    lay22->addWidget(static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(Objective10x)+1)));
    lay22->addWidget(static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(_10xPHField)+1)));
    lay22->addStretch();

    auto lay23 = new QHBoxLayout;
    lay23->addSpacing(ObjectiveSettingLeftSpacing);
    lay23->addWidget(new Label(ObjectiveLabel20x));
    lay23->addWidget(static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(Objective20x)+1)));
    lay23->addWidget(static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(_20xPHField)+1)));
    lay23->addWidget(static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(NA20x05Field)+1)));
    lay23->addWidget(static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(NA20x08Field)+1)));
    lay23->addStretch();

    auto lay24 = new QHBoxLayout;
    lay24->addSpacing(ObjectiveSettingLeftSpacing);
    lay24->addWidget(new Label(ObjectiveLabel40x));
    lay24->addWidget(static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(Objective40x)+1)));
    lay24->addWidget(static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(_40xPHField)+1)));
    lay24->addWidget(static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(NA40x095Field)+1)));
    lay24->addWidget(static_cast<RadioButton*>(objectivegroup->button(ObjectiveSettingFields.indexOf(NoneField)+1)));
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
    objectivegroup = new ButtonGroup(12,ButtonGroup::RadioBtn);
    savebtn = new PushButton(tr("save current objective on location"));
    locationbtn = new PushButton(tr("move machine to this"));
    locationgroup = new RadioGroup(4);
    tipinfo = new Label;
}
