/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:53:52
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:54:32
 * @FilePath: \EOS\interface\src\window\previewtool.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "previewtool.h"

PreviewTool::PreviewTool(QWidget *parent) : QWidget(parent)
{
    wellbox = new WellBox;
    camerabox = new CameraBox;

    lay = new QVBoxLayout;
    lay->addWidget(wellbox);
    lay->addWidget(camerabox);
    setLayout(lay);

    connect(wellbox,&WellBox::wellbrandChanged,this,&PreviewTool::wellbrandChanged);
    connect(wellbox,&WellBox::wellsizeChanged,this,&PreviewTool::wellsizeChanged);
    connect(wellbox,&WellBox::welldishChanged,this,&PreviewTool::welldishChanged);
    connect(wellbox,&WellBox::wellflaskChanged,this,&PreviewTool::wellflaskChanged);
    connect(wellbox,&WellBox::wellslideChanged,this,&PreviewTool::wellslideChanged);

}