/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-23 13:55:14
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 10:10:08
 * @FilePath: \EOS\interface\src\preview\objectivebox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "objectivebox.h"

ObjectiveBox::ObjectiveBox(QWidget *parent): GroupBox(parent)
{
    // 这里初始化文字时需要跟objectivesetting.cpp初始化的4个镜头选项保持一致
    rbtn_loc1 = new RadioButton(Objective4x+tr(loc1_));
    rbtn_loc2 = new RadioButton(Objective10x+tr(loc2_));
    rbtn_loc3 = new RadioButton(Objective20x+tr(loc3_));
    rbtn_loc4 = new RadioButton(Objective40x+tr(loc4_));

    location_button[ObjectiveLocationField1] = rbtn_loc1;
    location_button[ObjectiveLocationField2] = rbtn_loc2;
    location_button[ObjectiveLocationField3] = rbtn_loc3;
    location_button[ObjectiveLocationField4] = rbtn_loc3;

    QHBoxLayout * lay = new QHBoxLayout;
    lay->addWidget(rbtn_loc1);
    lay->addWidget(rbtn_loc2);
    lay->addWidget(rbtn_loc3);
    lay->addWidget(rbtn_loc4);
    lay->addStretch();

    rbtn_loc2->setChecked(true); // 默认应该也是10倍镜

    setLayout(lay);
    setTitle(tr(ObjectiveBoxTitle));

    connect(rbtn_loc1,&RadioButton::clicked,this,&ObjectiveBox::onClicked);
    connect(rbtn_loc2,&RadioButton::clicked,this,&ObjectiveBox::onClicked);
    connect(rbtn_loc3,&RadioButton::clicked,this,&ObjectiveBox::onClicked);
    connect(rbtn_loc4,&RadioButton::clicked,this,&ObjectiveBox::onClicked);
}

void ObjectiveBox::onObjectiveSettingChanged(const LocationObjectiveInfo &m)
{ // setting设置的每个位置放的物镜型号,需要更新当前4个位置物镜的文字

    rbtn_loc1->setText(m[ObjectiveLocationField1]+tr(loc1_));
    rbtn_loc2->setText(m[ObjectiveLocationField2]+tr(loc2_));
    rbtn_loc3->setText(m[ObjectiveLocationField3]+tr(loc3_));
    rbtn_loc4->setText(m[ObjectiveLocationField4]+tr(loc4_));

    // 可能有无镜头的选项
    if (m.values().contains(NoneField)) {
        auto loc = m.key(NoneField); // 找到无镜头的位置
        location_button[loc]->setEnabled(false); // none不能选禁用
        for(auto cloc: m.keys()) {
            if (cloc != loc) {
                location_button[cloc]->setEnabled(true); // 其它的要恢复使能
            }
            location_button[cloc]->setChecked(true);//从none移开,选择个有效的就可触发更新ui(更新channelbox,timebox有关通道的内容)信息即可
            location_button[cloc]->blockSignals(true); // 不需要在更改配置弹出0.17mm对话框阻塞信号
            location_button[cloc]->click();// click才能触发更新信号
            location_button[cloc]->blockSignals(false);
        }
    } else { // 没包含none
        for(auto cloc: m.keys()) {
               location_button[cloc]->setEnabled(true); // 可能第1次有none,第2次没有none要全部恢复使能
        }
        rbtn_loc1->setChecked(true); // 希望切换物镜硬件配置时能够确保UI界面信息的更新,手动触发
        rbtn_loc1->blockSignals(true);
        rbtn_loc1->click(); // click才能触发更新信号 => onClick函数
        rbtn_loc1->blockSignals(false);
    }
}

void ObjectiveBox::onClicked()
{ // 点击任意物镜时
    QString text;
    int objective_loc = 0; // 物镜位置
    // 判断点击的是哪个然后获取按钮的文本并获取物镜位置
    if (rbtn_loc1->isChecked()) {
        text = rbtn_loc1->text();
        objective_loc = 0;
    }
    else if (rbtn_loc2->isChecked()){
        text = rbtn_loc2->text();
        objective_loc = 1;
    }
    else if (rbtn_loc3->isChecked()){
        text = rbtn_loc3->text();
        objective_loc = 2;
    }
    else if (rbtn_loc4->isChecked()){
        text = rbtn_loc4->text();
        objective_loc = 3;
    }
    auto len = QString(loc1_).count();
    text.chop(len); // 去除位置尾缀[loc1]

    if (text.contains(NAField,Qt::CaseInsensitive))
        QMessageBox::information(this,InformationChinese,
                                 tr("please using 0.17mm container!"),QMessageBox::Yes);

    int isph = 0; // 是否为PH物镜, 带NA的或者包含BR的属于BR物镜

    if (text.contains(PHField,Qt::CaseInsensitive)) {
        isph = 1;
    } else isph = 0;// 新增的20XNA0.5WD2.1,20XNA0.8WD0.6,40XNA0.95WD0.18都属于BR镜头

    int objective = 0; // 物镜倍数的标记
    if (text.contains(Objective4x)) objective = 0;
    else if (text.contains(Objective10x)) objective = 1;
    else if (text.contains(Objective20x)) objective = 2;
    else if (text.contains(Objective40x)) objective = 3;
    //LOG<<objective<<objective_loc<<isph<<text;

    emit objectiveChanged(text);
    // 用于切物镜时就动电机,需要给物镜倍数,物镜位置,是否为PH类型
    emit objectiveToggled(objective,objective_loc,isph);
}

ObjectiveInfo ObjectiveBox::objectiveInfo() const
{
    ObjectiveInfo m;
    QString text;
    if (rbtn_loc1->isChecked()) {
        text = rbtn_loc1->text();
        m[ObjectiveLocationField] = QString::number(ObjectiveLocationField1Index);

    }
    else if (rbtn_loc2->isChecked()) {
        text = rbtn_loc2->text();
        m[ObjectiveLocationField] = QString::number(ObjectiveLocationField2Index);
    }
    else if (rbtn_loc3->isChecked()) {
        text = rbtn_loc3->text();
        m[ObjectiveLocationField] = QString::number(ObjectiveLocationField3Index);
    }
    else if (rbtn_loc4->isChecked()) {
        text = rbtn_loc4->text();
        m[ObjectiveLocationField] = QString::number(ObjectiveLocationField4Index);
    }
    auto len = QString(loc1_).count();
    text.chop(len);
    m[ObjectiveDescripField] = text;

    // objective存放倍数的原字符串
    if (m[ObjectiveDescripField].contains(Objective4x,Qt::CaseInsensitive))
        m[ObjectiveField]=Objective4x;
    else if (m[ObjectiveDescripField].contains(Objective10x,Qt::CaseInsensitive))
        m[ObjectiveField]=Objective10x;
    else if (m[ObjectiveDescripField].contains(Objective20x,Qt::CaseInsensitive))
        m[ObjectiveField]=Objective20x;
    else if (m[ObjectiveDescripField].contains(Objective40x,Qt::CaseInsensitive))
        m[ObjectiveField]=Objective40x;

    // 同理增加,ObjectiveTypeField,包含PH就是1否则就是0
    if (m[ObjectiveDescripField].contains(ObjectivePH,Qt::CaseInsensitive))
        m[ObjectiveTypeField]=getIndexFromFields(ObjectivePH);
    else
        m[ObjectiveTypeField]=getIndexFromFields(ObjectiveBR); // 其它物镜都是BR类型

    return m;
}

int ObjectiveBox::currentObjective() const
{ // 返回物镜倍数代号
    return getIndexFromFields(objectiveInfo()[ObjectiveField]).toUInt();
}

QString ObjectiveBox::currentObjectiveDescription() const
{ // 返回物镜描述文本
    return objectiveInfo()[ObjectiveDescripField];
}

void ObjectiveBox::importExperConfig(const QString &objectiveDescrip)
{ // objectiveDescrip = br4x
    QStringList objectives = {rbtn_loc1->text(),rbtn_loc2->text(),
            rbtn_loc3->text(),rbtn_loc4->text()}; // 当前的每个位置的物镜配置

    auto objectiveDes = objectiveDescrip; // br4x转4x,ph4x转4xPH
    LOG<<objectives<<objectiveDes; // ("4x", "4xPH", "10xPH", "40x") "4x"
    if (!objectives.contains(objectiveDes)) {
        // 可能没有这个硬件配置,就默认第1个
        rbtn_loc1->setChecked(true);
        return;
    }

    // 相等的那个为true
    rbtn_loc1->setChecked(rbtn_loc1->text() == objectiveDes);
    rbtn_loc2->setChecked(rbtn_loc2->text() == objectiveDes);
    rbtn_loc3->setChecked(rbtn_loc3->text() == objectiveDes);
    rbtn_loc4->setChecked(rbtn_loc4->text() == objectiveDes);

    // 手动触发下信号,去更新channelbox和timebox的选项是否可以勾选br或者ph
    onClicked();
}