/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-20 08:44:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-20 09:31:33
 * @FilePath: \EOS\interface\include\preview\channelnotifier.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef CHANNELNOTIFIER_H
#define CHANNELNOTIFIER_H

#include "window.h"

class INTERFACE_IMEXPORT ChannelNotifier: public QObject
{
    Q_OBJECT
public:
    void notify(PushButton*btn);
    void addToList(PushButton*btn);
    void removeFromList(PushButton*btn);
private:
    QMap<int,PushButton*> notifiers;
signals:
    void channelChanged(int option);
};

#endif //CHANNELNOTIFIER_H
