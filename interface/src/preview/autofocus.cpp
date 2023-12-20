/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-20 09:05:01
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 09:06:04
 * @FilePath: \EOS\interface\src\preview\autofocus.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "autofocus.h"

AutoFocus::AutoFocus(QWidget*parent):GroupBox(parent)
{
    initObjects();
    initLayout();
    resize(DefaultWindowSize.width()*2,DefaultWindowSize.height());

    connect(enableAutoFocus,&CheckBox::checkedChanged,[=](auto checked){settingbox->setEnabled(checked);});
    connect(applybtn,&PushButton::clicked,this,&AutoFocus::apply);
    connect(radioGroup,&RadioGroup::radioClicked,[=](auto id) {id--;custombox->setEnabled(id);});
}

void AutoFocus::apply()
{
    emit autoFocus();
}

void AutoFocus::initLayout()
{
    auto formlay = new QFormLayout;
    formlay->addRow(tr(FocusSkipHolesLabelField),skipholes);
    formlay->addRow(tr(FocusSkipViewsLabelField),skipviews);
    formlay->addRow(tr(FocusExtendRangeLabelField),enableExtendRange);

    auto hlay = new QHBoxLayout;
    hlay->addWidget(new Label(tr(FocusFocusModeLabelField)));
    hlay->addWidget(radioGroup);
    hlay->addStretch();

    auto customlay = new QVBoxLayout;
    customlay->addWidget(new CheckBox);
    custombox->setLayout(customlay);

    auto boxlay = new QVBoxLayout;
    boxlay->addLayout(formlay);
    boxlay->addLayout(hlay);
    boxlay->addWidget(custombox);
    settingbox->setLayout(boxlay);

    auto btnlay = new QHBoxLayout;
    btnlay->addStretch();
    btnlay->addWidget(applybtn);

    auto lay = new QVBoxLayout;
    lay->addWidget(enableAutoFocus);
    lay->addWidget(settingbox);
    lay->addLayout(btnlay);
    setLayout(lay);
}

void AutoFocus::initObjects()
{
    enableAutoFocus = new CheckBox(tr(FocusAutoFocusLabelField),true);
    settingbox = new GroupBox;
    skipholes = new SpinBox;
    skipviews = new SpinBox;
    enableExtendRange = new CheckBox("",false);
    radioGroup = new RadioGroup;
    custombox = new GroupBox;
    applybtn = new PushButton(tr(ApplyField));

    custombox->setEnabled(false);
    radioGroup->setText(QStringList()<<tr(AllChannelsField)<<tr(CustomField));
}

int AutoFocus::skipHoles() const
{
    return skipholes->value();
}

int AutoFocus::skipViews() const
{
    return skipviews->value();
}
