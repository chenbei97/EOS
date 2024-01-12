/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 15:14:24
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:22:16
 * @FilePath: \EOS\component\include\widgets\tableview.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_TABLEVIEW_H
#define EOSI_TABLEVIEW_H

#include "widgets.h"

class COMPONENT_IMEXPORT TableView : public QTableView
{
Q_OBJECT
public:
    explicit TableView(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent*e) override;
    void updateGeometries() override;
    void updateCellSize(int w,int h);
    void setModel(QAbstractItemModel *model) override;
private:
    void initGeneralConfig();
private slots:
    void onClicked(const QModelIndex &index);
    void onDoubleClicked(const QModelIndex &index);
    void onPressed(const QModelIndex &index);
signals:
    void currentRowClicked(int row);
    void mouseRightButtonPressed();
    void modelSettled(QAbstractItemModel * model);
    void modelChanged(QAbstractItemModel * model);
};

#endif //EOSI_TABLEVIEW_H
