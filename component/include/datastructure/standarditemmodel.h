/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 15:05:23
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:11:03
 * @FilePath: \EOS\component\include\datastructure\standarditemmodel.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_STANDARDITEMMODEL_H
#define EOSI_STANDARDITEMMODEL_H

#include "datastructure.h"
#include "alias.h"

class COMPONENT_IMEXPORT StandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit StandardItemModel(QObject *parent = nullptr);
    explicit StandardItemModel(int rows, int columns, QObject *parent = nullptr);
    void clearRow(int columnCount,const QStringList& headers,Qt::Orientation orientation = Qt::Horizontal);
    void setModelSize(int rows,int cols);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant data(int row,int col, int role = Qt::DisplayRole) const;
    bool setData(int row,int col,const QVariant &value,int role = Qt::DisplayRole);
    bool setItemData(int row,int col,const QMap<int, QVariant> &roles);
    QMap<int, QVariant> itemData(int row,int col) const;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    void setHeaderLabels(const QStringList& labels, Qt::Orientation orientation = Qt::Horizontal);
    void setRowData(int row, const QVariantVector& rowValues,int role = Qt::DisplayRole);
    void setRowTexts(int row,const QStringList& texts);
    QVariantVector rowData(int row,int role = Qt::DisplayRole) const;
    QStringList rowTexts(int row) const;
    void appendRowData(const QVariantVector& rowValues,int role = Qt::DisplayRole);
    void appendRowTexts(const QStringList& texts);
    void appendRowItems();
};


#endif //EOSI_STANDARDITEMMODEL_H
