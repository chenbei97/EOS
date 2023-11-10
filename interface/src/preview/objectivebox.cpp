/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-23 13:55:14
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-01 17:13:02
 * @FilePath: \EOS\interface\src\preview\objectivebox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "objectivebox.h"

ObjectiveBox::ObjectiveBox(QWidget *parent): GroupBox(parent)
{
    // 这里初始化文字时需要跟objectivesetting.cpp初始化的4个镜头选项保持一致
    rbtn_loc1 = new QRadioButton(Bright4xField);
    rbtn_loc2 = new QRadioButton(PH4xField);
    rbtn_loc3 = new QRadioButton(Bright10xField);
    rbtn_loc4 = new QRadioButton(PH10xField);

    location_button[CameraLocationField1] = rbtn_loc1;
    location_button[CameraLocationField2] = rbtn_loc2;
    location_button[CameraLocationField3] = rbtn_loc3;
    location_button[CameraLocationField4] = rbtn_loc3;

    QHBoxLayout * lay = new QHBoxLayout;
    lay->addWidget(rbtn_loc1);
    lay->addWidget(rbtn_loc2);
    lay->addWidget(rbtn_loc3);
    lay->addWidget(rbtn_loc4);

    rbtn_loc1->setChecked(true);

    setLayout(lay);
    setTitle(tr("物镜"));

    connect(rbtn_loc1,&QRadioButton::clicked,this,&ObjectiveBox::onClicked);
    connect(rbtn_loc2,&QRadioButton::clicked,this,&ObjectiveBox::onClicked);
    connect(rbtn_loc3,&QRadioButton::clicked,this,&ObjectiveBox::onClicked);
    connect(rbtn_loc4,&QRadioButton::clicked,this,&ObjectiveBox::onClicked);
}

void ObjectiveBox::onObjectiveSettingChanged(const LocationObjectiveInfo &m)
{ // 每个镜头放的物镜型号
    rbtn_loc1->setText(m[CameraLocationField1]);
    rbtn_loc2->setText(m[CameraLocationField2]);
    rbtn_loc3->setText(m[CameraLocationField3]);
    rbtn_loc4->setText(m[CameraLocationField4]);

    // 可能有无镜头的选项
    if (m.values().contains(NoneField)) {
        auto loc = m.key(NoneField); // 找到无镜头的位置
        location_button[loc]->setEnabled(false); // none不能选
        foreach(auto cloc, m.keys()) {
            if (cloc != loc) {
                location_button[cloc]->setEnabled(true); // 其它的要恢复
            }
            location_button[cloc]->setChecked(true);//从none移开,选择个有效的就可触发更新ui信息即可
            location_button[cloc]->click();// click才能触发
        }
    } else { // 没包含none
        foreach(auto cloc, m.keys()) {
               location_button[cloc]->setEnabled(true); // 可能第1次有none,第2次没有none要全部恢复
        }
        rbtn_loc1->setChecked(true); // 希望切换物镜硬件配置时能够确保UI界面信息的更新,手动触发
        rbtn_loc1->click(); // click才能触发
    }
}

void ObjectiveBox::onClicked()
{
    if (rbtn_loc1->isChecked())
       emit objectiveChanged(rbtn_loc1->text());
    else if (rbtn_loc2->isChecked())
       emit objectiveChanged(rbtn_loc2->text());
    else if (rbtn_loc3->isChecked())
       emit objectiveChanged(rbtn_loc3->text());
    else if (rbtn_loc4->isChecked())
       emit objectiveChanged(rbtn_loc4->text());
}

ObjectiveInfo ObjectiveBox::objectiveInfo() const
{
    ObjectiveInfo m;

    if (rbtn_loc1->isChecked()) {
        m[ObjectiveDescripField] = rbtn_loc1->text(); // 传递实际字符串 br4x
        m[CameraLocationField] = QString::number(CameraLocationField1Index);

    }
    else if (rbtn_loc2->isChecked()) {
        m[ObjectiveDescripField] = rbtn_loc2->text();
        m[CameraLocationField] = QString::number(CameraLocationField2Index);
    }
    else if (rbtn_loc3->isChecked()) {
        m[ObjectiveDescripField] = rbtn_loc3->text();
        m[CameraLocationField] = QString::number(CameraLocationField3Index);
    }
    else if (rbtn_loc4->isChecked()) {
        m[ObjectiveDescripField] = rbtn_loc4->text();
        m[CameraLocationField] = QString::number(CameraLocationField4Index);
    }

    //LOG<<m; // 注意! : 由于objectivesetting信号是异步的,此时rbtn_loc1其实并未赋值,所以构造函数初始化时要保持一致

    // objective存放原字符串
    if (m[ObjectiveDescripField].contains(Objective4x))
        m[ObjectiveField]=Objective4x;
    else if (m[ObjectiveDescripField].contains(Objective10x))
        m[ObjectiveField]=Objective10x;
    else if (m[ObjectiveDescripField].contains(Objective20x))
        m[ObjectiveField]=Objective20x;
    else if (m[ObjectiveDescripField].contains(Objective40x))
        m[ObjectiveField]=Objective40x;

    // 同理增加,ObjectiveTypeField
    if (m[ObjectiveDescripField].contains(ObjectivePH))
        m[ObjectiveTypeField]=getIndexFromFields(ObjectivePH);
    else if (m[ObjectiveDescripField].contains(ObjectiveBR))
        m[ObjectiveTypeField]=getIndexFromFields(ObjectiveBR);

    return m;
}

void ObjectiveBox::importExperConfig(const QString &objective)
{
    QStringList objectives = {rbtn_loc1->text(),rbtn_loc2->text(),
            rbtn_loc3->text(),rbtn_loc4->text()};
    if (!objectives.contains(objective)) {
        // 可能没有这个硬件配置,就默认第1个
        rbtn_loc1->setChecked(true);
        return;
    }
    // 相等的那个为true
    rbtn_loc1->setChecked(rbtn_loc1->text() == objective);
    rbtn_loc2->setChecked(rbtn_loc2->text() == objective);
    rbtn_loc3->setChecked(rbtn_loc3->text() == objective);
    rbtn_loc4->setChecked(rbtn_loc4->text() == objective);

    // 手动触发下信号,去更新channelbox和timebox的选项是否可以勾选br或者ph
    onClicked();
}