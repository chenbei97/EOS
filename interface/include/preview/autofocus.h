/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-20 09:04:42
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 09:05:51
 * @FilePath: \EOS\interface\include\preview\autofocus.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_AUTOFOCUS_H
#define EOS_AUTOFOCUS_H

#include "window.h"

class INTERFACE_IMEXPORT AutoFocus : public GroupBox
{
    Q_OBJECT
public:
    explicit AutoFocus(QWidget*parent= nullptr);
    int skipHoles() const;
    int skipViews() const;
private:
    void initObjects();
    void initLayout();
    void apply();
private:
    CheckBox * enableAutoFocus;
    GroupBox * settingbox;
    GroupBox * custombox;
    CheckBox * enableExtendRange;
    RadioGroup * radioGroup;
    SpinBox * skipholes;
    SpinBox * skipviews;
    PushButton * applybtn;
signals:
};

#endif //EOS_AUTOFOCUS_H
