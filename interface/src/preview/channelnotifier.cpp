/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-20 08:44:55
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-20 09:31:25
 * @FilePath: \EOS\interface\src\preview\channelnotifier.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "channelnotifier.h"


void ChannelNotifier::addToList(PushButton*button)
{
    auto id = button->getID();
    if (id != -1)
    {
        auto iter = notifiers.find(id);// 寻找这个通知者
        if (iter == notifiers.end()) // 如果没有这个通知者
            notifiers.insert(id, button);
    }
}

void ChannelNotifier::removeFromList(PushButton *button)
{
    auto id = button->getID();
    if (id != -1)
    {
        auto iter = notifiers.find(id);// 寻找这个通知者
        if (iter != notifiers.end()) // 如果有这个通知者
            notifiers.remove(id);
    }
}

void ChannelNotifier::notify(PushButton *btn)
{
    auto id = btn->getID();
    if (id != -1)
    {
        for(auto button: notifiers.values()) {
            if (id != button->getID()) { // 其它的一定关灯,这里不额外下发命令,服务端收到某个通道开灯时它有义务关闭其它灯
                button->setChecked(false);
                button->resetBackGroundColor();
                //emit channelChanged(-1);
            } else {
                !button->isChecked()?button->setBackGroundColor(Qt::yellow) // 设置背景颜色时内部切换了checked状态
                    :button->resetBackGroundColor();
                button->isChecked()? emit channelChanged(button->getID())
                    : emit channelChanged(-1);
                if (!button->isChecked()) {
                    QVariantMap m;
                    m[CurrentChannelField] = button->getID();
                    m[TurnOffLight] = 1;
                    m[BrightField] = -1;
                    SocketPointer->exec(TcpFramePool.toggleChannelEvent,assembleToggleChannelEvent(m));
                    if (ParserResult.toBool()) {
                        LOG<<"关闭"<<ChannelFields[button->getID()];
                    }
                }
            }
        }
    }
}