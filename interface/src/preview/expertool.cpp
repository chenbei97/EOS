/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-14 13:28:55
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-14 14:51:50
 * @FilePath: \EOS\interface\src\preview\expertool.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "expertool.h"

ExperTool::ExperTool(QWidget*parent):QWidget(parent)
{
    timebox = new TimeBox;
    zstackbox = new ZStackBox;
    savebox = new SaveBox;

    lay = new QVBoxLayout;
    lay->addWidget(timebox);
    lay->addWidget(zstackbox);
    lay->addWidget(savebox);
    lay->addStretch();
    setLayout(lay);

    connect(zstackbox,&ZStackBox::zstackChanged,this,&ExperTool::zstackChanged);
    connect(zstackbox,&ZStackBox::stitchChanged,this,&ExperTool::stitchChanged);
    connect(savebox,&SaveBox::exportFilePath,this,&ExperTool::exportFilePath);
    connect(savebox,&SaveBox::loadExper,this,&ExperTool::loadExper);
    connect(savebox,&SaveBox::stopExper,this,&ExperTool::stopExper);
    connect(this,&ExperTool::objectiveChanged,timebox,&TimeBox::disableChannel);
}

ExperToolInfo ExperTool::toolInfo() const
{
    ExperToolInfo info;

    // 1. zstack,stitch
    auto zstackinfo = zstackbox->zstackInfo();
    info[ZStackField] = zstackinfo[ZStackField];
    info[StitchField] = zstackinfo[StitchField];
    // 2. exper
    auto experinfo = timebox->timeInfo();
    foreach(auto key,experinfo.keys())
        info[key] = experinfo[key];

    return info;
}

void ExperTool::importExperConfig(const QVariantMap &m)
{
    // 需要br4x参数来对通道的选择进行禁用
    auto objective = m[ObjectiveDescripField].toString(); // br4x

    { // 1.stackbox
        auto zstack = m[ZStackField].toBool();
        auto stitch = m[StitchField].toBool();
        zstackbox->importExperConfig(zstack,stitch);
    }

    { // 2. timebox
        auto is_schedule = m[IsScheduleField].toBool();
        auto total = m[TotalTimeField].toLongLong();
        auto duration = m[DurationTimeField].toLongLong();
        auto channels = m[ChannelField].toString().split(",",QString::SkipEmptyParts);
        timebox->importExperConfig(is_schedule,total,duration,channels,objective);
    }
}