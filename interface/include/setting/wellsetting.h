/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-14 15:10:07
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 10:11:48
 * @FilePath: \EOS\interface\include\setting\wellsetting.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_WELLSETTING_H
#define EOS_WELLSETTING_H

#include "window.h"

class INTERFACE_IMEXPORT WellSetting: public GroupBox
{
    Q_OBJECT
public:
    explicit WellSetting(QWidget *parent = nullptr);
private:
    void initObjects();
    void initAttributes();
    void initLayout();
private:
    GroupBox * boardbox;
    RadioGroup * radioGroup;
};

#endif //EOS_WELLSETTING_H
