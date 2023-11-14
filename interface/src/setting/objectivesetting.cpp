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
    QVariantMap  m;
    if (location_one->isChecked())
        m[CameraLocationField] = 0;
    else if (location_two->isChecked())
        m[CameraLocationField] = 1;
    else if (location_three->isChecked())
        m[CameraLocationField] = 2;
    else if (location_four->isChecked())
        m[CameraLocationField] = 3;
    AssemblerPointer->assemble(TcpFramePool.frame0x0009,m);

    //LOG<<AssemblerMessage;
    SocketPointer->exec(TcpFramePool.frame0x0009,AssemblerMessage, false);
//    auto d = ParserResult; // 在不使用同步时这里只能拿到invalid qqvariant
//    LOG<<d;
//    if (d.toBool()) {
//        LOG<<"移动电机到位置"<<m[CameraLocationField].toInt();
//    }
}

void ObjectiveSetting::onMoveMachine(const QString& f,const QVariant& d)
{
    if (f == TcpFramePool.frame0x0009 && d.toBool()) {
        LOG<<"移动电机成功"; // 使用异步时可以绑定信号来实现
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

    info[CameraLocationField1] = Objective4x;
    info[CameraLocationField2] = Objective10x;
    info[CameraLocationField3] = Objective20x;
    info[CameraLocationField4] = Objective40x;

    tip = tr("配置信息:    %1 [%2]    %3 [%4]    %5 [%6]    %7 [%8]");
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

    auto lay2 = new QHBoxLayout;
    lay2->addWidget(new Label(tr("Objective Option: ")));
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
    locationbtn = new PushButton(tr("move machine to this"));

    br4x = new QRadioButton(Objective4x);
    ph4x = new QRadioButton(_4xPHField);
    br10x = new QRadioButton(Objective10x);
    ph10x = new QRadioButton(_10xPHField);
    br20x = new QRadioButton(Objective20x);
    ph20x = new QRadioButton(_20xPHField);
    br40x = new QRadioButton(Objective40x);
    ph40x = new QRadioButton(_40xPHField);
    none = new QRadioButton(NoneField);
    savebtn = new PushButton(tr("save current objective option"));

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
