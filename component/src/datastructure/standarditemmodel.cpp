/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 15:05:17
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:09:48
 * @FilePath: \EOS\component\src\datastructure\standarditemmodel.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "standarditemmodel.h"

void StandardItemModel::setModelSize(int rows, int cols)
{
    clear();
    setRowCount(rows);
    setColumnCount(cols);
}

StandardItemModel::StandardItemModel(QObject *parent) : QStandardItemModel(parent)
{

}

StandardItemModel::StandardItemModel(int rows, int columns, QObject *parent):QStandardItemModel(rows,columns,parent)
{

}

QVariant StandardItemModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::TextAlignmentRole)
        return  Qt::AlignCenter;

    return QStandardItemModel::data(index, role);
}

Qt::ItemFlags StandardItemModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant StandardItemModel::data(int row, int col, int role) const
{
    return data(index(row,col),role);
}

bool StandardItemModel::setData(int row, int col, const QVariant &value, int role)
{
    return QStandardItemModel::setData(index(row,col),value,role);
}

bool StandardItemModel::setItemData(int row, int col, const QMap<int, QVariant> &roles)
{
    return QStandardItemModel::setItemData(index(row,col),roles);
}

QMap<int, QVariant> StandardItemModel::itemData(int row, int col) const
{
    return QStandardItemModel::itemData(index(row,col));
}

void StandardItemModel::setHeaderLabels(const QStringList &labels, Qt::Orientation orientation)
{
    orientation == Qt::Horizontal?
    setHorizontalHeaderLabels(labels):
    setVerticalHeaderLabels(labels);
}

void StandardItemModel::setRowData(int row, const QVariantVector &rowValues, int role)
{ // 设置数据的时候这行数据的角色都是一样的
    auto valueCount = rowValues.count();
    if (valueCount>columnCount()) { // 值的数量比列数多那么以列数为准,多余的忽略
        for(int col = 0 ; col < columnCount(); ++col) {
            setData(row,col,rowValues.at(col),role);
        }
    } else { // 值的数量少
        for(int col = 0 ; col < valueCount; ++col) {
            setData(row,col,rowValues.at(col),role);
        }
    }
}

QVariantVector StandardItemModel::rowData(int row, int role) const
{
    QVariantVector vec;
    for(int c = 0; c < columnCount(); ++c){
        vec.append(data(row,c,role));
    }
    return vec;
}

void StandardItemModel::setRowTexts(int row, const QStringList &texts)
{ // 快捷方法,设置指定行的文本
    QVariantVector vars;
    for(auto text: texts)
        vars.append(text);
    setRowData(row,vars,Qt::EditRole);
}

QStringList StandardItemModel::rowTexts(int row) const
{
    QStringList texts;
    for(int c = 0; c < columnCount(); ++c){
        texts.append(data(row,c,Qt::DisplayRole).toString());
    }
    return texts;
}

void StandardItemModel::appendRowData(const QVariantVector &rowValues,int role)
{
    auto column_count = columnCount();
    auto value_count = rowValues.count();
    QList<QStandardItem *> rowItems;
    QStandardItem * item = nullptr;

    for(int c = 0; c < column_count; ++c) { // 值的数量多没事
        item = new QStandardItem;
        if (c + 1 <= value_count) // 值的数量少,例如2,那么c只能取0,1才能赋值
        {
            item->setData(rowValues.at(c),role);
        }
        rowItems.append(item); // 值不够的就是空项
    }
    appendRow(rowItems);
}

void StandardItemModel::appendRowTexts(const QStringList &texts)
{
    QVariantVector vars;
    for(auto text: texts)
        vars.append(text);
    appendRowData(vars,Qt::EditRole);
}

void StandardItemModel::appendRowItems()
{ // 添加1行空项
    QList<QStandardItem *> rowItems;
    for(int c = 0; c < columnCount(); ++c) {
        rowItems.append(new QStandardItem);
    }
    appendRow(rowItems);
}

void StandardItemModel::clearRow(int columnCount, const QStringList &headers, Qt::Orientation orientation)
{
    clear();
    setColumnCount(columnCount);
    setHeaderLabels(headers,orientation);
}