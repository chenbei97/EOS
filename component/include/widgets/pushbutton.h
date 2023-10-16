/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:41:53
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:09:14
 * @FilePath: \EOS\component\include\widgets\pushbutton.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_PUSHBUTTON_H
#define EOSI_PUSHBUTTON_H


#include "widgets.h"

class COMPONENT_IMEXPORT PushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PushButton(QWidget*parent=nullptr);
    explicit PushButton(const QString&text,QWidget*parent=nullptr);
};


#endif //EOSI_PUSHBUTTON_H
