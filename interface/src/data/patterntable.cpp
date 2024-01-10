/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-10 09:13:11
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 09:16:56
 * @FilePath: \EOS\interface\src\data\patterntable.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#include "patterntable.h"

PatternTable::PatternTable(QWidget*parent): GroupBox(parent)
{
    view = new TableView;
    model = new DataTableModel;
    comboDelegate = new ComboDelegate;
    initAttributes();

    auto lay = new QHBoxLayout;
    lay->addWidget(view);
    lay->setMargin(0);
    lay->setSpacing(0);
    setLayout(lay);

    initTable();
}

void PatternTable::initAttributes()
{
    tableHeader<<"Coordinate"<<"Group"<<"info"<<"Channel";
    comboDelegate->addItems(ChannelFields);
    model->setColumnCount(tableHeader.count());
    model->setHeaderLabels(tableHeader,Qt::Horizontal);
    model->setEditableColumns(QList<int>()<<model->columnCount()-1); // 最后1列可编辑
    view->setModel(model);
    view->setItemDelegateForColumn(tableHeader.count()-1,comboDelegate);
    view->setEditTriggers(TableView::DoubleClicked); // 设置禁止的话双击无法触发可编辑
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // 必须设置否则列宽不能自适应
    view->setSelectionBehavior(QAbstractItemView::SelectRows); // 按行选择
    view->horizontalHeader()->setStretchLastSection(true);
}

void PatternTable::initTable()
{
    model->appendRowData(QVariantVector()<<"(0.1,0.2)"<<"Positive Control"<<"青霉素 30uM"<<"BR");
    model->appendRowData(QVariantVector()<<"(0.2,0.3)"<<"Positive Control"<<"青霉素 30uM"<<"RFP");
    model->appendRowData(QVariantVector()<<"(0.3,0.4)"<<"Negative Control"<<"阿司匹林 30uM"<<"DAPI");
    model->appendRowTexts(QStringList()<<"(0.4,0.5)"<<"Negative Control"<<"阿司匹林 30uM"<<"PH");

}

void PatternTable::initFilterItem()
{
    const int row = 1; // 第2行

}

void PatternTable::resizeEvent(QResizeEvent *event)
{
    view->resizeColumnsToContents();
    view->resizeRowsToContents();
    event->accept();
}