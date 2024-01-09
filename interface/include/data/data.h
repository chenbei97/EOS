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

#include "window.h"

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
    QStackedWidget * stackCanvas;
    SinglePictureCanvas * pictureCanvas;
    GridPictureCanvas * gridPictureCanvas;
    Splitter * leftSplitter;
    GroupBox * rightBox;
private slots:
    void toggleCanvas(int idx);
};

#endif //EOS_DATA_H
