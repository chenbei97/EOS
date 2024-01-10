/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-10 13:13:54
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 17:26:19
 * @FilePath: \EOS\interface\src\data\datatablemodel.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#include "datatablemodel.h"

Qt::ItemFlags DataTableModel::flags(const QModelIndex& index) const
{
    for(auto col: editableColumns) {
        if (index.column() == col)
            return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void DataTableModel::setEditableColumns(const QList<int> &cols)
{
    editableColumns = cols;
}

DataTableModel::DataTableModel(QObject *parent) : StandardItemModel(parent)
{

}

DataTableModel::DataTableModel(int rows, int columns, QObject *parent):StandardItemModel(rows,columns,parent)
{

}