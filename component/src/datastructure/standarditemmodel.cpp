/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 15:05:17
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:09:48
 * @FilePath: \EOS\component\src\datastructure\standarditemmodel.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "standarditemmodel.h"

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
