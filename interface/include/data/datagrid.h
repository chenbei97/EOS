/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-12 11:43:47
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-12 11:56:15
 * @FilePath: \EOS\interface\include\data\datagrid.h
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_DATAGRID_H
#define EOS_DATAGRID_H

#include "window.h"

class INTERFACE_IMEXPORT DataGrid: public GridPictureCanvas
{
    Q_OBJECT
public:
    explicit DataGrid(QWidget*parent= nullptr);
    void displayImages(const ImageInfoVector & info);
};


#endif //EOS_DATAGRID_H
