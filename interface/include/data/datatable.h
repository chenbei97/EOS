/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-09 17:26:35
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 08:35:35
 * @FilePath: \EOS\interface\include\data\datatable.h
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */


#ifndef EOS_DATATABLE_H
#define EOS_DATATABLE_H

#include "window.h"

static const QStringList DataExperimentTableHeaders = {
        NameFieldUpper,StatusFieldUpper,StartTimeFieldUpper,ObjectiveFieldUpper,
        ChannelsFieldUpper,FocusFieldUpper,StitchFieldUpper,ZStackFieldUpper
};
#define DataExperimentTableStatusColumn DataExperimentTableHeaders.indexOf(StatusFieldUpper)

class INTERFACE_IMEXPORT DataTable: public GroupBox
{
    Q_OBJECT
public:
    explicit DataTable(QWidget*parent=nullptr);
private:
    TableView * view;
    StandardItemModel * model;
    QItemSelectionModel * selection;
private:
    void initAttributes();
    void initConnections();
    void initTableFromDataBase();
    void initTableFromRunningExperiment();
    void toggleRow(const QModelIndex &current, const QModelIndex &previous);
    bool isRunningState(int row) const;
signals:
    void currentRowChanged(int row,bool isRunning);
};
#endif //EOS_DATATABLE_H
