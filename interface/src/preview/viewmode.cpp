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
    initObjects();
    initAttributes();
    initLayout();

    setTitle(tr(ViewSelectBoxTitle));
    connect(groupMode,&RadioGroup::radioClicked,this, &ViewModeBox::radioClick);
    connect(triangleMove,&TriangleMove::triangleClicked,this,&ViewModeBox::triangleClick);
    connect(enableGroupBtn,&PushButton::clicked,this,&ViewModeBox::enableGroup);
    connect(endGroupBtn,&PushButton::clicked,this,&ViewModeBox::endGroup);
    connect(groupType,&ComboBox::currentTextChanged,this,&ViewModeBox::toggleGroup);
}

void ViewModeBox::radioClick(int mode)
{
    if (mode == 1) {
        if (groupType->isEnabled()) {
            triangleMove->setEnabled(true);
        }
    } else {
        triangleMove->setEnabled(false);
    }
    emit modeSelected(mode);
}

void ViewModeBox::triangleClick(int option)
{
    if (groupMode->checkedID() == 1 && groupType->isEnabled()) {
        emit triangleClicked(option);
    }
}

void ViewModeBox::enableGroup()
{
    groupType->setEnabled(true);
    if (groupMode->checkedID() == 1)
        triangleMove->setEnabled(true);
    emit enableWellPattern(true);
}

void ViewModeBox::endGroup()
{
    groupType->setEnabled(false);
    triangleMove->setEnabled(false);
    emit enableWellPattern(false);
}

void ViewModeBox::toggleGroup(const QString &text)
{
    emit groupTypeChanged(text);
    auto idx = groupType->currentIndex();
    auto color = groupType->itemData(idx,Qt::BackgroundRole).toString();
    emit groupColorChanged(QColor(color));
    groupType->setStyleSheet(tr("QComboBox:!editable{background:%1}").arg(color));
}

void ViewModeBox::initAttributes()
{
    groupMode->setText(QStringList()<<tr(PointModeField)<<tr(AreaModeField)<<tr(WholeModeField));
    triangleMove->setTriangleGap(0);
    triangleMove->setEnabled(false);
    groupType->setEnabled(false);
    groupType->setCurrentIndex(-1);
    auto cmodel = static_cast<QStandardItemModel*>(groupType->model());
    Q_ASSERT(cmodel->rowCount() <= GroupTypeColors.count());
    for(int i = 0; i < cmodel->rowCount(); ++i) {
        cmodel->item(i)->setBackground(GroupTypeColors[i]);
        cmodel->item(i)->setData(GroupTypeColors[i],Qt::BackgroundRole);
    }
}

void ViewModeBox::initLayout()
{
    // 1. leftbox
    auto leftbox = new GroupBox;
    auto leftlay = new QHBoxLayout;
    leftlay->addWidget(groupMode);
    leftbox->setLayout(leftlay);

    // 2. rightbox
    auto rightbox = new GroupBox;
    auto rightlay = new QVBoxLayout;
    rightlay->addWidget(enableGroupBtn);
    rightlay->addWidget(groupType);
    rightlay->addWidget(endGroupBtn);
    rightbox->setLayout(rightlay);

    // 3. mainlay
    auto lay = new QHBoxLayout;
    lay->addWidget(leftbox);
    lay->addWidget(rightbox);
    lay->addWidget(triangleMove);
    lay->setSpacing(10);
    lay->addStretch();
    setLayout(lay);
}

void ViewModeBox::initObjects()
{
    groupMode = new RadioGroup(3,Qt::Vertical);
    triangleMove = new TriangleMove;
    groupType = new ComboBox(GroupTypeFields);
    enableGroupBtn = new PushButton(tr("create group"));
    endGroupBtn = new PushButton(tr("done"));
}

void ViewModeBox::setViewEnabled(int option)
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