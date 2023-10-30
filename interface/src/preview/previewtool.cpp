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
    objectivebox = new ObjectiveBox;
    channelbox = new ChannelBox;
    camerabox = new CameraBox;
    focusbox = new FocusBox;

    filenameedit = new LineEdit("please input filename");
    exportToFile = new CheckBox(tr("导出到文件?"));
    saveallbtn = new PushButton(tr("保存所有设置"));
    filenameedit->hide();

    auto blay = new QHBoxLayout;
    blay->addStretch();
    blay->addWidget(filenameedit);
    blay->addWidget(exportToFile);
    blay->addWidget(saveallbtn);
    auto box = new GroupBox(tr("保存"));
    box->setLayout(blay);

    lay = new QVBoxLayout;
    lay->addWidget(wellbox);
    lay->addWidget(objectivebox);
    lay->addWidget(channelbox);
    lay->addWidget(focusbox);
    lay->addWidget(camerabox);


    auto b0 = new GroupBox("镜头");
    auto b1 = new GroupBox("Z_Stack");
    auto b2 = new GroupBox("实验配置");//时间+通道复选
    auto b3 = new GroupBox("启动实验"); // 按钮+实验信息预览+预计
    b0->setMinimumHeight(80);
    b1->setMinimumHeight(80);
    b2->setMinimumHeight(80);
    b3->setMinimumHeight(80);
    lay->addWidget(b1);
    lay->addWidget(box);
    lay->addWidget(b0);

    lay->addWidget(b2);
    lay->addWidget(b3);

    setLayout(lay);

    connect(exportToFile,&CheckBox::checkedChanged,this,[=](bool enable){filenameedit->setVisible(enable);});

    connect(wellbox,&WellBox::wellbrandChanged,this,&PreviewTool::wellbrandChanged);
    connect(wellbox,&WellBox::manufacturerChanged,this,&PreviewTool::manufacturerChanged);
    connect(objectivebox,&ObjectiveBox::objectiveChanged,this,&PreviewTool::objectiveChanged);
    connect(wellbox,&WellBox::wellbrandChanged,this,&PreviewTool::infoChanged);
    connect(wellbox,&WellBox::manufacturerChanged,this,&PreviewTool::infoChanged);
    connect(objectivebox,&ObjectiveBox::objectiveChanged,this,&PreviewTool::infoChanged);


    connect(camerabox,&CameraBox::exposureChanged,this,&PreviewTool::exposureChanged);
    connect(camerabox,&CameraBox::gainChanged,this,&PreviewTool::gainChanged);
    connect(camerabox,&CameraBox::brightChanged,this,&PreviewTool::brightChanged);
    // 不需要滑动条移动就会触发信息改变,只能是保存相机信息那个动作才可以触发
    connect(camerabox,&CameraBox::infoChanged,this,&PreviewTool::infoChanged);
    //connect(camerabox,&CameraBox::exposureChanged,this,&PreviewTool::infoChanged);
    //connect(camerabox,&CameraBox::gainChanged,this,&PreviewTool::infoChanged);
    //connect(camerabox,&CameraBox::brightChanged,this,&PreviewTool::infoChanged);


    connect(channelbox,&ChannelBox::channelChanged,this,&PreviewTool::channelChanged);
    connect(channelbox,&ChannelBox::channelChanged,camerabox,&CameraBox::setChannel);
    connect(channelbox,&ChannelBox::channelChanged,this,&PreviewTool::infoChanged);

    //LOG<<"wellinfo = "<<wellbox->wellInfo();
    //LOG<<"camerainfo = "<<camerabox->cameraInfo();
}

PreviewToolInfo PreviewTool::toolInfo() const
{
    PreviewToolInfo info;

    auto wellinfo = wellbox->wellInfo();
    foreach(auto key,wellinfo.keys())
        info[key] = wellinfo[key];

    auto objectiveinfo = objectivebox->objectiveInfo();
    info[ObjectiveField] = objectiveinfo[ObjectiveField];

    auto channelinfo = channelbox->channelInfo();
    info[ChannelField] = channelinfo[ChannelField];

    auto camerainfo = camerabox->cameraInfo();

    QStringList channels;
    if (!camerainfo.isEmpty()) { // 有勾选的通道
        foreach(auto channel, camerainfo.keys()) {
            CameraInfo val = camerainfo[channel];// 每个通道的信息

            QVariantMap m;//把每个通道的信息转成QVarintMap
            m[ChannelField] = channel;
            m[ExposureField] = val[ExposureField];
            m[GainField] = val[GainField];
            m[BrightField] = val[BrightField];
            info[channel] = m; // 这个channel字段存储这个CameraInfo
            channels << channel;
        }
    }

    info[ChannelField] = channels;//增加一个key=channel,和objective,well等是平级的,方便组装时判断
    //LOG<<"tool info = "<<info;
    return info;
}