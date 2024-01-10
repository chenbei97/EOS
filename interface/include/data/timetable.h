/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-10 09:51:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 17:27:15
 * @FilePath: \EOS\interface\include\data\timetable.h
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_TIMETABLE_H
#define EOS_TIMETABLE_H

#include "window.h"
#include "datatablemodel.h"

class INTERFACE_IMEXPORT TimeTable: public GroupBox
{
    Q_OBJECT
public:
    explicit TimeTable(QWidget*parent=nullptr);
private:
    TableView * view;
    DataTableModel * model;
    QStringList tableHeader;
private:
    void initTable();
    void initFilterItem();
};

#endif //EOS_TIMETABLE_H
