/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 16:24:37
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-22 15:14:51
 * @FilePath: \EOS\component\include\socket\socket.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_SOCKET_H
#define EOSI_SOCKET_H

// (1) 包含头文件
#include "qtcpsocket.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qjsonarray.h"
#include "qmutex.h"
#include "qthread.h"
#include "alias.h"
#include "qtimer.h"
#include "fields.h"
#include "constant.h"
#include "utilities.h"

// (1) 静态全局指针
class TcpSocket;
#define SocketInit (TcpSocket::instance())
#define SocketPointer (&TcpSocket::instance())

class AssemblerControl;
#define AssemblerPointer (&AssemblerControl::instance())
#define AssemblerMessage (AssemblerPointer->message())

class ParserControl;
#define ParserPointer (&ParserControl::instance())
#define ParserResult (ParserPointer->result())
#define ParserFrame (ParserPointer->frame())

// (2) 导出定义
#if defined(COMPONENT_LIBRARY)
#  define COMPONENT_IMEXPORT Q_DECL_EXPORT
#else
#  define COMPONENT_IMEXPORT Q_DECL_IMPORT
#endif

// (3) 帧头定义
enum TcpEventType {
    PreviewEvent = 0, // 点击孔,视野的预览事件
    LoadExperEvent = 1, // 启动实验事件
    AskConnectedStateEvent = 2, // 询问套接字连接状态事件
    AskActivateCodeEvent = 3,// 请求激活码事件
    AdjustBrightEvent = 4, // 滑动条调整brght事件
    ToggleChannelEvent = 5, // 切换通道关灯开灯事件
    AdjustLensEvent = 6, // 上下左右调整镜头事件
    MoveMachineEvent = 7,// 移动点击到指定物镜位置事件
    StopExperEvent = 8, // 停止实验事件
    ToggleObjectiveEvent = 9, // 切换物镜动电机事件
    RecordVideoEvent = 10, // 录制视频事件
    ManualFocusEvent = 11, // 手动聚焦事件
    AutoFocusEvent = 12, // 自动聚焦事件
    ChannelMergeEvent = 13, // 通道merge事件
    ExperFinishedEvent = 14, // 实验结束事件
};

typedef struct { // 注册过的帧头命令
    const QString previewEvent = QString::number(PreviewEvent);
    const QString loadExperEvent = QString::number(LoadExperEvent);
    const QString askConnectedStateEvent = QString::number(AskConnectedStateEvent);
    const QString askActivateCodeEvent = QString::number(AskActivateCodeEvent);
    const QString adjustBrightEvent = QString::number(AdjustBrightEvent);
    const QString toggleChannelEvent = QString::number(ToggleChannelEvent);
    const QString adjustLensEvent = QString::number(AdjustLensEvent);
    const QString moveMachineEvent = QString::number(MoveMachineEvent);
    const QString stopExperEvent = QString::number(StopExperEvent);
    const QString toggleObjectiveEvent = QString::number(ToggleObjectiveEvent);
    const QString recordVideoEvent = QString::number(RecordVideoEvent);
    const QString manualFocusEvent = QString::number(ManualFocusEvent);
    const QString autoFocusEvent = QString::number(AutoFocusEvent);
    const QString channelMergeEvent = QString::number(ChannelMergeEvent);
    const QString experFinishedEvent = QString::number(ExperFinishedEvent);
} TcpFrameList;

const QFieldList TcpUsedFrameList { // 用于解析时检测返回的帧是否正确,在这个列表内
        QString::number(PreviewEvent),QString::number(LoadExperEvent),QString::number(AskConnectedStateEvent),
        QString::number(AskActivateCodeEvent),QString::number(AdjustBrightEvent),QString::number(ToggleChannelEvent),
        QString::number(AdjustLensEvent),QString::number(MoveMachineEvent),QString::number(StopExperEvent),
        QString::number(ToggleObjectiveEvent),QString::number(RecordVideoEvent),QString::number(ManualFocusEvent),
        QString::number(AutoFocusEvent),QString::number(ChannelMergeEvent),QString::number(ExperFinishedEvent),
        "test0x0","test0x1","test0x2","test0x3"
};

struct FieldPreviewEvent {
    //const QString viewpoint = ViewCoordinateField;
    //const QString holepoint = HoleCoordinateField;
    const QString view_x = ViewXCoordinateField;
    const QString view_y = ViewYCoordinateField;
    const QString hole_x = HoleXCoordinateField;
    const QString hole_y = HoleYCoordinateField;
    const QString manufacturer = ManufacturerField;
    const QString wellsize = WellsizeField;
    const QString brand = BrandField;
    const QString bright = BrightField;
    const QString objective = ObjectiveField;
    const QString viewsize = ViewSizeField;
    const QString ishole = IsHoleField;
    //const QString current_channel = CurrentChannelField;
    const QString state = StateField; // 解析使用
};

struct FieldLoadExperEvent{
    // 不需要传递给下位机(但是Tcp用过的字段)
    const QString save_channels = SaveChannelsField;//配置过相机参数的所有通道
    const QString state = StateField; // 解析使用
    // 传给下位机json涉及的字段
    // wellbox
    const QString manufacturer = ManufacturerField;
    const QString wellbrand = BrandField;
    const QString wellsize = WellsizeField;
    // objective
    const QString objective_location = ObjectiveLocationField;
    const QString objective = ObjectiveField;
    const QString objective_descrip = ObjectiveDescripField;
    const QString objective_type = ObjectiveTypeField;
    // channel
    const QString ph = PHField;
    const QString gfp = GFPField;
    const QString rfp = RFPField;
    const QString dapi = DAPIField;
    // camera
    const QString exposure = ExposureField;
    const QString gain = GainField;
    const QString bright = BrightField;
    // zstack
    const QString zstack = ZStackField;
    const QString stitch = StitchField;
    // focus
    const QString focus = FocusField;
    const QString focus_step = FocusStepField;
    // exper
    const QString total_time = TotalTimeField;
    const QString duration_time = DurationTimeField;
    const QString start_time = StartTimeField;
    const QString is_schedule = IsScheduleField;
    const QString channel = ChannelField;
    // viewmode
    const QString viewmode = ViewModeField;
    // global
    const QString app = AppSelectField;
    // wellpattern
    const QString group = GroupField;
    const QString x = XField;
    const QString y = YField;
    const QString points = PointsField;
};

struct FieldExportExperEvent{
    // wellbox
    const QString manufacturer = ManufacturerField;
    const QString wellbrand = BrandField;
    const QString wellsize = WellsizeField;
    // objective
    const QString objective_location = ObjectiveLocationField;
    const QString objective = ObjectiveField;
    const QString objective_descrip = ObjectiveDescripField;
    const QString objective_type = ObjectiveTypeField;
    // channel
    const QString ph = PHField;
    const QString gfp = GFPField;
    const QString rfp = RFPField;
    const QString dapi = DAPIField;
    // camera
    const QString exposure = ExposureField;
    const QString gain = GainField;
    const QString bright = BrightField;
    // zstack
    const QString zstack = ZStackField;
    const QString stitch = StitchField;
    // focus
    const QString focus = FocusField;
    const QString focus_step = FocusStepField;
    // exper
    const QString total_time = TotalTimeField;
    const QString duration_time = DurationTimeField;
    const QString start_time = StartTimeField;
    const QString is_schedule = IsScheduleField;
    const QString channel = ChannelField;
    // viewmode
    const QString viewmode = ViewModeField;
    const QString current_group = CurrentGroupField;
    // global
    const QString app = AppSelectField;
    // wellpattern
    const QString group = GroupField;
    const QString holesize = HoleSizeField;
    // other
    const QString save_channels = SaveChannelsField;//配置过相机参数的所有通道
};

struct FieldAskConnectedStateEvent {
    const QString state = StateField; // 程序启动时发送命令询问是否连接上了
};

struct FieldAskActivateCodeEvent {
    const QString activate_code = ActivateCodeField; // 询问激活码
};

struct FieldAdjustBrightEvent {
    const QString current_channel = CurrentChannelField; // 调节某个通道的bright参数
    const QString bright = BrightField;
    const QString state = StateField;
};

struct FieldToggleChannelEvent {
    const QString turnoff_light = TurnOffLight;
    const QString current_channel = CurrentChannelField; // 切换通道
    const QString bright = BrightField;
    const QString state = StateField;
};

struct FieldAdjustLensEvent { // 移动镜头
    const QString direction = DirectionField;
    const QString state = StateField;
};

struct FieldMoveMachineEvent { // 移动电机
    const QString  objective_loc = ObjectiveLocationField;
    const QString state = StateField;
};

struct FieldStopExperEvent { // 停止实验
    const QString stop = StopField;
    const QString state = StateField;
};

struct FieldToggleObjectiveEvent {
    const QString objective_loc = ObjectiveLocationField;
    const QString objective = ObjectiveField;
    const QString isPH = IsPHField;
    const QString state = StateField;
};

struct FieldRecordVideoEvent {
    const QString path = PathField;
    const QString video_format = VideoFormatField;
    const QString video_framerate = VideoFrameRateField;
};

struct FieldManualFocusEvent {
    const QString focus = FocusField;
    const QString state = StateField;
};

struct FieldAutoFocusEvent {
    const QString path = PathField;
    const QString state = StateField;
};

struct FieldChannelMergeEvent {

};

struct FieldExperFinishedEvent {
    const QString exper_finished = ExperFinishedField;
};

typedef struct {
    FieldPreviewEvent fieldPreviewEvent;
    FieldLoadExperEvent fieldLoadExperEvent;
    FieldExportExperEvent fieldExportExperEvent;
    FieldAskConnectedStateEvent fieldAskConnectedStateEvent;
    FieldAskActivateCodeEvent fieldAskActivateCodeEvent;
    FieldAdjustBrightEvent fieldAdjustBrightEvent;
    FieldToggleChannelEvent fieldToggleChannelEvent;
    FieldAdjustLensEvent fieldAdjustLensEvent;
    FieldMoveMachineEvent fieldMoveMachineEvent;
    FieldStopExperEvent fieldStopExperEvent;
    FieldToggleObjectiveEvent fieldToggleObjectiveEvent;
    FieldRecordVideoEvent fieldRecordVideoEvent;
    FieldManualFocusEvent fieldManualFocusEvent;
    FieldAutoFocusEvent fieldAutoFocusEvent;
    FieldChannelMergeEvent fieldChannelMergeEvent;
    FieldExperFinishedEvent fieldExperFinishedEvent;
} TcpFieldList;

static QJsonDocument TcpAssemblerDoc;
static const TcpFrameList TcpFramePool;
static const TcpFieldList TcpFieldPool;

#define FieldPreviewEvent TcpFieldPool.fieldPreviewEvent
#define FieldLoadExperEvent TcpFieldPool.fieldLoadExperEvent
#define FieldExportExperEvent TcpFieldPool.fieldExportExperEvent
#define FieldAskConnectedStateEvent TcpFieldPool.fieldAskConnectedStateEvent
#define FieldAskActivateCodeEvent TcpFieldPool.fieldAskActivateCodeEvent
#define FieldAdjustBrightEvent TcpFieldPool.fieldAdjustBrightEvent
#define FieldToggleChannelEvent TcpFieldPool.fieldToggleChannelEvent
#define FieldAdjustLensEvent TcpFieldPool.fieldAdjustLensEvent
#define FieldMoveMachineEvent TcpFieldPool.fieldMoveMachineEvent
#define FieldStopExperEvent TcpFieldPool.fieldStopExperEvent
#define FieldToggleObjectiveEvent TcpFieldPool.fieldToggleObjectiveEvent
#define FieldRecordVideoEvent TcpFieldPool.fieldRecordVideoEvent
#define FieldManualFocusEvent TcpFieldPool.fieldManualFocusEvent
#define FieldAutoFocusEvent TcpFieldPool.fieldAutoFocusEvent
#define FieldChannelMergeEvent TcpFieldPool.fieldChannelMergeEvent
#define FieldExperFinishedEvent TcpFieldPool.fieldExperFinishedEvent

static QVariant parsePreviewEvent(QCVariantMap m)
{ // 预览事件
    if (!m.keys().contains(FieldPreviewEvent.state)) return false;
    if (!m.keys().contains(FrameField)) return false;
    return m[StateField].toString() == OkField;
}

static QByteArray assemblePreviewEvent(QCVariantMap m)
{// 预览事件
    QJsonObject object;
    object[FrameField] = PreviewEvent;

    object[FieldPreviewEvent.brand] = m[BrandField].toInt();
    object[FieldPreviewEvent.manufacturer] = m[ManufacturerField].toInt();
    object[FieldPreviewEvent.wellsize] = m[WellsizeField].toInt();
    object[FieldPreviewEvent.objective] = m[ObjectiveField].toInt();
#ifdef viewRowColUnEqual
    auto viewsize = m[HoleViewSizeField].value<Dimension2D>();
    object[FieldPreviewEvent.viewsize] = QString("%1,%2").arg(viewsize.rows).arg(viewsize.cols);
#else
    object[FieldPreviewEvent.viewsize] = m[HoleViewSizeField].toInt();
#endif
    //object[FieldPreviewEvent.current_channel] = m[CurrentChannelField].toString();
    auto holepoint = m[HoleCoordinateField].toPoint();
    auto viewpoint = m[ViewCoordinateField].toPointF();
    object[FieldPreviewEvent.hole_x] = holepoint.x();
    object[FieldPreviewEvent.hole_y] = holepoint.y();
    object[FieldPreviewEvent.view_x] = viewpoint.x(); // 浮点数保留6位需要变成字符串形式
    object[FieldPreviewEvent.view_y] = viewpoint.y(); // 又改回来了
    object[FieldPreviewEvent.ishole] = m[IsHoleField].toInt();

    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

static QVariant parseLoadExperEvent(QCVariantMap m)
{// 启动实验
    if (!m.keys().contains(FieldLoadExperEvent.state)) return false;
    if (!m.keys().contains(FrameField)) return false;
    return m[StateField].toString() == OkField;
}

static QByteArray assembleLoadExperEvent(QCVariantMap m)
{// 启动实验
    auto wellinfo = m[WellBoxTitle].value<WellInfo>();
    auto objectiveinfo = m[ObjectiveBoxTitle].value<ObjectiveInfo>();
    auto viewmodeinfo = m[ViewModeBoxTitle].value<ViewModeInfo>();
    auto wellgroupinfo = m[WellPatternField].value<WellPatternInfo>();
    auto channelinfo = m[ChannelBoxTitle].value<ChannelInfo>();
    auto camerafocusinfo = m[CameraFocusBoxTitle].value<CameraFocusInfo>();
    auto timeinfo = m[TimeBoxTitle].value<TimeInfo>();
    auto focusinfo = m[FocusBoxTitle].value<FocusInfo>();
    auto zstackinfo = m[ZStackBoxField].value<ZStackInfo>();
    QJsonObject object;
    object[FrameField] = LoadExperEvent;

    {
        // wellbox
        object[FieldLoadExperEvent.manufacturer] = wellinfo[FieldLoadExperEvent.manufacturer].toInt();
        object[FieldLoadExperEvent.wellbrand] = wellinfo[FieldLoadExperEvent.wellbrand].toInt();
        object[FieldLoadExperEvent.wellsize] = wellinfo[FieldLoadExperEvent.wellsize].toInt();

        // objectivebox
        object[FieldLoadExperEvent.objective_location] = objectiveinfo[FieldLoadExperEvent.objective_location].toInt();
        object[FieldLoadExperEvent.objective_descrip] = objectiveinfo[FieldLoadExperEvent.objective_descrip];//就是传递原字符串不需要改
        object[FieldLoadExperEvent.objective] = getIndexFromFields(objectiveinfo[FieldLoadExperEvent.objective]).toInt();
        object[FieldLoadExperEvent.objective_type] = objectiveinfo[FieldLoadExperEvent.objective_type].toInt();

        // viewmodebox
        object[FieldLoadExperEvent.viewmode] = viewmodeinfo[FieldLoadExperEvent.viewmode].toInt();

        // camerafocusbox
        auto save_channels = camerafocusinfo[FieldLoadExperEvent.save_channels].toStringList(); // 设置过相机参数的所有通道
        QJsonArray channelCameraInfo; // 组列表
        if (!save_channels.isEmpty()) { // 例如channels= {"PH","GFP"}

            QJsonArray channelInfoArr; // 保存所有通道信息的列表,这个是"PH"/"BR"等通道字段的值

            for(auto currentChannel: save_channels) {
                // 通道信息用QVarintMap保存的,有4个key,channel,exposure,gain,bright
                auto channelInfo = camerafocusinfo[currentChannel].value<QVariantMap>();

                auto exposure = channelInfo[ExposureField].toInt();
                auto gain = channelInfo[GainField].toInt();
                auto bright = channelInfo[BrightField].toInt();

                QJsonObject currentChannelInfoObject; // 每个通道的3个信息
                currentChannelInfoObject[ChannelField] = getIndexFromFields(currentChannel).toInt(); // 如PH转为0
                currentChannelInfoObject[ExposureField] = exposure;
                currentChannelInfoObject[GainField] = gain;
                currentChannelInfoObject[BrightField] = bright;

                channelInfoArr.append(currentChannelInfoObject); // 所有通道的信息组成列表,这个列表是camera_channel字段的值
            }

            object[CameraChannelField] = channelInfoArr; // camera_channel字段,存储了所有通道的配置信息
        }

        object[FieldLoadExperEvent.focus] = camerafocusinfo[FieldLoadExperEvent.focus].toDouble();
        object[FieldLoadExperEvent.focus_step] = camerafocusinfo[FieldLoadExperEvent.focus_step].toDouble();

        // timebox
        object[FieldExportExperEvent.total_time] = timeinfo[FieldExportExperEvent.total_time].toInt();
        object[FieldExportExperEvent.duration_time] = timeinfo[FieldExportExperEvent.duration_time].toInt();
        object[FieldExportExperEvent.start_time] = timeinfo[FieldExportExperEvent.start_time];
        object[FieldExportExperEvent.channel] = timeinfo[FieldExportExperEvent.channel];
        object[FieldExportExperEvent.is_schedule] = timeinfo[FieldExportExperEvent.is_schedule].toInt();

        // focusbox

        // zstackbox
        object[FieldExportExperEvent.zstack] = zstackinfo[FieldExportExperEvent.zstack].toInt();
        object[FieldExportExperEvent.stitch] = zstackinfo[FieldExportExperEvent.stitch].toInt();

        // 其它全局信息
        object[FieldLoadExperEvent.app] = m[FieldLoadExperEvent.app].toInt();
    }

    // 组-孔-视野的信息
    QJsonArray arr; // "group"的值是个列表 arr group=[{},{},{}]
    for(auto groupName: wellgroupinfo.keys()) {
        QJsonObject groupObject; // arr有多个groupObject对象,表示每个组的信息,"a组","b组" 每个小{}
        QJsonArray groupValues; // groupObject的值是个列表,也就是"a组"的值是个列表,包含孔信息

        auto groupinfo = wellgroupinfo[groupName].value<WellGroupInfo>();

        for(auto holekey: groupinfo.keys()) {
            auto holeinfo = groupinfo[holekey].value<WellHoleInfo>();

            auto coordinate = holeinfo[HoleCoordinateField].toPoint();
            auto viewpoints = holeinfo[HoleViewPointsField].value<QPointFVector>();//注意用电机坐标不是ui坐标

            QJsonObject holeObject;// "a组"=[{},{},{}],是每个小的{}
            //holeObject[HoleCoordinateField] = QString(PairArgsField).arg(QChar(coordinate.x()+65)).arg(coordinate.y());
            holeObject[FieldLoadExperEvent.x] = coordinate.x();
            holeObject[FieldLoadExperEvent.y] = coordinate.y();

            QJsonArray pointValues; // point字段的值是个列表
            for(int viewcount = 0; viewcount<viewpoints.count(); ++viewcount) {
                QJsonObject viewObject;
                auto viewpoint_x = viewpoints[viewcount].x();
                auto viewpoint_y = viewpoints[viewcount].y();

                //viewObject[HoleCoordinateField] = QString(PairArgsField).arg(viewpoint_x).arg(viewpoint_y);
                viewObject[FieldLoadExperEvent.x] = viewpoint_x;
                viewObject[FieldLoadExperEvent.y] = viewpoint_y;
                pointValues.append(viewObject);
            }

            holeObject[FieldLoadExperEvent.points] = pointValues;
            groupValues.append(holeObject);
        }
        groupObject[groupName] = groupValues;
        arr.append(groupObject);
    }
    object[FieldLoadExperEvent.group] = arr;
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

static QByteArray assembleExportExperEvent(QCVariantMap m)
{// 导出实验配置(不参与tcp通讯),和启动实验相比多了很多东西: 如组颜色,组的一些信息,视野尺寸等,是全部的信息都导出
    auto wellinfo = m[WellBoxTitle].value<WellInfo>();
    auto objectiveinfo = m[ObjectiveBoxTitle].value<ObjectiveInfo>();
    auto viewmodeinfo = m[ViewModeBoxTitle].value<ViewModeInfo>();
    auto wellgroupinfo = m[WellPatternField].value<WellPatternInfo>();
    auto wellpatternsize = m[HoleSizeField].toSize();
    auto channelinfo = m[ChannelBoxTitle].value<ChannelInfo>();
    auto camerafocusinfo = m[CameraFocusBoxTitle].value<CameraFocusInfo>();
    auto timeinfo = m[TimeBoxTitle].value<TimeInfo>();
    auto focusinfo = m[FocusBoxTitle].value<FocusInfo>();
    auto zstackinfo = m[ZStackBoxField].value<ZStackInfo>();

    QJsonObject object;
    {
        // wellbox
        object[FieldExportExperEvent.manufacturer] = wellinfo[FieldExportExperEvent.manufacturer].toInt();
        object[FieldExportExperEvent.wellbrand] = wellinfo[FieldExportExperEvent.wellbrand].toInt();
        object[FieldExportExperEvent.wellsize] = wellinfo[FieldExportExperEvent.wellsize].toInt();

        // objectivebox
        object[FieldExportExperEvent.objective_location] = objectiveinfo[FieldExportExperEvent.objective_location].toInt();
        object[FieldExportExperEvent.objective_descrip] = objectiveinfo[FieldExportExperEvent.objective_descrip];//就是传递原字符串不需要改
        object[FieldExportExperEvent.objective] = getIndexFromFields(objectiveinfo[FieldExportExperEvent.objective]).toInt();
        object[FieldExportExperEvent.objective_type] = objectiveinfo[FieldExportExperEvent.objective_type].toInt();

        // viewmodebox
        object[FieldExportExperEvent.viewmode] = viewmodeinfo[FieldExportExperEvent.viewmode].toInt();
        object[FieldExportExperEvent.current_group] = viewmodeinfo[FieldExportExperEvent.current_group];

        // channelbox(不需要)

        // camerafocusbox
        auto save_channels = camerafocusinfo[FieldExportExperEvent.save_channels].toStringList(); // 设置过相机参数的所有通道
        QJsonArray channelCameraInfo; // 组列表
        if (!save_channels.isEmpty()) { // 例如channels= {"PH","GFP"}

            QJsonArray channelInfoArr; // 保存所有通道信息的列表,这个是"PH"/"BR"等通道字段的值

            for(auto currentChannel: save_channels) {
                // 通道信息用QVarintMap保存的,有4个key,channel,exposure,gain,bright
                auto channelInfo = camerafocusinfo[currentChannel].value<QVariantMap>();

                auto exposure = channelInfo[ExposureField].toInt();
                auto gain = channelInfo[GainField].toInt();
                auto bright = channelInfo[BrightField].toInt();

                QJsonObject currentChannelInfoObject; // 每个通道的4个信息,为了导入确定是哪个通道,需要channel标识符
                currentChannelInfoObject[ChannelField] = getIndexFromFields(currentChannel).toInt(); // 如PH转为0
                currentChannelInfoObject[ExposureField] = exposure;
                currentChannelInfoObject[GainField] = gain;
                currentChannelInfoObject[BrightField] = bright;

                channelInfoArr.append(currentChannelInfoObject); // 所有通道的信息组成列表,这个列表是camera_channel字段的值
            }

            object[CameraChannelField] = channelInfoArr; // camera_channel字段,存储了所有通道的配置信息
        }
        object[FieldExportExperEvent.focus] = camerafocusinfo[FieldExportExperEvent.focus].toDouble();
        object[FieldExportExperEvent.focus_step] = camerafocusinfo[FieldExportExperEvent.focus_step].toDouble();

        // timebox
        object[FieldExportExperEvent.total_time] = timeinfo[FieldExportExperEvent.total_time].toInt();
        object[FieldExportExperEvent.duration_time] = timeinfo[FieldExportExperEvent.duration_time].toInt();
        object[FieldExportExperEvent.start_time] = timeinfo[FieldExportExperEvent.start_time];
        object[FieldExportExperEvent.channel] = timeinfo[FieldExportExperEvent.channel];
        object[FieldExportExperEvent.is_schedule] = timeinfo[FieldExportExperEvent.is_schedule].toInt();

        // focusbox


        // zstackbox
        object[FieldExportExperEvent.zstack] = zstackinfo[FieldExportExperEvent.zstack].toInt();
        object[FieldExportExperEvent.stitch] = zstackinfo[FieldExportExperEvent.stitch].toInt();

        // 其它全局信息
        object[FieldExportExperEvent.app] = m[FieldExportExperEvent.app].toInt();
    }

    // 组-孔-视野的信息
    QJsonArray arr; // "group"的值是个列表 arr group=[{},{},{}]
    for(auto groupName: wellgroupinfo.keys()) {

        QJsonObject groupObject; // arr有多个groupObject对象,表示每个组的信息,"a组","b组" 每个小{}
        QJsonArray groupValues; // groupObject的值是个列表,也就是"a组"的值是个列表,包含孔信息
        auto groupinfo = wellgroupinfo[groupName].value<WellGroupInfo>();

        for(auto holekey: groupinfo.keys()) {
            QJsonObject holeObject;
            QString tmpStr;
            auto holeinfo = groupinfo[holekey].value<WellHoleInfo>();

            holeObject[HoleMedicineField] = holeinfo[HoleMedicineField].toString();
            holeObject[HoleDoseField] = holeinfo[HoleDoseField].toString();
            holeObject[HoleDoseUnitField] = holeinfo[HoleDoseUnitField].toString();

            auto coordinate = holeinfo[HoleCoordinateField].toPoint();
            holeObject[HoleCoordinateField] = QString(PairArgsField).arg(coordinate.x()).arg(coordinate.y());
            holeObject[HoleGroupColorField] = holeinfo[HoleGroupColorField].toString();
            holeObject[HoleGroupNameField] = holeinfo[HoleGroupNameField].toString();
            auto groupholes = holeinfo[HoleGroupCoordinatesField].value<QPointVector>();
            auto allgroups = holeinfo[HoleAllGroupsField].value<QSet<QString>>();
            auto allholes = holeinfo[HoleAllCoordinatesField].value<QPoint2DVector>();
//            for(auto pt: groupholes) { // 组的所有孔坐标也没有意义,这个信息可以被其他信息推断,另外HoleInfo也没有这个字段,所以也不提供写入
//                tmpStr += QString("(%1,%2),").arg(pt.x()).arg(pt.y());
//            }
//            tmpStr.chop(1);
//            holeObject[HoleGroupCoordinatesField] = tmpStr;

//            tmpStr.clear(); // allgroups可以导入时自行重新计算,无需写入
//            for(auto n: allgroups.values()) {
//                tmpStr += QString("%1,").arg(n);
//            }
//            tmpStr.chop(1);
//            holeObject[HoleAllGroupsField] = tmpStr;

//            tmpStr.clear();
//            for(auto pts: allholes) {// allholes可以导入时自行重新计算,无需写入
//                for(auto pt: pts)
//                    tmpStr += QString("(%1,%2),").arg(pt.x()).arg(pt.y());
//                tmpStr.chop(1);
//                tmpStr += "|";
//            }
//            tmpStr.chop(1);
//            holeObject[HoleAllCoordinatesField] = tmpStr;
#ifdef viewRowColUnEqual
            auto viewsize = holeinfo[HoleViewSizeField].value<Dimension2D>();
            holeObject[HoleViewSizeField] = QString(PairArgsField).arg(viewsize.rows).arg(viewsize.cols);
#else
            holeObject[HoleViewSizeField] = holeinfo[HoleViewSizeField].toInt();
#endif
            auto viewrects = holeinfo[HoleViewRectsField].value<QRectFVector>();//
            auto viewuipoints = holeinfo[HoleViewUiPointsField].value<QPointFVector>();
            auto viewpoints = holeinfo[HoleViewPointsField].value<QPointFVector>();
            if (viewuipoints.isEmpty()) //注意:如果是点模式这里是空的,viewrects也是空的
                viewuipoints = viewpoints; // 因为点模式下电机坐标也是离散坐标

            tmpStr.clear();
            for(auto rect: viewrects) {
                tmpStr += QString("(%1,%2,%3,%4),").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg( rect.height());
            }
            tmpStr.chop(1);
            holeObject[HoleViewRectsField] = tmpStr;

            // wellpattern/wellview统一只使用viewrect绘制比较有效率,不提供uipoints的写入了
//            tmpStr.clear();
//            for(auto pt: viewuipoints) {
//                tmpStr += QString("(%1,%2),").arg(pt.x()).arg(pt.y());
//            }
//            tmpStr.chop(1);
//            holeObject[HoleViewUiPointsField] = tmpStr;

            tmpStr.clear();
            for(auto pt: viewpoints) {
                tmpStr += QString("(%1,%2),").arg(pt.x()).arg(pt.y());
            }
            tmpStr.chop(1);
            holeObject[HoleViewPointsField] = tmpStr;

            groupValues.append(holeObject);
        }
        groupObject[groupName] = groupValues;
        arr.append(groupObject);
    }
    object[FieldExportExperEvent.group] = arr;
    object[FieldExportExperEvent.holesize] = QString(PairArgsField).arg(wellpatternsize.width()).arg(wellpatternsize.height());
    TcpAssemblerDoc.setObject(object);
    return TcpAssemblerDoc.toJson();
}

static QVariant parseAskConnectedStateEvent(QCVariantMap m)
{ // 用于询问是否连接的命令,只要有回复不为空就ok,随便发
    if (!m.keys().contains(FieldAskConnectedStateEvent.state)) return false;
    if (!m.keys().contains(FrameField)) return false;
    auto text = m[FieldAskConnectedStateEvent.state].toString();
    return text == OkField;
}

static QByteArray assembleAskConnectedStateEvent(QCVariantMap m)
{ // 用于询问是否连接的命令,只要有回复不为空就ok,随便发
    Q_UNUSED(m);
    QJsonObject object;
    object[FrameField] = AskConnectedStateEvent;
    object[FieldAskConnectedStateEvent.state] = "socket is connected?";
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();

    auto length = convertToHex(json.length()).toUtf8();
    // "@@@0x00000038{\n    \"frame\": 2,\n    \"state\": \"socket is connected?\"\n}\n"
    //LOG<<PrependSeparateField(PrependField(json,length));

    return AppendSeparateField(json);
}

static QVariant parseAskActivateCodeEvent(QCVariantMap m)
{ // 询问设备激活码,回复激活码
    if (!m.keys().contains(FieldAskActivateCodeEvent.activate_code)) return QVariant();
    if (!m.keys().contains(FrameField)) return QVariant();
    auto code = m[FieldAskActivateCodeEvent.activate_code].toString();
    return code;
}

static QByteArray assembleAskActivateCodeEvent(QCVariantMap m)
{ // 询问设备激活码,回复激活码
    Q_UNUSED(m);
    QJsonObject object;
    object[FrameField] = AskActivateCodeEvent;
    object[FieldAskActivateCodeEvent.activate_code] = "ask activate_code";
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

static QByteArray assembleAdjustBrightEvent(QCVariantMap m)
{ // 调节bright参数
    QJsonObject object;
    object[FrameField] = AdjustBrightEvent;
    object[FieldAdjustBrightEvent.bright] = m[BrightField].toDouble();
    object[FieldAdjustBrightEvent.current_channel] = m[CurrentChannelField].toInt();
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

static QVariant parseAdjustBrightEvent(QCVariantMap m)
{ // 调节相机参数
    if (!m.keys().contains(FieldAdjustBrightEvent.state)) return false;
    if (!m.keys().contains(FrameField)) return false;
    return m[StateField].toString() == OkField;
}

static QByteArray assembleToggleChannelEvent(QCVariantMap m)
{ // 切换通道
    QJsonObject object;
    object[FrameField] = ToggleChannelEvent;
    object[FieldToggleChannelEvent.bright] = m[BrightField].toDouble();
    object[FieldToggleChannelEvent.turnoff_light] = m[TurnOffLight].toInt();
    object[FieldToggleChannelEvent.current_channel] = m[CurrentChannelField].toInt();
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

static QVariant parseToggleChannelEvent(QCVariantMap m)
{// 切换通道
    if (!m.keys().contains(FrameField)) return false;
    if (!m.keys().contains(FieldToggleChannelEvent.state)) return false;
    auto ret = m[StateField].toString();
    return ret == OkField;
}

static QByteArray assembleAdjustLensEvent(QCVariantMap m)
{ // 移动镜头
    QJsonObject object;
    object[FrameField] = AdjustLensEvent;
    object[FieldAdjustLensEvent.direction] = m[DirectionField].toInt();
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

static QVariant parseAdjustLensEvent(QCVariantMap m)
{// 移动镜头
    if (!m.keys().contains(FrameField)) return false;
    if (!m.keys().contains(FieldAdjustLensEvent.state)) return false;
    auto ret = m[StateField].toString();
    return ret == OkField;
}

static QByteArray assembleMoveMachineEvent(QCVariantMap m)
{ // 移动电机到指定位置
    QJsonObject object;
    object[FrameField] = MoveMachineEvent;
    object[FieldMoveMachineEvent.objective_loc] = m[ObjectiveLocationField].toInt();
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

static QVariant parseMoveMachineEvent(QCVariantMap m)
{// 移动电机到指定位置
    if (!m.keys().contains(FrameField)) return false;
    if (!m.keys().contains(FieldMoveMachineEvent.state)) return false;
    auto ret = m[StateField].toString();
    return ret == OkField;
}

static QByteArray assembleStopExperEvent(QCVariantMap m)
{ // 停止实验
    QJsonObject object;
    object[FrameField] = StopExperEvent;
    object[FieldStopExperEvent.stop] = m[StopField].toInt();
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

static QVariant parseStopExperEvent(QCVariantMap m)
{// 停止实验
    if (!m.keys().contains(FrameField)) return false;
    if (!m.keys().contains(FieldStopExperEvent.state)) return false;
    auto ret = m[StateField].toString();
    return ret == OkField;
}

static QByteArray assembleToggleObjectiveEvent(QCVariantMap m)
{ // 切物镜动电机事件
    QJsonObject object;
    object[FrameField] = ToggleObjectiveEvent;
    object[FieldToggleObjectiveEvent.objective_loc] = m[ObjectiveLocationField].toInt();
    object[FieldToggleObjectiveEvent.objective] = m[ObjectiveField].toInt();
    object[FieldToggleObjectiveEvent.isPH] = m[IsPHField].toInt();
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

static QVariant parseToggleObjectiveEvent(QCVariantMap m)
{// 切物镜动电机事件
    if (!m.keys().contains(FrameField)) return false;
    if (!m.keys().contains(FieldToggleObjectiveEvent.state)) return false;
    auto ret = m[StateField].toString();
    return ret == OkField;
}

static QByteArray assembleRecordVideoEvent(QCVariantMap m)
{ // 录制视频事件
    QJsonObject object;
    object[FrameField] = RecordVideoEvent;
    object[FieldRecordVideoEvent.path] = m[PathField].toString();
    object[FieldRecordVideoEvent.video_format] = m[VideoFormatField].toInt();
    object[FieldRecordVideoEvent.video_framerate] = m[VideoFrameRateField].toInt();
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

static QVariant parseRecordVideoEvent(QCVariantMap m)
{// 录制视频事件
    if (!m.keys().contains(FrameField)) return false;
    if (!m.keys().contains(FieldRecordVideoEvent.path)) return false;
    auto path = m[FieldRecordVideoEvent.path].toString(); // 会回复合成xxx.avi所在路径
    return path;
}

static QByteArray assembleManualFocusEvent(QCVariantMap m)
{// 手动调焦事件
    QJsonObject object;
    object[FrameField] = ManualFocusEvent;
    object[FieldManualFocusEvent.focus] = m[FocusField].toDouble(); // focus注意传浮点数
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

static QVariant parseManualFocusEvent(QCVariantMap m)
{// 手动调焦事件
    if (!m.keys().contains(FrameField)) return false;
    if (!m.keys().contains(FieldManualFocusEvent.state)) return false;
    auto state = m[FieldManualFocusEvent.state].toString();
    return state == OkField;
}

static QByteArray assembleAutoFocusEvent(QCVariantMap m)
{// 自动调焦事件
    QJsonObject object;
    object[FrameField] = AutoFocusEvent;
    object[PathField] = m[PathField].toString();
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

static QVariant parseAutoFocusEvent(QCVariantMap m)
{// 手动调焦事件
    if (!m.keys().contains(FrameField)) return false;
    if (!m.keys().contains(FieldAutoFocusEvent.state)) return false;
    auto state = m[StateField].toString();
    return state == OkField;
}

static QByteArray assembleChannelMergeEvent(QCVariantMap m)
{// 通道合并事件
    QJsonObject object;
    object[FrameField] = AutoFocusEvent;
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

static QVariant parseChannelMergeEvent(QCVariantMap m)
{// 通道事件
    if (!m.keys().contains(FrameField)) return false;
    return false;
}

static QVariant parseExperFinishedEvent(QCVariantMap m)
{ // 实验结束-服务端主动给客户端发
    if (!m.keys().contains(FrameField)) return false;
    if (!m.keys().contains(FieldExperFinishedEvent.exper_finished)) return false;
    auto isFinished = m[FieldExperFinishedEvent.exper_finished].toInt();
    return isFinished;
}

/*---------以下都是临时测试函数,以后可以注释掉-----------------*/
static QByteArray assemble_test0x0(QCVariantMap m)
{ // test0x0会传来x,y,frame字段
    QJsonObject object;
    object[FrameField] = "test0x0";
    object[XField] = m[XField].toString();
    object[YField] = m[YField].toString();

    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}
static QVariant parse_test0x0(QCVariantMap m)
{// test0x0会返回x,y,frame,path字段
    if (!m.keys().contains(XField)) return QVariant();
    if (!m.keys().contains(YField)) return QVariant();
    if (!m.keys().contains(PathField)) return QVariant();
    if (!m.keys().contains(FrameField)) return QVariant();

    auto path = m[PathField].toString();
    return path;
}
static QByteArray assemble_test0x1(QCVariantMap m)
{ // test0x1会传来equip,frame字段
    QJsonObject object;
    object[FrameField] = "test0x1";
    object[EquipField] = m[EquipField].toString();

    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}
static QVariant parse_test0x1(QCVariantMap m)
{// test0x1会返回equip,frame字段
    if (!m.keys().contains(EquipField)) return QVariant();
    if (!m.keys().contains(FrameField)) return QVariant();

    auto equip = m[EquipField].toString();
    return equip;
}
static QVariant parse_test0x2(QCVariantMap m)
{// test0x2会返回state,frame字段
    if (!m.keys().contains(StateField)) return QVariant();
    if (!m.keys().contains(FrameField)) return QVariant();

    auto state = m[StateField].toString();
    return state == "ok";
}

static QVariant parse_test0x3(QCVariantMap m)
{// test0x3会返回state,frame字段
    if (!m.keys().contains(StateField)) return QVariant();
    if (!m.keys().contains(FrameField)) return QVariant();

    auto state = m[StateField].toString();
    return state == "ok";
}
/*---------以上都是临时测试函数,以后可以注释掉-----------------*/

// 根据帧头选择对应的解析函数
static QMap<QString,TcpParseFuncPointer>  TcpParseFunctions = {
        {TcpFramePool.previewEvent,parsePreviewEvent},
        {TcpFramePool.loadExperEvent, parseLoadExperEvent},
        {TcpFramePool.askConnectedStateEvent, parseAskConnectedStateEvent},
        {TcpFramePool.askActivateCodeEvent, parseAskActivateCodeEvent},
        {TcpFramePool.adjustBrightEvent, parseAdjustBrightEvent},
        {TcpFramePool.toggleChannelEvent, parseToggleChannelEvent},
        {TcpFramePool.adjustLensEvent, parseAdjustLensEvent},
        {TcpFramePool.moveMachineEvent, parseMoveMachineEvent},
        {TcpFramePool.stopExperEvent, parseStopExperEvent},
        {TcpFramePool.toggleObjectiveEvent, parseToggleObjectiveEvent},
        {TcpFramePool.recordVideoEvent, parseRecordVideoEvent},
        {TcpFramePool.manualFocusEvent, parseManualFocusEvent},
        {TcpFramePool.autoFocusEvent, parseAutoFocusEvent},
        {TcpFramePool.channelMergeEvent, parseChannelMergeEvent},
        {TcpFramePool.experFinishedEvent, parseExperFinishedEvent},
        {"test0x0",parse_test0x0},
        {"test0x1",parse_test0x1},
        {"test0x2",parse_test0x2},
        {"test0x3",parse_test0x3},
};

static QMap<QString,TcpAssembleFuncPointer>  TcpAssembleFunctions = {
        {TcpFramePool.previewEvent,assemblePreviewEvent},
        {TcpFramePool.loadExperEvent,assembleLoadExperEvent},
        {TcpFramePool.askConnectedStateEvent,assembleAskConnectedStateEvent},
        {TcpFramePool.askActivateCodeEvent,assembleAskActivateCodeEvent},
        {TcpFramePool.adjustBrightEvent,assembleAdjustBrightEvent},
        {TcpFramePool.toggleChannelEvent,assembleToggleChannelEvent},
        {TcpFramePool.adjustLensEvent,assembleAdjustLensEvent},
        {TcpFramePool.moveMachineEvent,assembleMoveMachineEvent},
        {TcpFramePool.stopExperEvent,assembleStopExperEvent},
        {TcpFramePool.toggleObjectiveEvent, assembleToggleObjectiveEvent},
        {TcpFramePool.recordVideoEvent, assembleRecordVideoEvent},
        {TcpFramePool.manualFocusEvent, assembleManualFocusEvent},
        {TcpFramePool.autoFocusEvent, assembleAutoFocusEvent},
        {TcpFramePool.channelMergeEvent, assembleChannelMergeEvent},
        {"test0x0",assemble_test0x0},
        {"test0x1",assemble_test0x1},
};


#endif //EOSI_SOCKET_H
