/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-17 17:25:29
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 10:11:23
 * @FilePath: \EOS\interface\src\setting\settingnotifier.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "settingnotifier.h"

void SettingNotifier::addToList(GroupBox*box,RadioButton*radio)
{
    auto id = box->getID();
    if (id != -1)
    {
        auto iter = notifiers.find(id);// 寻找这个通知者
        if (iter == notifiers.end()) // 如果没有这个通知者
            notifiers.insert(id, qMakePair(box,radio));
    }
}

void SettingNotifier::removeFromList(GroupBox *box,RadioButton*radio)
{
    auto id = box->getID();
    if (id != -1)
    {
        auto iter = notifiers.find(id);// 寻找这个通知者
        if (iter != notifiers.end()) // 如果有这个通知者
            notifiers.remove(id);
    }
}

void SettingNotifier::notify(GroupBox *box)
{ // 不通知自身,其它的所有box都要通知
    auto id = box->getID();
    if (id != -1)
    {
        for(auto pair: notifiers.values()) {
            if (id != pair.first->getID()) {
                pair.first->setHidden(true); // 通知结果是除了自身其它都隐藏
                pair.second->setChecked(false);
            } else {
                pair.first->setHidden(false);
                pair.second->setChecked(true);
            }
        }
    }
}