/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-10 09:12:39
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 09:16:48
 * @FilePath: \EOS\interface\include\data\patterntable.h
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_PATTERNTABLE_H
#define EOS_PATTERNTABLE_H

#include "window.h"
#include "datatablemodel.h"

static const QStringList PatternTableHeaders = {
        CoordinateFieldUpper,GroupFieldUpper,InfoFieldUpper,ChannelFieldUpper,
};
#define PatternTableViewCoordinateColumn PatternTableHeaders.indexOf(CoordinateFieldUpper)
#define PatternTableChannelColumn PatternTableHeaders.indexOf(ChannelFieldUpper)
class INTERFACE_IMEXPORT PatternTable: public GroupBox
{
    Q_OBJECT
public:
    explicit PatternTable(QWidget*parent=nullptr);
    void refreshTable(const DataPatternHoleInfo& info);
    void resizeEvent(QResizeEvent*event) override;
private:
    TableView * view;
    DataTableModel * model;
    ComboDelegate * comboDelegate;
    QItemSelectionModel * selectionModel;
    DataPatternHoleInfo currentPatternInfo;
private:
    void initAttributes();
    void initTable();
    void initConnections();
    void clickRow(int row);
    void toggleRow(const QModelIndex &current,const QModelIndex &previous);
signals:
    void currentHoleViewChannelInfo(const ImageInfoVector& info);
};

#endif //EOS_PATTERNTABLE_H
