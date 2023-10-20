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
    connect(wellbox,&WellBox::objectiveChanged,this,&PreviewTool::objectiveChanged);
    connect(wellbox,&WellBox::wellbrandChanged,this,&PreviewTool::infoChanged);
    connect(wellbox,&WellBox::wellsizeChanged,this,&PreviewTool::infoChanged);
    connect(wellbox,&WellBox::welldishChanged,this,&PreviewTool::infoChanged);
    connect(wellbox,&WellBox::wellflaskChanged,this,&PreviewTool::infoChanged);
    connect(wellbox,&WellBox::wellslideChanged,this,&PreviewTool::infoChanged);
    connect(wellbox,&WellBox::objectiveChanged,this,&PreviewTool::infoChanged);

    connect(camerabox,&CameraBox::phExposureChanged,this,&PreviewTool::phExposureChanged);
    connect(camerabox,&CameraBox::phGainChanged,this,&PreviewTool::phGainChanged);
    connect(camerabox,&CameraBox::phBrightChanged,this,&PreviewTool::phBrightChanged);
    connect(camerabox,&CameraBox::phExposureChanged,this,&PreviewTool::infoChanged);
    connect(camerabox,&CameraBox::phGainChanged,this,&PreviewTool::infoChanged);
    connect(camerabox,&CameraBox::phBrightChanged,this,&PreviewTool::infoChanged);

    connect(camerabox,&CameraBox::gfpExposureChanged,this,&PreviewTool::gfpExposureChanged);
    connect(camerabox,&CameraBox::gfpGainChanged,this,&PreviewTool::gfpGainChanged);
    connect(camerabox,&CameraBox::gfpBrightChanged,this,&PreviewTool::gfpBrightChanged);
    connect(camerabox,&CameraBox::gfpExposureChanged,this,&PreviewTool::infoChanged);
    connect(camerabox,&CameraBox::gfpGainChanged,this,&PreviewTool::infoChanged);
    connect(camerabox,&CameraBox::gfpBrightChanged,this,&PreviewTool::infoChanged);

    connect(camerabox,&CameraBox::rfpExposureChanged,this,&PreviewTool::rfpExposureChanged);
    connect(camerabox,&CameraBox::rfpGainChanged,this,&PreviewTool::rfpGainChanged);
    connect(camerabox,&CameraBox::rfpBrightChanged,this,&PreviewTool::rfpBrightChanged);
    connect(camerabox,&CameraBox::rfpExposureChanged,this,&PreviewTool::infoChanged);
    connect(camerabox,&CameraBox::rfpGainChanged,this,&PreviewTool::infoChanged);
    connect(camerabox,&CameraBox::rfpBrightChanged,this,&PreviewTool::infoChanged);

    connect(camerabox,&CameraBox::dapiExposureChanged,this,&PreviewTool::dapiExposureChanged);
    connect(camerabox,&CameraBox::dapiGainChanged,this,&PreviewTool::dapiGainChanged);
    connect(camerabox,&CameraBox::dapiBrightChanged,this,&PreviewTool::dapiBrightChanged);
    connect(camerabox,&CameraBox::dapiExposureChanged,this,&PreviewTool::infoChanged);
    connect(camerabox,&CameraBox::dapiGainChanged,this,&PreviewTool::infoChanged);
    connect(camerabox,&CameraBox::dapiBrightChanged,this,&PreviewTool::infoChanged);

    connect(camerabox,&CameraBox::channelChanged,this,&PreviewTool::channelChanged);
    connect(camerabox,&CameraBox::channelChanged,this,&PreviewTool::infoChanged);

    //LOG<<"wellinfo = "<<wellbox->wellInfo();
    //LOG<<"camerainfo = "<<camerabox->cameraInfo();
}

PreviewToolInfo PreviewTool::toolInfo() const
{
    PreviewToolInfo info;

    auto wellinfo = wellbox->wellInfo();
    foreach(auto key,wellinfo.keys()) {
        info[key] = wellinfo[key];
    }

    auto camerainfo = camerabox->cameraInfo(); // 可能4个都没选
    if (!camerainfo.isEmpty()) { // 有勾选的通道
        foreach(auto key, camerainfo.keys()) {
            ChannelInfo val = camerainfo[key];// 每个通道的信息
            QVariantMap m;//把结构体信息转成QVarintMap
            m[ChannelField] = val.channel;
            //m[IsCheckedField] = val.isChecked; // 能传过来的消息一定都是勾选过的,不需要这个额外字段了
            m[ExposureField] = val.expousre;
            m[GainField] = val.gain;
            m[BrightField] = val.bright;

            info[key] = m; // 转成QVarintMap,wellinfo的值是QString,camerainfo的值是QVarintMap
        }
    }

    return info;
}