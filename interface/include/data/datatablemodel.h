/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-10 13:13:07
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 17:26:49
 * @FilePath: \EOS\interface\include\data\datatablemodel.h
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_DATAMODEL_H
#define EOS_DATAMODEL_H

#include "window.h"

class DataTableModel: public StandardItemModel
{
    Q_OBJECT
public:
    explicit DataTableModel(QObject *parent = nullptr);
    explicit DataTableModel(int rows, int columns, QObject *parent = nullptr);
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    void setEditableColumns(const QList<int> & cols);
private:
    QList<int> editableColumns;
};

#endif //EOS_DATAMODEL_H
