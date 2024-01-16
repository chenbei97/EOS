/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-15 08:45:56
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-15 09:02:57
 * @FilePath: \EOS\component\include\widgets\stackwidget.h
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_STACKWIDGET_H
#define EOS_STACKWIDGET_H

#include "widgets.h"

class COMPONENT_IMEXPORT StackWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit StackWidget(QWidget*parent= nullptr);
};

#endif //EOS_STACKWIDGET_H
