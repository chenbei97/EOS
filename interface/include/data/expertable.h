/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-09 17:26:35
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 08:35:35
 * @FilePath: \EOS\interface\include\data\expertable.h
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */


#ifndef EOS_EXPERTABLE_H
#define EOS_EXPERTABLE_H

#include "window.h"

class INTERFACE_IMEXPORT ExperTable: public GroupBox
{
    Q_OBJECT
public:
    explicit ExperTable(QWidget*parent=nullptr);
private:
    TableView * view;
    StandardItemModel * model;
    QStringList tableHeader;
};
#endif //EOS_EXPERTABLE_H
