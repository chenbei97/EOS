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
    lay = new QVBoxLayout;
    auto box1 = initWellBox();
    auto box2 = initCameraBox();

    auto lay1 = new QVBoxLayout;
    lay1->addWidget(box1);
    lay1->addWidget(box2);

    lay->addLayout(lay1);
    setLayout(lay);
}

GroupBox* PreviewTool::initWellBox()
{
    wellbrandCombo = new ComboBox(QStringList()<<"brand1"<<"brand2"<<"brand3");
    wellsizeCombo = new ComboBox(WellsizeFields);
    welldishCombo = new ComboBox(QStringList()<<"dish1"<<"dish2"<<"dish3");
    wellflaskCombo = new ComboBox(QStringList()<<"flash1"<<"flash2"<<"flash3");
    wellslideCombo = new ComboBox(QStringList()<<"slide1"<<"slide2"<<"slide3");
    objectiveCombo = new ComboBox(ObjectiveFields);

    auto box = new GroupBox;
    auto formlay = new QFormLayout;
    formlay->addRow("brand: ",wellbrandCombo);
    formlay->addRow("size: ",wellsizeCombo);
    formlay->addRow("dish: ",welldishCombo);
    formlay->addRow("flask: ",wellflaskCombo);
    formlay->addRow("slide: ",wellslideCombo);
    formlay->addRow("objective: ",objectiveCombo);
    formlay->setVerticalSpacing(20);
    box->setLayout(formlay);

//    auto vlay = new QVBoxLayout;
//    vlay->addWidget(cameraExposureSlider);
//    vlay->addWidget(cameraGainSlider);
//    vlay->addWidget(cameraBrightSlider);
//    box->setLayout(vlay);
    return box;
}

GroupBox* PreviewTool::initCameraBox()
{
    cameraExposureSlider = new Slider("exposure: ","ms",0,15000);
    cameraGainSlider = new Slider("gain: ","%",100,5000);
    cameraBrightSlider = new Slider("bright: ","",0,100);

    auto box = new GroupBox;
    auto vlay = new QVBoxLayout;
    vlay->addWidget(cameraExposureSlider);
    vlay->addWidget(cameraGainSlider);
    vlay->addWidget(cameraBrightSlider);

    box->setLayout(vlay);
    return box;
}