/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-10 08:37:21
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 08:52:09
 * @FilePath: \EOS\interface\include\data\datapattern.h
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_DATAPATTERN_H
#define EOS_DATAPATTERN_H

#include "window.h"

class INTERFACE_IMEXPORT DataPattern: public Pattern
{
    Q_OBJECT
public:
    explicit DataPattern(int rows,int cols,QWidget*parent= nullptr);
};

#endif //EOS_DATAPATTERN_H
