/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-16 13:42:50
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 13:44:28
 * @FilePath: \EOS\component\src\datastructure\listwidgetitem.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "listwidgetitem.h"

ListWidgetItem::ListWidgetItem(QListWidget * parent) :QListWidgetItem(parent,Type)
{
    setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable|Qt::ItemIsSelectable);
}

ListWidgetItem::ListWidgetItem(const QString&text,QListWidget * parent) :QListWidgetItem(text,parent,Type)
{
    setFlags(Qt::ItemIsEnabled|Qt::ItemIsEditable|Qt::ItemIsSelectable);
}
