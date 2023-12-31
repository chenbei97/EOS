/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-24 09:43:19
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-24 09:46:18
 * @FilePath: \EOS\interface\include\preview\focustool.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_FOCUSBOX_H
#define EOSI_FOCUSBOX_H

#include "window.h"
#include "focusbox.h"

class INTERFACE_IMEXPORT FocusBox: public GroupBox
{
    Q_OBJECT
public:
    explicit FocusBox(QWidget*parent= nullptr);
    void importExperConfig(double focus,double step);
    void setEnabled(bool enabled);
    double focus() const;
    double focusStep() const;
    FocusInfo focusInfo() const;
private:
    void initObjects();
    void initAttributes();
    void initLayout();
private:
    TriangleMove * pattern;
    DoubleSlider * slider;
    RoundButton * topbtn;
    RoundButton * bottombtn;
    SpinBox * step;
    PushButton * autofocusbtn;
    AutoFocus * autofocusdlg;
    void addFocus();
    void subtractFocus();
    void onAutoFocus();
signals:
    void leftMove();
    void rightMove();
    void topMove();
    void bottomMove();
    void directionMove(int option);
    void valueChanged(double val);
    void autoFocus();
};

#endif //EOSI_FOCUSBOX_H
