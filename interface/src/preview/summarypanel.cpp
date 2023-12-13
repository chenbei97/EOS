/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-07 16:24:45
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-08 11:10:14
 * @FilePath: \EOS\interface\src\preview\summarypanel.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "summarypanel.h"

SummaryPanel::SummaryPanel(const QVariantMap &m, QWidget *parent) : QWidget(parent)
{
    edit = new TextEdit;
    edit->setReadOnly(true);
    edit->setFrameShape(QTextEdit::NoFrame);
    auto lay = new QHBoxLayout;
    lay->addWidget(edit);
    setLayout(lay);

    setData(m);
}

void SummaryPanel::setData(const QVariantMap &m)
{
    auto toolinfo = m[PreviewToolField].value<QVariantMap>();
    auto patterninfo = m[PreviewPatternField].value<QVariantMap>();
#ifdef usetab
    auto experinfo = m[ExperToolField].value<QVariantMap>();
#endif
    auto estimatedSpace = m[EstimatedSpaceField].toString();
    edit->clear();
    QString indent = "      ";
    QString indent2x = indent+indent;
    int moveLength = 100;
    {

        edit->append(tr("<strong><font color = #20A848>[1]Basic Info:</font></strong>"));
        // 1.wellbox
        {
            auto manufacturer = ManufacturerFields[toolinfo[FieldLoadExperEvent.manufacturer].toInt()];
            QString brand;
            switch (toolinfo[FieldLoadExperEvent.manufacturer].toInt()) {
                case 0:
                    brand = Brand1Fields[toolinfo[FieldLoadExperEvent.wellbrand].toInt()];
                    break;
                case 1:
                    brand = Brand2Fields[toolinfo[FieldLoadExperEvent.wellbrand].toInt()];
                    break;
                case 2:
                    brand = Brand3Fields[toolinfo[FieldLoadExperEvent.wellbrand].toInt()];
                    break;
                case 3:
                    brand = Brand4Fields[toolinfo[FieldLoadExperEvent.wellbrand].toInt()];
                    break;
            }
            auto wellsize = WellsizeFields[toolinfo[FieldLoadExperEvent.wellsize].toInt()];
            edit->append("");
            edit->append(tr("<strong><font color = #00A2E8>(1)WellInfo:</font></strong>"));
            //edit->setIndent(8,indent);
            edit->append(tr("<strong><font color = #00A2E8>1.manufacturer: %1</font></strong>").arg(manufacturer));
            //edit->setIndent(21,indent2x);
            edit->append(tr("<strong><font color = #00A2E8>2.brand: %1</font></strong>").arg(brand));
            //edit->setIndent(moveLength,indent2x);
            edit->append(tr("<strong><font color = #00A2E8>3.wellsize: %1</font></strong>").arg(wellsize));
            //edit->setIndent(moveLength,indent2x);
        }

        // 2. objectivebox
        {
            auto cameraloc = ObjectiveLocationFields[toolinfo[FieldLoadExperEvent.objective_location].toInt()];
            auto objective = toolinfo[FieldLoadExperEvent.objective].toString();//就是传递原字符串不需要改
            edit->append("");
            edit->append(tr("<strong><font color = #00A2E8>(2)Objective Info:</font></strong>"));
            //edit->setIndent(moveLength,indent);
            edit->append(tr("<strong><font color = #00A2E8>1.location: %1</font></strong>").arg(cameraloc));
            edit->append(tr("<strong><font color = #00A2E8>2.objective: %1</font></strong>").arg(objective));
        }

        // 3. camerabox
        {
            auto capture_channels = toolinfo[FieldLoadExperEvent.capture_channel].toStringList(); // 设置过相机参数的所有通道
            edit->append("");
            if (!capture_channels.isEmpty()) {
                edit->append(tr("<strong><font color = #00A2E8>(3)Channel Info:</font></strong>"));
                int count = 0;
                foreach(auto currentChannel, capture_channels) {
                    count++;
                    auto channelInfo = toolinfo[currentChannel].value<QVariantMap>();
                    auto exposure = channelInfo[ExposureField].toString();
                    auto gain = channelInfo[GainField].toString();
                    auto bright = channelInfo[BrightField].toString();
                    edit->append(tr("<strong><font color = #00A2E8>%1)%2 channel:</font></strong>").arg(count).arg(currentChannel));
                    edit->append(tr("<strong><font color = #00A2E8>1.exposure: %1 ms</font></strong>").arg(exposure));
                    edit->append(tr("<strong><font color = #00A2E8>2.gain: %1 %</font></strong>").arg(gain));
                    edit->append(tr("<strong><font color = #00A2E8>3.bright: %1</font></strong>").arg(bright));
                }
            } else {
                edit->append(tr("<strong><font color = #00A2E8>(3)Channel Info:none</font></strong>"));
            }
        }

        // 4. focusbox
        {
            auto focus = toolinfo[FieldLoadExperEvent.focus].toString();
            auto step = toolinfo[FieldLoadExperEvent.focus_step].toString();
            edit->append("");
            edit->append(tr("<strong><font color = #00A2E8>(4)Focus Info:</font></strong>"));
            edit->append(tr("<strong><font color = #00A2E8>1.focus: %1</font></strong>").arg(focus));
            edit->append(tr("<strong><font color = #00A2E8>2.step: %1</font></strong>").arg(step));
        }

        // 5. zstackbox
        {
#ifdef usetab
            auto zstack = experinfo[FieldLoadExperEvent.zstack].toBool();
            auto stitch = experinfo[FieldLoadExperEvent.stitch].toBool();
#else
            auto zstack = toolinfo[FieldLoadExperEvent.zstack].toBool();
            auto stitch = toolinfo[FieldLoadExperEvent.stitch].toBool();
#endif
            auto app = AppFields[m[FieldLoadExperEvent.app].toInt()];
            edit->append("");
            edit->append(tr("<strong><font color = #00A2E8>(5)Other Info:</font></strong>"));
            edit->append(tr("<strong><font color = #00A2E8>1.zstack: %1</font></strong>").arg(zstack));
            edit->append(tr("<strong><font color = #00A2E8>2.stitch: %1</font></strong>").arg(stitch));
            edit->append(tr("<strong><font color = #00A2E8>3.app: %1</font></strong>").arg(app));
        }

        // 6. experbox
        {
#ifdef usetab
            auto is_schedule = experinfo[FieldLoadExperEvent.is_schedule].toBool();
            auto duration = experinfo[FieldLoadExperEvent.duration_time].toLongLong()*1.0;
            auto total = experinfo[FieldLoadExperEvent.total_time].toLongLong()*1.0;
            auto start_time = experinfo[FieldLoadExperEvent.start_time].toString();
            auto channels = experinfo[FieldLoadExperEvent.channel].toString().split(",",QString::SkipEmptyParts);
#else
            auto is_schedule = toolinfo[FieldLoadExperEvent.is_schedule].toBool();
            auto duration = toolinfo[FieldLoadExperEvent.duration_time].toLongLong()*1.0;
            auto total = toolinfo[FieldLoadExperEvent.total_time].toLongLong()*1.0;
            auto start_time = toolinfo[FieldLoadExperEvent.start_time].toString();
            auto channels = toolinfo[FieldLoadExperEvent.channel].toString().split(",",QString::SkipEmptyParts);
#endif
            edit->append("");
            edit->append(tr("<strong><font color = #00A2E8>(6)Exper Info:</font></strong>"));
            edit->append(tr("<strong><font color = #00A2E8>1.is_schedule?: %1</font></strong>").arg(is_schedule));
            is_schedule?
            edit->append(tr("<strong><font color = #00A2E8>2.start time: %1</font></strong>").arg(start_time)):
            edit->append(tr("<strong><font color = #00A2E8>2.start time: 立即执行</font></strong>"));
            edit->append(tr("<strong><font color = #00A2E8>3.total time: %1 s - %2 min - %3 hour</font></strong>").arg(total).arg(total/60.0).arg(total/3600.0));
            edit->append(tr("<strong><font color = #00A2E8>4.duration time: %1 s - %2 min - %3 hour</font></strong>").arg(duration).arg(duration/60.0).arg(duration/3600.0));
            QString channel = "";
            for(int idx = 0; idx < channels.count(); ++idx) {
                if(channels[idx].toUInt()) {
                    channel+=ChannelFields[idx]+",";
                }
            }
            channel.chop(1);
            if (!channel.isEmpty())
            edit->append(tr("<strong><font color = #00A2E8>5.channels: %1</font></strong>").arg(channel));
            else edit->append(tr("<strong><font color = #00A2E8>5.channels: no channel selected</font></strong>"));

            // 做些提示,已经勾选的通道是否保存过参数
            auto capture_channels = toolinfo[FieldLoadExperEvent.capture_channel].toStringList();
            if (!channel.isEmpty()) {
                if (capture_channels.isEmpty()) {
                    edit->append(tr("<strong><font color = #FF1E27>Warning: the channel you have checked [%1] none of the camera parameters have been saved, "
                                    "and will be executed with default parameters!</font></strong>").arg(channel));
                } else {
                    for(int idx = 0; idx < channels.count(); ++idx) {
                        if(channels[idx].toUInt()) {
                            auto c =  ChannelFields[idx];
                            if (!capture_channels.contains(c)) {
                                // 勾选的这个实验通道没在保存过的通道里
                                edit->append(tr("<strong><font color = #FF1E27>Warning: the channel you have checked [%1] none of the camera parameters have been saved,"
                                                "and will be executed with default parameters!</font></strong>").arg(c));
                            }
                        }
                    }
                }
            }
        }

        // 7.孔组视野信息
        edit->append("");
        auto patternGroupInfo = patterninfo[GroupField].value<WellPatternInfo>();
        if (patternGroupInfo.keys().isEmpty()) {
            edit->append(tr("<strong><font color = #20A848>[2]Hole Info:No grouping</font></strong>"));
        }else {
            edit->append(tr("<strong><font color = #20A848>[2]Hole Info:</font></strong>"));

            int count = 0;
            for(auto group: patternGroupInfo.keys()) {
                count++;
                auto groupinfo = patternGroupInfo[group].value<QVariantMap>();
                // 每个组拿第1个孔的信息就可以了,至少1个孔
                auto holeinfo = groupinfo.value(groupinfo.firstKey()).value<QVariantMap>();

                auto expertype = holeinfo[HoleExperTypeField].toString();
                auto medicine = holeinfo[HoleMedicineField].toString();
                auto dose = holeinfo[HoleDoseField].toString();
                auto unit = holeinfo[HoleDoseUnitField].toString();
                edit->append(tr("<strong><font color = #00A2E8>%1)%2:</font></strong>").arg(count).arg(group));
                edit->append(tr("<strong><font color = #00A2E8>1.expertype: %1</font></strong>").arg(expertype));
                edit->append(tr("<strong><font color = #00A2E8>2.medicine: %1</font></strong>").arg(medicine));
                edit->append(tr("<strong><font color = #00A2E8>3.dose: %1-%2</font></strong>").arg(dose).arg(unit));
            }
        }

        //8.磁盘剩余可用空间和本次实验预计占据的空间
        edit->append("");
        edit->append(tr("<strong><font color = #20A848>[2]Disk Info:</font></strong>"));
        foreach(auto p,systemDrivers()) {
                edit->append(tr("<strong><font color = #FF1E27>\"%1\": totoal[%2MB] free[%3MB] percent[%4%]</font></strong>").arg(p).arg(
                        getDiskSpace(p, false)).arg(getDiskSpace(p)).arg(100.0*getDiskSpace(p)/ getDiskSpace(p, false)));
        }
        edit->append(tr("<strong><font color = #FF1E27>estimated space: %1MB</font></strong>").arg(estimatedSpace));
    }
}
