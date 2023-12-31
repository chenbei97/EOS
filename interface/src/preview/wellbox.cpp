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
    wellbrandCombo->setCurrentIndex(2);

    auto lay = new QHBoxLayout;
    lay->addWidget(manufacturerCombo);
    lay->addWidget(wellbrandCombo);
    lay->addStretch();

    setLayout(lay);
    setTitle(tr(WellBoxTitle));

    connect(wellbrandCombo,QOverload<int>::of(&ComboBox::currentIndexChanged),this,&WellBox::onWellbrandChanged);
    connect(manufacturerCombo,QOverload<int>::of(&ComboBox::currentIndexChanged),this,&WellBox::manufacturerChanged);
    connect(manufacturerCombo,QOverload<int>::of(&ComboBox::currentIndexChanged),this,&WellBox::onManufacturerChanged);
}

void WellBox::importWellInfo(int manufacturer, int brand)
{
    manufacturerCombo->setCurrentIndex(manufacturer); // 先更新厂家
    wellbrandCombo->setCurrentIndex(brand); // 再更新品牌
}

void WellBox::onWellbrandChanged(int option)
{
    emit wellbrandChanged(option);
    if (wellbrandCombo->currentText().contains(SlideField)) {
        emit welltypeChanged(1); // 载玻片类型
    } else  emit welltypeChanged(0); // 切换孔板类型
}

void WellBox::onManufacturerChanged(int option)
{
    wellbrandCombo->blockSignals(true);
    auto idx = wellbrandCombo->currentIndex(); // 原来的位置
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
    wellbrandCombo->setCurrentIndex(idx); // 切换厂家时要恢复原来的位置
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
    } else if (brand.count(SlideField)) { // 载玻片的wellsize字段无用
        m[WellsizeField] = -1;
    }

    return m;
}

int WellBox::wellSize() const
{
    auto brand = wellbrandCombo->currentText();
    QString id;

    // 把wellsize提取出来比较方便
    if (brand.count(WellSize384)) id = getIndexFromFields(WellSize384);
    else if (brand.count(WellSize24)) id = getIndexFromFields(WellSize24);
    else if (brand.count(WellSize96)) id = getIndexFromFields(WellSize96);
    else if (brand.count(WellSize6) && !brand.count(WellSize96)){
        id = getIndexFromFields(WellSize6); // 96包含了6,所以包含6不能说明就是6,还不能是96
    } else if (brand.count(SlideField)) { // 载玻片的wellsize字段无用
        id = "-1";
    }
    return id.toInt();
}

int WellBox::wellBrand() const
{
    return getIndexFromFields(wellbrandCombo->currentText()).toInt();
}

int WellBox::wellManufacturer() const
{
    return getIndexFromFields(manufacturerCombo->currentText()).toInt();
}

QString WellBox::wellBrandName() const
{
    return wellbrandCombo->currentText();
}

QString WellBox::wellManufacturerName() const
{
    return manufacturerCombo->currentText();
}
