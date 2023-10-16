/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 15:01:21
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:09:42
 * @FilePath: \EOS\component\src\datastructure\sqlquerymodel.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "sqlquerymodel.h"

SqlQueryModel::SqlQueryModel(QObject*parent):QSqlQueryModel(parent)
{

}

QVariant SqlQueryModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::TextAlignmentRole)
        return  Qt::AlignCenter;
    return QSqlQueryModel::data(index, role);
}

void SqlQueryModel::sort(int column, Qt::SortOrder order)
{ // QAbstractItemModel 默认什么也没做这里也是
    Q_UNUSED(column);
    Q_UNUSED(order);
}