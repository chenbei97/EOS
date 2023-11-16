/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-16 13:25:25
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 13:39:00
 * @FilePath: \EOS\component\src\datastructure\gradienthiddenanimation.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "gradienthiddenanimation.h"

GradientHiddenAnimation::GradientHiddenAnimation(QObject *parent) : QPropertyAnimation(parent)
{
    meffect = new QGraphicsOpacityEffect;
    meffect->setOpacity(DefaultOpacityValue);

    setTargetObject(meffect);
    setPropertyName(OpacityField);
    setEasingCurve(DefaultEasingCurve);
    setDuration(DafaultEffectDuration);
    setStartValue(DoubleZeroValue);
    setEndValue(DefaultOpacityValue);
}

QGraphicsOpacityEffect * GradientHiddenAnimation::effect() const
{
    return meffect;
}