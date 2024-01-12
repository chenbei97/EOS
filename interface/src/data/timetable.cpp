/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-10 09:51:59
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 17:26:33
 * @FilePath: \EOS\interface\src\data\timetable.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */
#include "timetable.h"

void TimeTable::refreshTable(const ImageInfoVector& info)
{
    currentViewChannelInfo = info;
    model->clearRow(TimeTableHeaders.count(),TimeTableHeaders);
    for(auto image: info) { // 可能视野坐标相同但是时间戳不同
        auto stamp = image.stamp;
        auto path = image.path; // 图像路径
        QStringList rowTexts;
        rowTexts << stamp.toString(DefaultImageDateTimeFormat);
        rowTexts << path;
        rowTexts << "A";
        rowTexts << "青霉素";
        //LOG<<rowTexts;
        model->appendRowTexts(rowTexts);
    }
    view->resizeColumnToContents(TimeTableImageColumn); // resizeAll的不起作用
}

void TimeTable::clickRow(int row)
{ // 控制单张大图的显示
    auto path = model->data(row,TimeTableImageColumn).toString();
    emit imageClicked(path);
}

TimeTable::TimeTable(QWidget*parent): GroupBox(parent)
{
    view = new TableView;
    model = new DataTableModel;
    initTable();

    auto lay = new QHBoxLayout;
    lay->addWidget(view);
    lay->setMargin(0);
    lay->setSpacing(0);
    setLayout(lay);
}

void TimeTable::initTable()
{
    model->setRowCount(0);
    model->setColumnCount(TimeTableHeaders.count());
    model->setHorizontalHeaderLabels(TimeTableHeaders);
    view->setModel(model);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //view->horizontalHeader()->setStretchLastSection(true);
    connect(view,&TableView::currentRowClicked,this,&TimeTable::clickRow);
}