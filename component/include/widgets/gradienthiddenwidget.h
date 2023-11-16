/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-16 13:18:11
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 13:38:38
 * @FilePath: \EOS\component\include\widgets\gradienthiddenwidget.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_GRADIENTHIDDENWIDGET_H
#define EOS_MAIN_CORE_PY_GRADIENTHIDDENWIDGET_H

#include "widgets.h"
#include "gradienthiddenanimation.h"

class COMPONENT_IMEXPORT GradientHiddenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GradientHiddenWidget(QWidget *parent = nullptr);
    void showEvent(QShowEvent *event) override;
    void enableEffect(bool enable);
protected:
    QVBoxLayout * mainlay;
private:
    GradientHiddenAnimation * animation;
    bool enabled = true;
signals:

};

#endif //EOS_MAIN_CORE_PY_GRADIENTHIDDENWIDGET_H
