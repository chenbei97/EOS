/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 15:03:18
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:09:45
 * @FilePath: \EOS\component\src\datastructure\SqlTableModel.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "sqltablemodel.h"

SqlTableModel::SqlTableModel(QObject *parent, QSqlDatabase db):QSqlTableModel(parent,db)
{

}

QVariant SqlTableModel::data(const QModelIndex &idx, int role) const
{
    if (role == Qt::BackgroundRole && isDirty(idx))
        return QBrush(QColor(Qt::yellow)); // 修改过数据
    if (role == Qt::TextAlignmentRole)
        return  Qt::AlignCenter;
    return QSqlTableModel::data(idx, role);
}

QVariant SqlTableModel::data(int row,int col, int role) const
{
    auto idx = index(row,col);
    return data(idx,role);
}

QString SqlTableModel::data(int row,int col) const
{
    return data(row,col,Qt::DisplayRole).toString();
}

void SqlTableModel::setTable(const QString &tableName)
{
    auto old = this->tableName();
    QSqlTableModel::setTable(tableName);
    emit tableSettled(tableName);
    if (tableName != old)
        emit tableChanged(tableName);
}
