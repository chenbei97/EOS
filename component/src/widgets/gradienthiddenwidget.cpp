/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-16 13:18:21
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 13:38:27
 * @FilePath: \EOS\component\src\widgets\gradienthiddenwidget.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "gradienthiddenwidget.h"

GradientHiddenWidget::GradientHiddenWidget(QWidget *parent) : QWidget(parent)
{
    enableEffect(true);
    animation = new GradientHiddenAnimation(this);
    setGraphicsEffect(animation->effect());
    mainlay =  new QVBoxLayout(this);
    setLayout(mainlay);
}

void GradientHiddenWidget::showEvent(QShowEvent *event)
{
    if (enabled)
        animation->start();
    return QWidget::showEvent(event);
}

void GradientHiddenWidget::enableEffect(bool enable)
{
    enabled = enable;
}

