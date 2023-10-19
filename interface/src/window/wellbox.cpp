/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 11:16:44
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-19 13:36:05
 * @FilePath: \EOS\interface\src\window\wellbox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "wellbox.h"

WellBox::WellBox(QWidget*parent): GroupBox(parent)
{
    wellbrandCombo = new ComboBox(QStringList()<<"brand1"<<"brand2"<<"brand3");
    wellsizeCombo = new ComboBox(WellsizeFields);
    welldishCombo = new ComboBox(QStringList()<<"dish1"<<"dish2"<<"dish3");
    wellflaskCombo = new ComboBox(QStringList()<<"flash1"<<"flash2"<<"flash3");
    wellslideCombo = new ComboBox(QStringList()<<"slide1"<<"slide2"<<"slide3");
    objectiveCombo = new ComboBox(ObjectiveFields);

    auto gridlay = new QGridLayout;
    gridlay->addWidget(new Label("brand: "),0,0);
    gridlay->addWidget(wellbrandCombo,0,1);
    gridlay->addWidget(new Label("wellsize: "),0,2);
    gridlay->addWidget(wellsizeCombo,0,3);
    gridlay->addWidget(new Label("objective: "),0,4);
    gridlay->addWidget(objectiveCombo,0,5);

    gridlay->addWidget(new Label("dish: "),1,0);
    gridlay->addWidget(welldishCombo,1,1);
    gridlay->addWidget(new Label("flask: "),1,2);
    gridlay->addWidget(wellflaskCombo,1,3);
    gridlay->addWidget(new Label("slide: "),1,4);
    gridlay->addWidget(wellslideCombo,1,5);

    gridlay->setColumnStretch(0,1);
    gridlay->setColumnStretch(1,2);
    gridlay->setColumnStretch(3,1);
    gridlay->setColumnStretch(4,2);
    gridlay->setColumnStretch(5,1);
    gridlay->setColumnStretch(6,2);

    gridlay->setHorizontalSpacing(WellBoxGridLayHorSpacing);
    setLayout(gridlay);

    connect(wellbrandCombo,QOverload<int>::of(&ComboBox::currentIndexChanged),this,&WellBox::wellbrandChanged);
    connect(wellsizeCombo,QOverload<int>::of(&ComboBox::currentIndexChanged),this,&WellBox::wellsizeChanged);
    connect(wellflaskCombo,QOverload<int>::of(&ComboBox::currentIndexChanged),this,&WellBox::wellflaskChanged);
    connect(wellslideCombo,QOverload<int>::of(&ComboBox::currentIndexChanged),this,&WellBox::wellslideChanged);
    connect(welldishCombo,QOverload<int>::of(&ComboBox::currentIndexChanged),this,&WellBox::welldishChanged);
    connect(objectiveCombo,QOverload<int>::of(&ComboBox::currentIndexChanged),this,&WellBox::objectiveChanged);
}