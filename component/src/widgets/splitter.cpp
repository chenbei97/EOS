/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:40:40
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:10:27
 * @FilePath: \EOS\component\src\widgets\splitter.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "splitter.h"

Splitter::Splitter(Qt::Orientation orientation, QWidget *parent) :QSplitter(parent)
{
    setOrientation(orientation);
    setHandleWidth(SplitterHandleWidth);
    setOpaqueResize(false);
    setChildrenCollapsible(true);
    setStyleSheet(SplitterStyle);
}


Splitter::Splitter(QWidget *parent) :QSplitter(parent)
{
    setOrientation(Qt::Horizontal);
    setHandleWidth(SplitterHandleWidth);
    setOpaqueResize(false);
    setChildrenCollapsible(true);
    setStyleSheet(SplitterStyle);
}
