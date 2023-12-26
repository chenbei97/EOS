/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-20 09:05:01
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 09:06:04
 * @FilePath: \EOS\interface\src\preview\autofocus.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "focusbox.h"

FocusBox::FocusBox(QWidget*parent):GroupBox(parent)
{
    initObjects();
    initLayout();
    resize(DefaultWindowSize.width()*2,DefaultWindowSize.height());

    connect(autoFocus,&CheckBox::checkedChanged,mainbox,&GroupBox::setEnabled);
    connect(custom,&CheckBox::checkedChanged,custombox,&GroupBox::setEnabled);
    setTitle(FocusBoxTitle);
}

void FocusBox::initLayout()
{
    auto formlay = new QFormLayout;
    formlay->addRow(tr(FocusSkipHolesLabelField),skipholes);
    formlay->addRow(tr(FocusSkipViewsLabelField),skipviews);
    formlay->addRow(tr(FocusExtendRangeLabelField),extendRange);
    formlay->addRow(tr(FocusAllCycleField),cycle);
    formlay->addRow(tr(FocusAllChannelsField),allChannels);
    formlay->addRow(tr(CustomField),custom);

    auto boxlay = new QVBoxLayout;
    boxlay->addLayout(formlay);
    boxlay->addWidget(custombox);
    mainbox->setLayout(boxlay);

    auto lay = new QVBoxLayout;
    lay->addWidget(autoFocus);
    lay->addWidget(mainbox);
    setLayout(lay);
}

void FocusBox::initObjects()
{
    autoFocus = new CheckBox(tr(FocusAutoFocusLabelField),true);
    mainbox = new GroupBox;

    skipholes = new SpinBox;
    skipviews = new SpinBox;
    extendRange = new CheckBox("",false);
    cycle = new CheckBox("",false);
    allChannels = new CheckBox("",false);
    custom = new CheckBox("",false);
    custombox = new GroupBox(CustomField);

    custombox->setEnabled(false);
}

int FocusBox::skipHoles() const
{
    if (autoFocus->isChecked())
        return skipholes->value();
    else return 0;
}

FocusInfo FocusBox::focusInfo() const
{
    FocusInfo info;

    info[AutoFocusField] = isAutoFocus();
    info[SkipHolesField] = skipHoles();
    info[SkipViewsField] = skipViews();
    info[ExtendRangeField] = isExtendRange();
    info[AllCycleField] = isAllCycle();
    info[AllChannelsField] = isAllChannels();

    return info;
}

int FocusBox::skipViews() const
{
    if (isAutoFocus())
        return skipviews->value();
    else return 0;
}

bool FocusBox::isAutoFocus() const
{
    return autoFocus->isChecked();
}

bool FocusBox::isAllChannels() const
{
    if (!isAutoFocus()) return false;
    return allChannels->isChecked();
}

bool FocusBox::isAllCycle() const
{
    if (!isAutoFocus()) return false;
    return cycle->isChecked();
}

bool FocusBox::isExtendRange() const
{
    if (!isAutoFocus()) return false;
    return extendRange->isChecked();
}
