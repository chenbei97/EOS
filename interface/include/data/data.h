/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-08 14:36:10
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-08 15:16:19
 * @FilePath: \EOS\interface\include\data\data.h
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_DATA_H
#define EOS_DATA_H

#include "datatable.h"
#include "datapattern.h"
#include "dataimage.h"
#include "patterntable.h"
#include "timetable.h"
#include "datagrid.h"

class INTERFACE_IMEXPORT DataWidget: public QWidget
{
    Q_OBJECT
public:
    explicit DataWidget(QWidget*parent= nullptr);
    void resizeEvent(QResizeEvent*event) override;
private:
    void initObjects();
    void initAttributes();
    void initLayout();
    void initConnections();
private:
    GroupBox * leftBox;
    GroupBox * rightBox;
    QStackedWidget * pictureStack;
    QStackedWidget * gridStack;
    QStackedWidget * dataPatternStack;
    QStackedWidget * patternTableStack;
    QStackedWidget * timeTableStack;

    SinglePictureCanvas * pictureCanvas;
    DataTable * dataTable;
    DataImage * dataImage; // 图后期处理

    DataGrid * filterGridCanvas; // 共用型
    QStackedWidget * gridFilterStack; // gridStack + filterGridCanvas
private slots:
    void toggleExperiment(int row, bool isRunning,const PlateImageInfo& info);
    void addStackWidget(const PlateImageInfo& info);
    void parseResult(const QString & f,const QVariant & d);
};

#endif //EOS_DATA_H
