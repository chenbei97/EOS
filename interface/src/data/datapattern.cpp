/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-10 08:39:41
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 08:51:59
 * @FilePath: \EOS\interface\src\data\datapattern.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#include "datapattern.h"

DataPattern::DataPattern(int rows, int cols, QWidget *parent) : Pattern(rows,cols,parent)
{
    mMouseEvent = true;
    setContextMenuPolicy(Qt::ActionsContextMenu);
}