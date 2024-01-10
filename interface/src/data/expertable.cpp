/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-09 17:26:46
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 08:35:28
 * @FilePath: \EOS\interface\src\data\expertable.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#include "expertable.h"

ExperTable::ExperTable(QWidget*parent): GroupBox(parent)
{
    view = new TableView;
    model = new StandardItemModel;
    view->setModel(model);
    auto lay = new QHBoxLayout;
    lay->addWidget(view);
    lay->setMargin(0);
    lay->setSpacing(0);
    setLayout(lay);

    tableHeader<<"Name"<<"Status"<<"Start Time"<<"Objective"<<"Channels"
        <<"Focus"<<"Stitch"<<"ZStack";
    model->setRowCount(5);
    model->setColumnCount(8);
    model->setHorizontalHeaderLabels(tableHeader);
}

