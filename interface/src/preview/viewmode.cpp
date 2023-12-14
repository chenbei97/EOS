/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-04 13:48:06
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-14 13:53:59
 * @FilePath: \EOS\interface\src\preview\viewmode.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "viewmode.h"

ViewModeBox::ViewModeBox(QWidget *parent) : GroupBox(parent)
{
    pointMode = new QRadioButton(tr(PointModeField));
    rectMode = new QRadioButton(tr(RectModeField));
    wholeMode = new QRadioButton(tr(WholeModeField));
    pointMode->setChecked(true);

    auto lay = new QHBoxLayout;
    lay->addWidget(pointMode);
    lay->addWidget(rectMode);
    lay->addWidget(wholeMode);

    setLayout(lay);
    setTitle(tr(ViewSelectBoxTitle));

    auto group = new QButtonGroup;
    group->addButton(pointMode,0);
    group->addButton(rectMode,1);
    group->addButton(wholeMode,2);

    connect(group,QOverload<int>::of(&QButtonGroup::buttonClicked),this, &ViewModeBox::modeSelected);
}

void ViewModeBox::setEnabled(int option)
{ // 0表示孔板类型,1表示载玻片类型
    pointMode->setEnabled(!option);
    rectMode->setEnabled(true);
    wholeMode->setEnabled(!option);
    if (option) {
        rectMode->setChecked(true);
        rectMode->click(); // 这样才能触发信号
    }
}

ViewMode ViewModeBox::viewMode() const
{
    ViewMode mode;
    if (pointMode->isChecked())
        mode = ViewMode::PointMode;
    else if (rectMode->isChecked())
        mode = ViewMode::RectMode;
    else if (wholeMode->isChecked())
        mode = ViewMode::WholeMode;
    return mode;
}

ViewModeInfo ViewModeBox::viewModeInfo() const
{
    ViewModeInfo info;
    switch (viewMode()) {
        case ViewMode::PointMode:
            info[ViewModeField] = QString::number(0);
            break;
        case ViewMode::RectMode:
            info[ViewModeField] = QString::number(1);
            break;
        case ViewMode::WholeMode:
            info[ViewModeField] = QString::number(2);
            break;
    }
    return info;
}