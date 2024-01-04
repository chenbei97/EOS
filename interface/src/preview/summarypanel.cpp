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
    auto estimatedSpace = m[EstimatedSpaceField].toString();
    auto wellinfo = m[WellBoxTitle].value<WellInfo>();
    auto objectiveinfo = m[ObjectiveBoxTitle].value<ObjectiveInfo>();
    auto wellgroupinfo = m[WellPatternField].value<WellPatternInfo>();
    auto channelinfo = m[ChannelBoxTitle].value<ChannelInfo>();
    auto camerafocusinfo = m[CameraFocusBoxTitle].value<CameraFocusInfo>();
    auto timeinfo = m[TimeBoxTitle].value<TimeInfo>();
    auto focusinfo = m[FocusBoxTitle].value<FocusInfo>();
    auto zstackinfo = m[ZStackBoxField].value<ZStackInfo>();

    edit->clear();
    QString indent = "      ";
    QString indent2x = indent+indent;
    int moveLength = 100;
    {

        edit->append(tr("<strong><font color = #20A848>[1]Basic Info:</font></strong>"));
        // 1.wellbox
        {
            auto manufacturer = ManufacturerFields[wellinfo[FieldLoadExperEvent.manufacturer].toInt()];
            QString brand;
            switch (wellinfo[FieldLoadExperEvent.manufacturer].toInt()) {
                case 0:
                    brand = Brand1Fields[wellinfo[FieldLoadExperEvent.wellbrand].toInt()];
                    break;
                case 1:
                    brand = Brand2Fields[wellinfo[FieldLoadExperEvent.wellbrand].toInt()];
                    break;
                case 2:
                    brand = Brand3Fields[wellinfo[FieldLoadExperEvent.wellbrand].toInt()];
                    break;
                case 3:
                    brand = Brand4Fields[wellinfo[FieldLoadExperEvent.wellbrand].toInt()];
                    break;
            }
            auto wellsize = WellsizeFields[wellinfo[FieldLoadExperEvent.wellsize].toInt()];
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
            auto cameraloc = ObjectiveLocationFields[objectiveinfo[FieldLoadExperEvent.objective_location].toInt()];
            auto objective = objectiveinfo[FieldLoadExperEvent.objective];//就是传递原字符串不需要改
            edit->append("");
            edit->append(tr("<strong><font color = #00A2E8>(2)Objective Info:</font></strong>"));
            //edit->setIndent(moveLength,indent);
            edit->append(tr("<strong><font color = #00A2E8>1.location: %1</font></strong>").arg(cameraloc));
            edit->append(tr("<strong><font color = #00A2E8>2.objective: %1</font></strong>").arg(objective));
        }

        // 3. viewmodebox 不展示

        // 4. camerafocusbox
        {
            auto save_channels = camerafocusinfo[FieldLoadExperEvent.save_channels].toStringList(); // 设置过相机参数的所有通道
            edit->append("");
            if (!save_channels.isEmpty()) {
                edit->append(tr("<strong><font color = #00A2E8>(3)Channel Info:</font></strong>"));
                int count = 0;
                for(auto currentChannel: save_channels) { // 保存过设置的所有通道
                    count++;
                    auto channelInfo = camerafocusinfo[currentChannel].value<QVariantMap>(); // 存储某个通道的是QVariantMap
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

        // 5. camerafocusbox+focusbox
        {
            auto focus = camerafocusinfo[FieldLoadExperEvent.focus].toString();
            auto step = camerafocusinfo[FieldLoadExperEvent.focus_step].toString();
            edit->append("");
            edit->append(tr("<strong><font color = #00A2E8>(4)Focus Info:</font></strong>"));
            edit->append(tr("<strong><font color = #00A2E8>1.focus: %1</font></strong>").arg(focus));
            edit->append(tr("<strong><font color = #00A2E8>2.step: %1</font></strong>").arg(step));
        }

        // 6. timebox
        {
            auto is_schedule = timeinfo[FieldLoadExperEvent.is_schedule].toInt();
            auto duration = timeinfo[FieldLoadExperEvent.duration_time].toLongLong()*1.0;
            auto total = timeinfo[FieldLoadExperEvent.total_time].toLongLong()*1.0;
            auto start_time = timeinfo[FieldLoadExperEvent.start_time];
            auto channels = timeinfo[FieldLoadExperEvent.channel].split(",",QString::SkipEmptyParts);
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
            auto save_channels = camerafocusinfo[FieldLoadExperEvent.save_channels].toStringList();
            if (!channel.isEmpty()) {
                if (save_channels.isEmpty()) {
                    edit->append(tr("<strong><font color = #FF1E27>Warning: the channel you have checked [%1] none of the camera parameters have been saved, "
                                    "and will be executed with default parameters!</font></strong>").arg(channel));
                } else {
                    for(int idx = 0; idx < channels.count(); ++idx) {
                        if(channels[idx].toUInt()) {
                            auto c =  ChannelFields[idx];
                            if (!save_channels.contains(c)) {
                                // 勾选的这个实验通道没在保存过的通道里
                                edit->append(tr("<strong><font color = #FF1E27>Warning: the channel you have checked [%1] none of the camera parameters have been saved,"
                                                "and will be executed with default parameters!</font></strong>").arg(c));
                            }
                        }
                    }
                }
            }
        }

        // 7. zstackbox
        {
            auto zstack = zstackinfo[FieldLoadExperEvent.zstack].toInt();
            auto stitch = zstackinfo[FieldLoadExperEvent.stitch].toInt();
            auto app = AppFields[m[FieldLoadExperEvent.app].toInt()];
            edit->append("");
            edit->append(tr("<strong><font color = #00A2E8>(5)Other Info:</font></strong>"));
            edit->append(tr("<strong><font color = #00A2E8>1.zstack: %1</font></strong>").arg(zstack));
            edit->append(tr("<strong><font color = #00A2E8>2.stitch: %1</font></strong>").arg(stitch));
            edit->append(tr("<strong><font color = #00A2E8>3.app: %1</font></strong>").arg(app));
        }

        // 8.孔组视野信息
        edit->append("");
        if (wellgroupinfo.keys().isEmpty()) {
            edit->append(tr("<strong><font color = #20A848>[2]Hole Info:No grouping</font></strong>"));
        }else {
            edit->append(tr("<strong><font color = #20A848>[2]Hole Info:</font></strong>"));

            int count = 0;
            for(auto group: wellgroupinfo.keys()) {
                count++;
                auto groupinfo = wellgroupinfo[group].value<WellGroupInfo>();
                // 每个组拿第1个孔的信息就可以了,至少1个孔
                auto holeinfo = groupinfo.value(groupinfo.firstKey()).value<WellHoleInfo>();

                auto medicine = holeinfo[HoleMedicineField].toString();
                auto dose = holeinfo[HoleDoseField].toString();
                auto unit = holeinfo[HoleDoseUnitField].toString();
                edit->append(tr("<strong><font color = #00A2E8>%1)%2:</font></strong>").arg(count).arg(group));
                edit->append(tr("<strong><font color = #00A2E8>1.medicine: %1</font></strong>").arg(medicine));
                edit->append(tr("<strong><font color = #00A2E8>2.dose: %1-%2</font></strong>").arg(dose).arg(unit));
            }
        }

        //9.磁盘剩余可用空间和本次实验预计占据的空间
//        edit->append("");
//        edit->append(tr("<strong><font color = #20A848>[2]Disk Info:</font></strong>"));
//        foreach(auto p,systemDrivers()) {
//                edit->append(tr("<strong><font color = #FF1E27>\"%1\": totoal[%2MB] free[%3MB] percent[%4%]</font></strong>").arg(p).arg(
//                        getDiskSpace(p, false)).arg(getDiskSpace(p)).arg(100.0*getDiskSpace(p)/ getDiskSpace(p, false)));
//        }
//        edit->append(tr("<strong><font color = #FF1E27>estimated space: %1MB</font></strong>").arg(estimatedSpace));
    }
}
