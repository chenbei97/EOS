/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 09:33:29
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-31 09:43:42
 * @FilePath: \EOS\interface\include\preview\historybox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOSI_HISTORYBOX_H
#define EOSI_HISTORYBOX_H

#include "window.h"

class INTERFACE_IMEXPORT HistoryBox: public GroupBox
{
Q_OBJECT
public:
    explicit HistoryBox(QWidget*parent= nullptr);
private:
    PushButton * btn;
    CheckBox * checkbox;
    void onBtn();
    void onCheckBox(bool isChecked);
signals:
};

#endif //EOSI_HISTORYBOX_H
