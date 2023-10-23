/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 10:19:49
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-19 10:20:21
 * @FilePath: \EOS\interface\include\window\channelbox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_CHANNELBOX_H
#define EOSI_CHANNELBOX_H

#include "window.h"

class INTERFACE_IMEXPORT ChannelBox: public GroupBox
{
    Q_OBJECT
public:
    explicit ChannelBox(QWidget*parent= nullptr);
    ChannelInfo channelInfo() const;
private:
    QRadioButton * brbox;
    QRadioButton * phbox;
    QRadioButton * gfpbox;
    QRadioButton * rfpbox;
    QRadioButton * dapibox;
signals:
    void channelChanged(int option);

};
#endif //EOSI_CHANNELBOX_H
