/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:40:27
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:09:22
 * @FilePath: \EOS\component\include\widgets\splitter.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_SPLITTER_H
#define EOSI_SPLITTER_H

#include "widgets.h"

class COMPONENT_IMEXPORT Splitter : public QSplitter
{
    Q_OBJECT
public:
    explicit Splitter(Qt::Orientation orientation, QWidget *parent = nullptr);
    explicit Splitter(QWidget *parent = nullptr);
};

#endif //EOSI_SPLITTER_H
