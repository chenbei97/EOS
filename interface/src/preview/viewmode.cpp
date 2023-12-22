/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-04 13:48:06
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 10:12:06
 * @FilePath: \EOS\interface\src\preview\viewmode.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "viewmode.h"

ViewModeBox::ViewModeBox(QWidget *parent) : GroupBox(parent)
{
    groupMode = new RadioGroup(3,Qt::Vertical);
    groupMode->setText(QStringList()<<tr(PointModeField)<<tr(RectModeField)<<tr(WholeModeField));

    auto lay = new QHBoxLayout;
    lay->addWidget(groupMode);

    setLayout(lay);
    setTitle(tr(ViewSelectBoxTitle));

    connect(groupMode,&RadioGroup::radioClicked,this, &ViewModeBox::modeSelected);
}

void ViewModeBox::setEnabled(int option)
{ // 0表示孔板类型,1表示载玻片类型
    groupMode->setEnabled(1,!option);
    groupMode->setEnabled(2,true); // 区域模式任何时候都可用
    groupMode->setEnabled(3,!option);
    if (option) {
        groupMode->setChecked(2,true); // 如果是载玻片,区域模式的checked要设置上
        groupMode->emitSignals(2);// 这样才能触发信号
    }
}

ViewMode ViewModeBox::viewMode() const
{
    ViewMode mode;
    auto checkID = groupMode->checkedID();
    checkID--;
    mode = ViewMode(checkID);
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