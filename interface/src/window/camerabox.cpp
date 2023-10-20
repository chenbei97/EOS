/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-19 09:15:26
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-19 09:15:59
 * @FilePath: \EOS\interface\src\window\camerabox.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "camerabox.h"

CameraBox::CameraBox(QWidget*parent): GroupBox(parent)
{
    cameratool1 = new CameraTool;
    cameratool2 = new CameraTool;
    cameratool3 = new CameraTool;
    cameratool4 = new CameraTool;

    cameratool1->setTitle(PHField);
    cameratool2->setTitle(GFPField);
    cameratool3->setTitle(RFPField);
    cameratool4->setTitle(DAPIField);

    setEnabled(0, false);
    setEnabled(1, false);
    setEnabled(2, false);
    setEnabled(3, false);

    auto gridlay = new QGridLayout;
    gridlay->addWidget(cameratool1,0,0);
    gridlay->addWidget(cameratool2,0,1);
    gridlay->addWidget(cameratool3,1,0);
    gridlay->addWidget(cameratool4,1,1);

    channelbox = new ChannelBox;

    auto lay = new QVBoxLayout;
    lay->addWidget(channelbox);
    lay->addLayout(gridlay);

    setLayout(lay);

    connect(cameratool1,&CameraTool::exposureChanged,this,&CameraBox::phExposureChanged);
    connect(cameratool1,&CameraTool::gainChanged,this,&CameraBox::phGainChanged);
    connect(cameratool1,&CameraTool::brightChanged,this,&CameraBox::phBrightChanged);

    connect(cameratool2,&CameraTool::exposureChanged,this,&CameraBox::gfpExposureChanged);
    connect(cameratool2,&CameraTool::gainChanged,this,&CameraBox::gfpGainChanged);
    connect(cameratool2,&CameraTool::brightChanged,this,&CameraBox::gfpBrightChanged);

    connect(cameratool3,&CameraTool::exposureChanged,this,&CameraBox::rfpExposureChanged);
    connect(cameratool3,&CameraTool::gainChanged,this,&CameraBox::rfpGainChanged);
    connect(cameratool3,&CameraTool::brightChanged,this,&CameraBox::rfpBrightChanged);

    connect(cameratool4,&CameraTool::exposureChanged,this,&CameraBox::dapiExposureChanged);
    connect(cameratool4,&CameraTool::gainChanged,this,&CameraBox::dapiGainChanged);
    connect(cameratool4,&CameraTool::brightChanged,this,&CameraBox::dapiBrightChanged);

    connect(channelbox,&ChannelBox::phSelected,this,[=](auto isSelect)
        { setEnabled(0,isSelect);emit channelChanged(channelbox->selectStates());});
    connect(channelbox,&ChannelBox::gfpSelected,this,[=](auto isSelect)
        { setEnabled(1,isSelect);emit channelChanged(channelbox->selectStates());});
    connect(channelbox,&ChannelBox::rfpSelected,this,[=](auto isSelect)
        { setEnabled(2,isSelect);emit channelChanged(channelbox->selectStates());});
    connect(channelbox,&ChannelBox::dapiSelected,this,[=](auto isSelect)
        { setEnabled(3,isSelect);emit channelChanged(channelbox->selectStates());});


}

void CameraBox::setVisible(int index,bool visible)
{
    switch (index) {
        case 0: cameratool1->setVisible(visible);
            break;
        case 1: cameratool2->setVisible(visible);
            break;
        case 2: cameratool3->setVisible(visible);
            break;
        case 3: cameratool4->setVisible(visible);
            break;
        default:
            break;
    }
}

void CameraBox::setEnabled(int index, bool enabled)
{
    switch (index) {
        case 0: cameratool1->setEnabled(enabled);
            break;
        case 1: cameratool2->setEnabled(enabled);
            break;
        case 2: cameratool3->setEnabled(enabled);
            break;
        case 3: cameratool4->setEnabled(enabled);
            break;
        default:
            break;
    }
}

CameraInfo CameraBox::cameraInfo() const
{ // 只有勾选的通道才会打包信息
    auto states = channelbox->selectStates();
    //LOG<<"checked states = "<<states;
    CameraInfo info;
    ChannelInfo m;

    if (states[0]) {
        m = {PHField,states[0],cameratool1->exposure(),
                        cameratool1->gain(),cameratool1->bright()};
        info[PHField] = m;
    }

    if (states[1]) {
        m = {GFPField,states[1],cameratool2->exposure(),
             cameratool2->gain(),cameratool2->bright()};
        info[GFPField] = m;
    }

    if (states[2]) {
        m = {RFPField,states[2],cameratool3->exposure(),
             cameratool3->gain(),cameratool3->bright()};
        info[RFPField] = m;
    }

    if (states[3]) {
        m = {DAPIField,states[3],cameratool4->exposure(),
             cameratool4->gain(),cameratool4->bright()};
        info[DAPIField] = m;
    }

    return info;
}