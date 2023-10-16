/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 15:04:08
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:10:53
 * @FilePath: \EOS\component\include\datastructure\sqltablemodel.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_SQLTABLEMODEL_H
#define EOSI_SQLTABLEMODEL_H

#include "datastructure.h"

class COMPONENT_IMEXPORT SqlTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit SqlTableModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const override;
    QVariant data(int row, int col, int role) const;
    QString data(int row,int col) const;
    void setTable(const QString &tableName) override;
signals:
    void tableSettled(const QString& tableName);
    void tableChanged(const QString& tableName);
};

#endif //EOSI_SQLTABLEMODEL_H
