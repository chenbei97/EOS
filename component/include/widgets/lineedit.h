/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:33:36
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:09:07
 * @FilePath: \EOS\component\include\widgets\lineedit.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_LINEEDIT_H
#define EOSI_LINEEDIT_H

#include "widgets.h"

class COMPONENT_IMEXPORT LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEdit(QWidget*parent = nullptr);
    explicit LineEdit(const QString&text,QWidget*parent = nullptr);
    void setIntRange(int min,int max);
    void setRegExp(const QRegExp &rx);
private:
    QIntValidator * validator = nullptr;
    QRegExpValidator *regexValidator = nullptr;
};

#endif //EOSI_LINEEDIT_H
