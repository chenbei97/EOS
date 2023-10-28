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

class INTERFACE_IMEXPORT FocusBox: public GroupBox
{
    Q_OBJECT
public:
    explicit FocusBox(QWidget*parent= nullptr);
    void setEnabled(bool enabled);
    int focus() const;
private:
    DoubleSlider * slider;
    RoundButton * leftbtn;
    RoundButton * rightbtn;
    SpinBox * step;
    PushButton * autofocusbtn;
    void addFocus();
    void subtractFocus();
    void onAutoFocus();
};

#endif //EOSI_FOCUSBOX_H