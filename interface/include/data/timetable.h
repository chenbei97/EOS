/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-10 09:51:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-10 17:27:15
 * @FilePath: \EOS\interface\include\data\timetable.h
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_TIMETABLE_H
#define EOS_TIMETABLE_H

#include "window.h"
#include "datatablemodel.h"

static const QStringList TimeTableHeaders = {
       TimeStampFieldUpper,ImageFieldUpper,GroupFieldUpper,InfoFieldUpper
};
#define TimeTableStampColumn TimeTableHeaders.indexOf(TimeStampFieldUpper)
#define TimeTableImageColumn TimeTableHeaders.indexOf(ImageFieldUpper)

class INTERFACE_IMEXPORT TimeTable: public GroupBox
{
    Q_OBJECT
public:
    explicit TimeTable(QWidget*parent=nullptr);
    void refreshTable(const ImageInfoVector& info);
private:
    TableView * view;
    DataTableModel * model;
    ImageInfoVector currentViewChannelInfo;
private:
    void initTable();
    void clickRow(int row);
signals:
    void imageClicked(const QString& path);
};

#endif //EOS_TIMETABLE_H
