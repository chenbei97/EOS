/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-24 09:43:19
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-24 09:46:18
 * @FilePath: \EOS\interface\include\preview\focustool.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_FOCUSTOOL_H
#define EOSI_FOCUSTOOL_H

#include "window.h"

class INTERFACE_IMEXPORT FocusTool: public QWidget
{
    Q_OBJECT
public:
    explicit FocusTool(QWidget*parent= nullptr);
private:
    Slider * slider;
    RoundButton * leftbtn;
    RoundButton * rightbtn;
    SpinBox * step;
    void addFocus();
    void subtractFocus();
};

#endif //EOSI_FOCUSTOOL_H
