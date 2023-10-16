/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:07:46
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:08:35
 * @FilePath: \EOS\component\include\widgets\Combobox.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_COMBOBOX_H
#define EOSI_COMBOBOX_H

#include "widgets.h"

class COMPONENT_IMEXPORT ComboBox: public QComboBox
{
    Q_OBJECT
public:
    explicit ComboBox(QWidget*parent = nullptr);
    explicit ComboBox(const QString&text,QWidget*parent = nullptr);
    explicit ComboBox(const QStringList&texts,QWidget*parent = nullptr);
};


#endif //EOSI_COMBOBOX_H
