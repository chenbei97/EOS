/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-20 09:04:42
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 09:05:51
 * @FilePath: \EOS\interface\include\preview\autofocus.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_FOCUSBOX_H
#define EOS_FOCUSBOX_H

#include "window.h"

class INTERFACE_IMEXPORT FocusBox : public GroupBox
{
    Q_OBJECT
public:
    explicit FocusBox(QWidget*parent= nullptr);
    int skipHoles() const;
    int skipViews() const;
    bool isAutoFocus() const;
    bool isExtendRange() const;
    bool isAllChannels() const;
    bool isAllCycle() const;
    FocusInfo focusInfo() const;
private:
    void initObjects();
    void initLayout();
private:
    CheckBox * autoFocus;
    GroupBox * mainbox;

    SpinBox * skipholes;
    SpinBox * skipviews;
    CheckBox * extendRange;
    CheckBox * cycle;
    CheckBox * allChannels;
    CheckBox * custom;
    GroupBox * custombox;
signals:
};

#endif //EOS_FOCUSBOX_H
