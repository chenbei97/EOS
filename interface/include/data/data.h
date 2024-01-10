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
    QStackedWidget * topCanvas;
    SinglePictureCanvas * pictureCanvas;

    QStackedWidget * bottomCanvas;
    GridPictureCanvas * gridRunningCanvas;
    GridPictureCanvas * gridCompeleCanvas;

    Splitter * leftSplitter;
    GroupBox * rightBox;
    DataTable * dataTable;
    DataPattern * dataPattern;
    PatternTable * patternTable;
    TimeTable * timeTable; // 2个水平表
    DataImage * dataImage; // 图后期处理
private slots:
    void toggleCanvas(int idx);
    void toggleExperiment(int row, bool isRunning);
    void parseResult(const QString & f,const QVariant & d);
};

#endif //EOS_DATA_H
