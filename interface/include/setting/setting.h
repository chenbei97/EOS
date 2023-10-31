/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-31 14:11:59
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-31 14:13:53
 * @FilePath: \EOS\interface\include\setting\setting.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_SETTING_H
#define EOSI_SETTING_H

#include "window.h"

class INTERFACE_IMEXPORT Setting : public QWidget
{
    Q_OBJECT
public:
    explicit Setting(QWidget*parent= nullptr);
};

#endif //EOSI_SETTING_H
