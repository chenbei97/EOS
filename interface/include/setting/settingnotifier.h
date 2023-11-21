/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-17 17:25:19
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-17 17:27:16
 * @FilePath: \EOS\interface\include\setting\settingnotifier.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_SETTINGNOTIFIER_H
#define EOS_SETTINGNOTIFIER_H

#include "window.h"

class INTERFACE_IMEXPORT SettingNotifier : public QObject
{
    Q_OBJECT
public:
    void notify(GroupBox*box);
    void addToList(GroupBox*box,QRadioButton*radio);
    void removeFromList(GroupBox*box,QRadioButton*radio);
private:
    QMap<int,QPair<GroupBox*,QRadioButton*>> notifiers;
};

#endif //EOS_SETTINGNOTIFIER_H
