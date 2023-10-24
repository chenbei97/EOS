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
    wellbrandCombo = new ComboBox(BrandFields);
    manufacturerCombo = new ComboBox(ManufacturerFields);

    auto lay = new QHBoxLayout;
    lay->addWidget(manufacturerCombo);
    lay->addWidget(wellbrandCombo);

    setLayout(lay);
    setTitle(tr("孔板"));

    connect(wellbrandCombo,QOverload<int>::of(&ComboBox::currentIndexChanged),this,&WellBox::wellbrandChanged);
    connect(manufacturerCombo,QOverload<int>::of(&ComboBox::currentIndexChanged),this,&WellBox::manufacturerChanged);
}

WellInfo WellBox::wellInfo() const
{
    WellInfo m;

    m[BrandField] = wellbrandCombo->currentText();
    m[ManufacturerField] = manufacturerCombo->currentText();

    return m;
}

