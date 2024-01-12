/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-12 11:45:02
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-12 11:56:09
 * @FilePath: \EOS\interface\src\data\datagrid.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#include "datagrid.h"
#include <QtConcurrent/QtConcurrent>

void DataGrid::displayImages(const ImageInfoVector &info)
{
    imagemodel->clear();
    completeParams->clear();
    imagemodel->setRowCount(0);
    imagemodel->setColumnCount(cols);
    emit readyRead();
    QtConcurrent::run([=]{
        for(auto image: info) {
            appendImage(image.path);
        }
        emit finished();
    });
}

DataGrid::DataGrid(QWidget *parent) : GridPictureCanvas(parent)
{

}

