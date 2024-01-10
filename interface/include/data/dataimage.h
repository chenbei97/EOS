/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-10 09:01:32
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 09:05:16
 * @FilePath: \EOS\interface\include\data\dataimage.h
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_DATAIMAGE_H
#define EOS_DATAIMAGE_H

#include "window.h"

class INTERFACE_IMEXPORT DataImage: public GroupBox
{
    Q_OBJECT
public:
    explicit DataImage(QWidget*parent=nullptr);
};

#endif //EOS_DATAIMAGE_H
