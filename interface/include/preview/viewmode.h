/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-04 13:47:39
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-14 13:54:05
 * @FilePath: \EOS\interface\include\preview\viewmode.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_VIEWMODE_H
#define EOS_MAIN_CORE_PY_VIEWMODE_H

#include "window.h"

class INTERFACE_IMEXPORT ViewModeBox: public GroupBox
{
    Q_OBJECT
public:
    explicit ViewModeBox(QWidget*parent= nullptr);
    void setEnabled(int option);
    ViewMode viewMode() const;
    ViewModeInfo viewModeInfo() const;
private:
    QRadioButton * pointMode;
    QRadioButton * rectMode;
    QRadioButton * wholeMode;
signals:
    void modeSelected(int mode);
};

#endif //EOS_MAIN_CORE_PY_VIEWMODE_H
