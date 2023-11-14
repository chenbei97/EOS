/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 11:16:44
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-14 10:05:25
 * @FilePath: \EOS\interface\src\preview\wellbox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "wellbox.h"

WellBox::WellBox(QWidget*parent): GroupBox(parent)
{
    wellbrandCombo = new ComboBox(Brand1Fields);
    manufacturerCombo = new ComboBox(ManufacturerFields);

    auto lay = new QHBoxLayout;
    lay->addWidget(manufacturerCombo);
    lay->addWidget(wellbrandCombo);

    setLayout(lay);
    setTitle(tr("Wellsize"));

    connect(wellbrandCombo,QOverload<int>::of(&ComboBox::currentIndexChanged),this,&WellBox::wellbrandChanged);
    connect(manufacturerCombo,QOverload<int>::of(&ComboBox::currentIndexChanged),this,&WellBox::manufacturerChanged);
    connect(manufacturerCombo,QOverload<int>::of(&ComboBox::currentIndexChanged),this,&WellBox::onManufacturerChange);
}

void WellBox::importExperConfig(int manufacturer, int brand)
{
    manufacturerCombo->setCurrentIndex(manufacturer); // 先更新厂家
    wellbrandCombo->setCurrentIndex(brand); // 再更新品牌
}

void WellBox::onManufacturerChange(int option)
{
    wellbrandCombo->blockSignals(true);
    wellbrandCombo->clear();
    switch (option) {
        case 0:
            wellbrandCombo->addItems(Brand1Fields);
            break;
        case 1:
            wellbrandCombo->addItems(Brand2Fields);
            break;
        case 2:
            wellbrandCombo->addItems(Brand3Fields);
            break;
        case 3:
            wellbrandCombo->addItems(Brand4Fields);
            break;
    }
    wellbrandCombo->blockSignals(false);
}

WellInfo WellBox::wellInfo() const
{
    WellInfo m;

    auto brand = wellbrandCombo->currentText();

    m[BrandField] = getIndexFromFields(brand);
    m[ManufacturerField] = getIndexFromFields(manufacturerCombo->currentText());

    // 把wellsize提取出来比较方便
    if (brand.count(WellSize384)) m[WellsizeField] = getIndexFromFields(WellSize384);
    else if (brand.count(WellSize24)) m[WellsizeField] = getIndexFromFields(WellSize24);
    else if (brand.count(WellSize96)) m[WellsizeField] = getIndexFromFields(WellSize96);
    else if (brand.count(WellSize6) && !brand.count(WellSize96)){
        m[WellsizeField] = getIndexFromFields(WellSize6); // 96包含了6,所以包含6不能说明就是6,还不能是96
    }

    return m;
}

