/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-16 13:24:47
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 13:38:51
 * @FilePath: \EOS\component\include\datastructure\gradienthiddenanimation.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_GRADIENTHIDDENANIMATION_H
#define EOS_MAIN_CORE_PY_GRADIENTHIDDENANIMATION_H

#include "datastructure.h"

class COMPONENT_IMEXPORT GradientHiddenAnimation : public QPropertyAnimation
{
    Q_OBJECT
public:
    explicit GradientHiddenAnimation(QObject *parent = nullptr);
    QGraphicsOpacityEffect * effect() const;
private:
    QGraphicsOpacityEffect * meffect;
signals:

};

#endif //EOS_MAIN_CORE_PY_GRADIENTHIDDENANIMATION_H
