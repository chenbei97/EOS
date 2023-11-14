/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:27:04
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:08:44
 * @FilePath: \EOS\component\include\widgets\label.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_LABEL_H
#define EOSI_LABEL_H

#include "widgets.h"

class COMPONENT_IMEXPORT Label : public QLabel
{
    Q_OBJECT
public:
    explicit Label(QWidget*parent=nullptr);
    explicit Label(const QString&text,QWidget*parent=nullptr);
    //void paintEvent(QPaintEvent*event) override;

};
#endif //EOSI_LABEL_H
