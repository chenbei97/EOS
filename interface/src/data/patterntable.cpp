/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-10 09:13:11
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 09:16:56
 * @FilePath: \EOS\interface\src\data\patterntable.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#include "patterntable.h"

void PatternTable::refreshTable(const DataPatternHoleInfo& info)
{
    model->clearRow(PatternTableHeaders.count(),PatternTableHeaders);
    currentPatternInfo = info;
    if (!info.isSelected) return;

    // 指定孔的所有视野坐标(不重复)
    auto view_coords = info.info.viewCoordinates(info.hole.x(),info.hole.y());
    for(auto view_coord: view_coords) { // 只按视野来
        QStringList rowTexts;
        rowTexts << QString("(%1,%2)").arg(view_coord.x()).arg(view_coord.y());

        auto supportChannels = info.info.getViewChannels(info.hole.x(),info.hole.y(),view_coord.x(),view_coord.y());
        if (supportChannels.isEmpty()) // 说明没有一个通道,这是不可能的一般
            rowTexts << BRField;
        else rowTexts << supportChannels[0]; // 尽量显示有照片的那个通道
        model->appendRowTexts(rowTexts);
    }
}

void PatternTable::clickRow(int row)
{
    auto rowText = model->rowTexts(row);
    auto view_coord_text = rowText.at(PatternTableViewCoordinateColumn);
    auto channel_text = rowText.at(PatternTableChannelColumn);
    auto view_coord = convertToPointF(view_coord_text);
    Q_ASSERT(currentPatternInfo.isSelected == true);
    auto info = currentPatternInfo.info.viewChannelImages(currentPatternInfo.hole,view_coord,channel_text);
    LOG<<"current view:"<<view_coord<<"channel:"<<channel_text<<"image count:"<<info.size();
    emit currentHoleViewChannelInfo(info);
}

PatternTable::PatternTable(QWidget*parent): GroupBox(parent)
{
    view = new TableView;
    model = new DataTableModel;
    comboDelegate = new ComboDelegate;
    initAttributes();
    initConnections();

    auto lay = new QHBoxLayout;
    lay->addWidget(view);
    lay->setMargin(0);
    lay->setSpacing(0);
    setLayout(lay);

    initTable();
}

void PatternTable::initAttributes()
{
    comboDelegate->addItems(ChannelFields);
    model->setColumnCount(PatternTableHeaders.count());
    model->setHeaderLabels(PatternTableHeaders,Qt::Horizontal);
    model->setEditableColumns(QList<int>()<<model->columnCount()-1); // 最后1列可编辑
    view->setModel(model);
    view->setItemDelegateForColumn(PatternTableHeaders.count()-1,comboDelegate);
    view->setEditTriggers(TableView::DoubleClicked); // 设置禁止的话双击无法触发可编辑
    view->setSelectionBehavior(QAbstractItemView::SelectRows); // 按行选择
}

void PatternTable::initTable()
{
}

void PatternTable::initConnections()
{
    connect(view,&TableView::currentRowClicked,this,&PatternTable::clickRow);
}

void PatternTable::resizeEvent(QResizeEvent *event)
{
    view->resizeColumnsToContents();
    view->resizeRowsToContents();
    event->accept();
}