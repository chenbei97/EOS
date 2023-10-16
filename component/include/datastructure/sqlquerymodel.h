/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 15:01:11
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:10:45
 * @FilePath: \EOS\component\include\datastructure\sqlquerymodel.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_SQLQUERYMODEL_H
#define EOSI_SQLQUERYMODEL_H

#include "datastructure.h"

class COMPONENT_IMEXPORT SqlQueryModel : public QSqlQueryModel
{
Q_OBJECT
public:
    explicit SqlQueryModel(QObject *parent = 0);

    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override; // 查询模型什么也没做
};

#endif //EOSI_SQLQUERYMODEL_H
