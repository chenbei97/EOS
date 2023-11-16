/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 17:04:12
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 17:07:41
 * @FilePath: \EOS\component\include\widgets\textedit.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_TEXTEDIT_H
#define EOSI_TEXTEDIT_H

#include "widgets.h"

class COMPONENT_IMEXPORT TextEdit: public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEdit(QWidget*parent= nullptr);
    explicit TextEdit(const QString& text,QWidget*parent= nullptr);
    QString text() const;
    void setIndent(int moveLength,const QString& indent="    ");
};
#endif //EOSI_TEXTEDIT_H
