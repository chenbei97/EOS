/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-17 17:25:19
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 10:12:10
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
    void addToList(GroupBox*box,RadioButton*radio);
    void removeFromList(GroupBox*box,RadioButton*radio);
private:
    QMap<int,QPair<GroupBox*,RadioButton*>> notifiers;
};

#endif //EOS_SETTINGNOTIFIER_H
