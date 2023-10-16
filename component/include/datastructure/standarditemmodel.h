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

class COMPONENT_IMEXPORT StandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit StandardItemModel(QObject *parent = nullptr);
    explicit StandardItemModel(int rows, int columns, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
};


#endif //EOSI_STANDARDITEMMODEL_H
