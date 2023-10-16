/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:37:52
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:09:19
 * @FilePath: \EOS\component\include\widgets\spinbox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_SPINBOX_H
#define EOSI_SPINBOX_H

#include "widgets.h"

class COMPONENT_IMEXPORT SpinBox : public QDoubleSpinBox
{
Q_OBJECT
public:
    explicit SpinBox(QWidget*parent = nullptr);
    void wheelEvent(QWheelEvent*e) ;
};


#endif //EOSI_SPINBOX_H
