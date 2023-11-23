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
    LoadExperEvent, // 启动实验事件
    AskConnectedStateEvent, // 询问套接字连接状态事件
    AskActivateCodeEvent,// 请求激活码事件
    AdjustBrightEvent, // 滑动条调整brght事件
    ToggleChannelEvent, // 切换通道关灯开灯事件
    AdjustLensEvent, // 上下左右调整镜头事件
    MoveMachineEvent,// 移动点击到指定物镜位置事件
    StopExperEvent, // 停止实验事件
    ToggleObjectiveEvent, // 切换物镜动电机事件
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
} TcpFrameList;

const QFieldList TcpUsedFrameList { // 用于解析时检测返回的帧是否正确,在这个列表内
        QString::number(PreviewEvent),QString::number(LoadExperEvent),QString::number(AskConnectedStateEvent),
        QString::number(AskActivateCodeEvent),QString::number(AdjustBrightEvent),QString::number(ToggleChannelEvent),
        QString::number(AdjustLensEvent),QString::number(MoveMachineEvent),QString::number(StopExperEvent),
        QString::number(ToggleObjectiveEvent),
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
    const QString capture_channel = CaptureChannelField;//配置过相机参数的所有通道
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
    // global
    const QString app = AppSelectField;
    // wellpattern
    const QString group = GroupField;
    const QString x = XField;
    const QString y = YField;
    const QString points = PointsField;
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
    const QString state = StateField;
};

typedef struct {
    FieldPreviewEvent fieldPreviewEvent;
    FieldLoadExperEvent fieldLoadExperEvent;
    FieldAskConnectedStateEvent fieldAskConnectedStateEvent;
    FieldAskActivateCodeEvent fieldAskActivateCodeEvent;
    FieldAdjustBrightEvent fieldAdjustBrightEvent;
    FieldToggleChannelEvent fieldToggleChannelEvent;
    FieldAdjustLensEvent fieldAdjustLensEvent;
    FieldMoveMachineEvent fieldMoveMachineEvent;
    FieldStopExperEvent fieldStopExperEvent;
    FieldToggleObjectiveEvent fieldToggleObjectiveEvent;
} TcpFieldList;

static QJsonDocument TcpAssemblerDoc;
static const TcpFrameList TcpFramePool;
static const TcpFieldList TcpFieldPool;

#define FieldPreviewEvent TcpFieldPool.fieldPreviewEvent
#define FieldLoadExperEvent TcpFieldPool.fieldLoadExperEvent
#define FieldAskConnectedStateEvent TcpFieldPool.fieldAskConnectedStateEvent
#define FieldAskActivateCodeEvent TcpFieldPool.fieldAskActivateCodeEvent
#define FieldAdjustBrightEvent TcpFieldPool.fieldAdjustBrightEvent
#define FieldToggleChannelEvent TcpFieldPool.fieldToggleChannelEvent
#define FieldAdjustLensEvent TcpFieldPool.fieldAdjustLensEvent
#define FieldMoveMachineEvent TcpFieldPool.fieldMoveMachineEvent
#define FieldStopExperEvent TcpFieldPool.fieldStopExperEvent
#define FieldToggleObjectiveEvent TcpFieldPool.fieldToggleObjectiveEvent

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
    object[FieldPreviewEvent.viewsize] = m[HoleViewSizeField].toInt();
    //object[FieldPreviewEvent.current_channel] = m[CurrentChannelField].toString();
    auto holepoint = m[HoleCoordinateField].toPoint();
    auto viewpoint = m[ViewCoordinateField].toPoint();
    object[FieldPreviewEvent.hole_x] = holepoint.x();
    object[FieldPreviewEvent.hole_y] = holepoint.y();
    object[FieldPreviewEvent.view_x] = viewpoint.x();
    object[FieldPreviewEvent.view_y] = viewpoint.y();
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
    auto toolinfo = m[PreviewToolField].value<QVariantMap>();
    auto patterninfo = m[PreviewPatternField].value<QVariantMap>();
#ifdef usetab
    auto experinfo = m[ExperToolField].value<QVariantMap>();
#endif
    QJsonObject object;
    object[FrameField] = LoadExperEvent;

    // wellbox
    object[FieldLoadExperEvent.manufacturer] = toolinfo[FieldLoadExperEvent.manufacturer].toInt();
    object[FieldLoadExperEvent.wellbrand] = toolinfo[FieldLoadExperEvent.wellbrand].toInt();
    object[FieldLoadExperEvent.wellsize] = toolinfo[FieldLoadExperEvent.wellsize].toInt();

    // objectivebox
    object[FieldLoadExperEvent.objective_location] = toolinfo[FieldLoadExperEvent.objective_location].toInt();
    object[FieldLoadExperEvent.objective_descrip] = toolinfo[FieldLoadExperEvent.objective_descrip].toString();//就是传递原字符串不需要改
    object[FieldLoadExperEvent.objective] = getIndexFromFields(toolinfo[FieldLoadExperEvent.objective].toString()).toInt();
    object[FieldLoadExperEvent.objective_type] = toolinfo[FieldLoadExperEvent.objective_type].toInt();

    // camerabox
    auto capture_channels = toolinfo[FieldLoadExperEvent.capture_channel].toStringList(); // 设置过相机参数的所有通道
    QJsonArray channelCameraInfo; // 组列表
    if (!capture_channels.isEmpty()) { // 例如channels= {"PH","GFP"}

        QJsonArray channelInfoArr; // 保存所有通道信息的列表,这个是"PH"/"BR"等通道字段的值

        foreach(auto currentChannel, capture_channels) {
            // 通道信息用QVarintMap保存的,有4个key,channel,exposure,gain,bright
            auto channelInfo = toolinfo[currentChannel].value<QVariantMap>();

            Q_ASSERT(currentChannel == channelInfo[ChannelField]);
            auto exposure = channelInfo[ExposureField].toInt();
            auto gain = channelInfo[GainField].toInt();
            auto bright = channelInfo[BrightField].toInt();
            //LOG<<exposure<<gain<<bright;

            QJsonObject currentChannelInfoObject; // 每个通道的3个信息
            currentChannelInfoObject[ChannelField] = getIndexFromFields(currentChannel).toInt(); // 如PH转为0
            currentChannelInfoObject[ExposureField] = exposure;
            currentChannelInfoObject[GainField] = gain;
            currentChannelInfoObject[BrightField] = bright;

            channelInfoArr.append(currentChannelInfoObject); // 所有通道的信息组成列表,这个列表是camera_channel字段的值
        }

        object[CameraChannelField] = channelInfoArr; // camera_channel字段,存储了所有通道的配置信息
    }

    // focusbox
    object[FieldLoadExperEvent.focus] = toolinfo[FieldLoadExperEvent.focus].toDouble();
    object[FieldLoadExperEvent.focus_step] = toolinfo[FieldLoadExperEvent.focus_step].toDouble();

#ifdef usetab
    // zstackbox
    object[FieldLoadExperEvent.zstack] = experinfo[FieldLoadExperEvent.zstack].toInt();
    object[FieldLoadExperEvent.stitch] = experinfo[FieldLoadExperEvent.stitch].toInt();

    // experbox
    object[FieldLoadExperEvent.total_time] = experinfo[FieldLoadExperEvent.total_time].toInt();
    object[FieldLoadExperEvent.duration_time] = experinfo[FieldLoadExperEvent.duration_time].toInt();
    object[FieldLoadExperEvent.start_time] = experinfo[FieldLoadExperEvent.start_time].toString();
    object[FieldLoadExperEvent.channel] = experinfo[FieldLoadExperEvent.channel].toInt();
    object[FieldLoadExperEvent.is_schedule] = experinfo[FieldLoadExperEvent.is_schedule].toInt();
#else
    // zstackbox
    object[FieldLoadExperEvent.zstack] = toolinfo[FieldLoadExperEvent.zstack].toInt();
    object[FieldLoadExperEvent.stitch] = toolinfo[FieldLoadExperEvent.stitch].toInt();

    // experbox
    object[FieldLoadExperEvent.total_time] = toolinfo[FieldLoadExperEvent.total_time].toInt();
    object[FieldLoadExperEvent.duration_time] = toolinfo[FieldLoadExperEvent.duration_time].toInt();
    object[FieldLoadExperEvent.start_time] = toolinfo[FieldLoadExperEvent.start_time].toString();
    object[FieldLoadExperEvent.channel] = toolinfo[FieldLoadExperEvent.channel].toInt();
    object[FieldLoadExperEvent.is_schedule] = toolinfo[FieldLoadExperEvent.is_schedule].toInt();
#endif

    // 其它全局信息
    object[FieldLoadExperEvent.app] = m[FieldLoadExperEvent.app].toInt();

    // 组-孔-视野的信息
    QJsonArray arr; // "group"的值是个列表 arr group=[{},{},{}]
    foreach(auto group, patterninfo.keys()) {
        QJsonObject groupObject; // arr有多个groupObject对象,表示每个组的信息,"a组","b组" 每个小{}
        QJsonArray groupValues; // groupObject的值是个列表,也就是"a组"的值是个列表,包含孔信息

        auto groupinfo = patterninfo[group].value<QVariantMap>();

        //LOG<<"hole keys = "<<groupinfo.keys();
        foreach(auto holename,groupinfo.keys()) {
            auto holeinfo = groupinfo[holename].value<QVariantMap>();

            auto grouppoints = holeinfo[HoleGroupCoordinatesField].value<QPointVector>();

            auto coordinate = holeinfo[HoleCoordinateField].toPoint();
            auto viewsize = holeinfo[HoleViewSizeField].toSize();
            auto viewpoints = holeinfo[HoleViewPointsField].value<QPointVector>();

            //auto expertype = holeinfo[HoleExperTypeField].toString();
            //auto medicine = holeinfo[HoleMedicineField].toString();
            //auto dose = holeinfo[HoleDoseField].toString();
            //auto unit = holeinfo[HoleDoseUnitField].toString();

            //LOG<<coordinate<<viewsize<<viewpoints<<expertype<<medicine<<dose<<unit<<holename<<group<<grouppoints;

            QJsonObject holeObject;// "a组"=[{},{},{}],是每个小的{}
            //holeObject[HoleCoordinateField] = QString("(%1,%2)").arg(QChar(coordinate.x()+65)).arg(coordinate.y());
            holeObject[FieldLoadExperEvent.x] = coordinate.x();
            holeObject[FieldLoadExperEvent.y] = coordinate.y();

            QJsonArray pointValues; // point字段的值是个列表
            for(int viewcount = 0; viewcount<viewpoints.count(); ++viewcount) {
                QJsonObject viewObject;
                auto viewpoint_x = viewpoints[viewcount].x();
                auto viewpoint_y = viewpoints[viewcount].y();

                //viewObject[HoleCoordinateField] = QString("(%1,%2)").arg(viewpoint_x).arg(viewpoint_y);
                viewObject[FieldLoadExperEvent.x] = viewpoint_x;
                viewObject[FieldLoadExperEvent.y] = viewpoint_y;
                pointValues.append(viewObject);
            }

            holeObject[FieldLoadExperEvent.points] = pointValues;
            groupValues.append(holeObject);
        }
        groupObject[group] = groupValues;
        arr.append(groupObject);
    }
    object[FieldLoadExperEvent.group] = arr;
    //LOG<<object;
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
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
    LOG<<PrependSeparateField(PrependField(json,length));

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
    object[FieldAdjustBrightEvent.bright] = m[BrightField].toString();
    object[FieldAdjustBrightEvent.current_channel] = m[CurrentChannelField].toString();
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
    object[FieldToggleChannelEvent.bright] = m[BrightField].toString();
    object[FieldToggleChannelEvent.turnoff_light] = m[TurnOffLight].toString();
    object[FieldToggleChannelEvent.current_channel] = m[CurrentChannelField].toString();
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
    object[FieldAdjustLensEvent.direction] = m[DirectionField].toString();
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
    object[FieldMoveMachineEvent.objective_loc] = m[ObjectiveLocationField].toString();
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
    object[FieldStopExperEvent.stop] = m[StopField].toString();
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

/*---------以下都是临时测试函数,以后可以注释掉-----------------*/
static QByteArray assemble_test0x0(QCVariantMap m)
{ // test0x0会传来x,y,frame字段
    QJsonObject object;
    object[FrameField] = "test0x0";
    object["x"] = m["x"].toString();
    object["y"] = m["y"].toString();

    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}
static QVariant parse_test0x0(QCVariantMap m)
{// test0x0会返回x,y,frame,path字段
    if (!m.keys().contains("x")) return QVariant();
    if (!m.keys().contains("y")) return QVariant();
    if (!m.keys().contains("path")) return QVariant();
    if (!m.keys().contains(FrameField)) return QVariant();

    auto path = m["path"].toString();
    return path;
}
static QByteArray assemble_test0x1(QCVariantMap m)
{ // test0x1会传来equip,frame字段
    QJsonObject object;
    object[FrameField] = "test0x1";
    object["equip"] = m["equip"].toString();

    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}
static QVariant parse_test0x1(QCVariantMap m)
{// test0x1会返回equip,frame字段
    if (!m.keys().contains("equip")) return QVariant();
    if (!m.keys().contains(FrameField)) return QVariant();

    auto equip = m["equip"].toString();
    return equip;
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
        {"test0x0",parse_test0x0},
        {"test0x1",parse_test0x1},
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
        {"test0x0",assemble_test0x0},
        {"test0x1",assemble_test0x1},
};


#endif //EOSI_SOCKET_H
