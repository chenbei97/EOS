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
    QBoolVector selectStates() const;
private:
    CheckBox * phbox;
    CheckBox * gfpbox;
    CheckBox * rfpbox;
    CheckBox * dapibox;
signals:
    void phSelected(bool isSelect);
    void gfpSelected(bool isSelect);
    void rfpSelected(bool isSelect);
    void dapiSelected(bool isSelect);

};
#endif //EOSI_CHANNELBOX_H
