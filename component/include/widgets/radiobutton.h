/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-20 09:47:31
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-20 09:49:42
 * @FilePath: \EOS\component\include\widgets\radiobutton.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_RADIOBUTTON_H
#define EOS_RADIOBUTTON_H

#include "widgets.h"

class COMPONENT_IMEXPORT RadioButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit RadioButton(QWidget*parent= nullptr);
    explicit RadioButton(const QString&text,QWidget*parent= nullptr);
};

#endif //EOS_RADIOBUTTON_H
