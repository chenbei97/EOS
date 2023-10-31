/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-23 13:55:14
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-23 14:13:59
 * @FilePath: \EOS\interface\src\window\objectivebox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "objectivebox.h"

ObjectiveBox::ObjectiveBox(QWidget *parent): GroupBox(parent)
{
    rbtn4 = new QRadioButton(Objective4x);
    rbtn10 = new QRadioButton(Objective10x);
    rbtn20 = new QRadioButton(Objective20x);
    rbtn40 = new QRadioButton(Objective40x);

    QHBoxLayout * lay = new QHBoxLayout;
    lay->addWidget(rbtn4);
    lay->addWidget(rbtn10);
    lay->addWidget(rbtn20);
    lay->addWidget(rbtn40);

//    QButtonGroup * group = new QButtonGroup;
//    group->addButton(rbtn4,0);
//    group->addButton(rbtn10,1);
//    group->addButton(rbtn20,2);
//    group->addButton(rbtn40,3);
    rbtn4->setChecked(true);

    setLayout(lay);
    setTitle(tr("物镜"));

    //connect(group,QOverload<int>::of(&QButtonGroup::buttonClicked),this,&ObjectiveBox::objectiveChanged);
    connect(rbtn4,&QRadioButton::clicked,this,&ObjectiveBox::onClicked);
    connect(rbtn10,&QRadioButton::clicked,this,&ObjectiveBox::onClicked);
    connect(rbtn20,&QRadioButton::clicked,this,&ObjectiveBox::onClicked);
    connect(rbtn40,&QRadioButton::clicked,this,&ObjectiveBox::onClicked);
}

void ObjectiveBox::onClicked()
{
    if (rbtn4->isChecked())
       emit objectiveChanged(0);
    else if (rbtn10->isChecked())
       emit objectiveChanged(1);
    else if (rbtn20->isChecked())
       emit objectiveChanged(2);
    else if (rbtn40->isChecked())
       emit objectiveChanged(3);
}

ObjectiveInfo ObjectiveBox::objectiveInfo() const
{
    ObjectiveInfo m;

    if (rbtn4->isChecked())
        m[ObjectiveField] = getIndexFromFields(Objective4x); // 转换为idx无需在这里,这里需要实际字符串
    else if (rbtn10->isChecked())
        m[ObjectiveField] = getIndexFromFields(Objective10x);
    else if (rbtn20->isChecked())
        m[ObjectiveField] = getIndexFromFields(Objective20x);
    else if (rbtn40->isChecked())
        m[ObjectiveField] = getIndexFromFields(Objective40x);

    return m;
}

