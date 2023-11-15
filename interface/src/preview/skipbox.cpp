/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-15 10:44:17
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-15 10:50:58
 * @FilePath: \EOS\interface\src\preview\skipbox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "skipbox.h"

SkipBox::SkipBox(QWidget *parent) : GroupBox(parent)
{
    skipholes = new SpinBox;
    skipviews = new SpinBox;

    auto formlay = new QFormLayout;
    formlay->addRow(FocusSkipHolesLabelField,skipholes);
    formlay->addRow(FocusSkipViewsLabelField,skipviews);

    skipholes->setMaximumWidth(TimeBoxSpinBoxMaxWidth);
    skipviews->setMaximumWidth(TimeBoxSpinBoxMaxWidth);

    setLayout(formlay);
    setTitle(tr(FocusxField));
}

int SkipBox::skipHoles() const
{
    return skipholes->value();
}

int SkipBox::skipViews() const
{
    return skipviews->value();
}
