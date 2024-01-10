/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-09 17:26:46
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 08:35:28
 * @FilePath: \EOS\interface\src\data\datatable.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#include "datatable.h"

void DataTable::initTableFromRunningExperiment()
{ // 已经完成的实验信息都在数据库里,这里去调数据库初始化
    model->appendRowTexts(QStringList()<<"2024-2-1"<<CompleteFieldUpper);
    model->appendRowTexts(QStringList()<<"2024-2-2"<<CompleteFieldUpper);
    model->appendRowTexts(QStringList()<<"2024-2-3"<<CompleteFieldUpper);
    model->appendRowTexts(QStringList()<<"2024-2-4"<<RunningFieldUpper);
}

void DataTable::initTableFromDataBase()
{

}

void DataTable::toggleRow(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if (current.isValid()) {
        bool isRunning = isRunningState(current.row());
        LOG<<current.row()<<isRunning;
        emit currentRowChanged(current.row(),isRunning);
    }
}

bool DataTable::isRunningState(int row) const
{
    auto s = model->data(row,DataExperimentTableStatusColumn).toString();
    return s == RunningFieldUpper;
}

void DataTable::initConnections()
{
    connect(selection,&QItemSelectionModel::currentRowChanged,this,&DataTable::toggleRow);
}

void DataTable::initAttributes()
{
    model->setRowCount(0);
    model->setColumnCount(DataExperimentTableHeaders.count());
    model->setHorizontalHeaderLabels(DataExperimentTableHeaders);
    view->setModel(model);
    view->setSelectionModel(selection);
    view->setSelectionBehavior(TableView::SelectRows);
}

DataTable::DataTable(QWidget*parent): GroupBox(parent)
{
    view = new TableView;
    model = new StandardItemModel;
    selection = new QItemSelectionModel(model);
    auto lay = new QHBoxLayout;
    lay->addWidget(view);
    lay->setMargin(0);
    lay->setSpacing(0);
    setLayout(lay);
    initAttributes();
    initTableFromDataBase();
    initTableFromRunningExperiment();
    initConnections();
}



