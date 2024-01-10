/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-10 09:51:59
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 17:26:33
 * @FilePath: \EOS\interface\src\data\timetable.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */
#include "timetable.h"

TimeTable::TimeTable(QWidget*parent): GroupBox(parent)
{
    view = new TableView;
    model = new DataTableModel;
    view->setModel(model);
    auto lay = new QHBoxLayout;
    lay->addWidget(view);
    lay->setMargin(0);
    lay->setSpacing(0);
    setLayout(lay);

    initTable();
}

void TimeTable::initTable()
{
    tableHeader<<"TimeStamp"<<"Image";
    model->setRowCount(5);
    model->setColumnCount(2);
    model->setHorizontalHeaderLabels(tableHeader);
}

void TimeTable::initFilterItem()
{
    const int row = 1; // 第2行

}