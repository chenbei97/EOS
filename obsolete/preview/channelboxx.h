/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 10:19:49
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 10:09:30
 * @FilePath: \EOS\interface\include\preview\channelbox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOS_CHANNELBOXX_H
#define EOS_CHANNELBOXX_H

#include "window.h"

class INTERFACE_IMEXPORT ChannelBoxx: public GroupBox
{
    Q_OBJECT
public:
    explicit ChannelBoxx(QWidget*parent= nullptr);
    ChannelInfo channelInfo() const;
    void disableChannel(const QString& obj);
private:
    RadioButton * brbox;
    RadioButton * phbox;
    RadioButton * gfpbox;
    RadioButton * rfpbox;
    RadioButton * dapibox;
signals:
    void channelChanged(int option);

};
#endif //EOS_CHANNELBOXX_H
