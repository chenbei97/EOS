/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-16 13:42:42
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 13:44:15
 * @FilePath: \EOS\component\include\datastructure\listwidgetitem.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_LISTWIDGETITEM_H
#define EOS_MAIN_CORE_PY_LISTWIDGETITEM_H

#include "datastructure.h"

class COMPONENT_IMEXPORT ListWidgetItem : public QListWidgetItem
{ // QListWidgetItem不是QObject的子类，不能使用Q_OBJECT宏
public:
    explicit ListWidgetItem(QListWidget * parent = nullptr);
    explicit ListWidgetItem(const QString&text,QListWidget * parent = nullptr);
};

#endif //EOS_MAIN_CORE_PY_LISTWIDGETITEM_H
