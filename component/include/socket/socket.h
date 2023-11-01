/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 16:24:37
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 16:25:07
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
#include "qdebug.h"
#include "qdatetime.h"
#include "qmutex.h"
#include "qthread.h"
#include "alias.h"
#include "qtimer.h"

// (2) 定义常量
#define CURRENT_PATH (QDir::currentPath())
#define CURRENT_THREAD (QThread::currentThread())
#define LOG (qDebug()<<"["<<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] ")
#define SqlExecFailedLOG (qDebug()<<"[" \
    <<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] sql exec failed! error is ")
#define SocketPort 3000
#define LocalHost "localhost"
#define SocketWaitTime 3000
static const char* ConfigFileSuffix = ".eos";
static const char* MatchConfigFileSuffix = "*.eos";
#define DefaultDateTimeFormat "yyyy_MM_dd hh::mm"
static const char* FrameField = "frame";
static const char* SeparateField = "@@@";
#define AppendSeparateField(s) (s+SeparateField)

static const char* ManufacturerField = "manufacturer";

static const QFieldList ManufacturerFields = {
        "manufacturer1","manufacturer2","manufacturer3","manufacturer4",
};

static const QFieldList WellsizeFields = {
        "6","24","96","384",
};
static const char* WellsizeField = "wellsize";
static const char* WellsizeFieldLabel = "wellsize: ";

static const char* ObjectiveField = "objective";
static const char* ObjectiveMagnificationField = "objective_magnification";
static const char* ObjectiveTypeField = "objective_type";
static const char* ObjectiveFieldLabel = "objective: ";
#define ObjectivePH "ph"
#define ObjectiveBR "br"
static const QFieldList ObjectiveTypeFields = {
        ObjectivePH,ObjectiveBR,
};
#define Objective4x "4x"
#define Objective10x "10x"
#define Objective20x "20x"
#define Objective40x "40x"
static const QFieldList ObjectiveMagnificationFields = {
        Objective4x,Objective10x,Objective20x,Objective40x
};
static const QFieldList ChannelFields = {
        "BR","PH","GFP","RFP","DAPI"
};
static const char* BRField = "BR";
static const char* PHField = "PH";
static const char* GFPField = "GFP";
static const char* RFPField = "RFP";
static const char* DAPIField = "DAPI";
static const char* ChannelField = "channel";
static const char* IsCheckedField = "isChecked";
static const char* ChannelFieldLabel = "channel: ";

static const QFieldList ImageFormatFields = {
        "jpg","png","ico","bmp"
};

static const QFieldList VideoFormatFields = {
        "avi","wmv"
};

static const QFieldList Brand1Fields = {
        "brand1_6","brand1_24","brand1_96","brand1_384"
};
static const QFieldList Brand2Fields = {
        "brand2_6","brand2_24","brand2_96","brand2_384"
};
static const QFieldList Brand3Fields = {
        "brand3_6","brand3_24","brand3_96","brand3_384"
};
static const QFieldList Brand4Fields = {
        "brand4_6","brand4_24","brand4_96","brand4_384"
};

/*
    6孔板
    4x: 10*10，
    10x: 27*27
    20x: 50*50
    40x: 116*116
 */
#define view_well_6_4x 10
#define view_well_6_10x 27
#define view_well_6_20x 50
#define view_well_6_40x 116

/*  24孔板
    4x: 4*4，
    10x: 14*14
    20x: 20*20
    40x: 45*45
*/
#define view_well_24_4x 4
#define view_well_24_10x 14
#define view_well_24_20x 20
#define view_well_24_40x 45

/*  96孔板
    4x: 2*2，
    10x: 5*5
    20x: 10*10
    40x: 24*24
*/
#define view_well_96_4x 4
#define view_well_96_10x 14
#define view_well_96_20x 20
#define view_well_96_40x 45

/*  384孔板
    4x: 1*1，
    10x: 3*3
    20x: 5*5
    40x: 12*12
*/
#define view_well_384_4x 1
#define view_well_384_10x 3
#define view_well_384_20x 5
#define view_well_384_40x 12

const QVector<QVector<QVector<int>>> ViewCircleMapFields = {
        {//不同的厂家
            {view_well_6_4x,view_well_6_10x,view_well_6_20x,view_well_6_40x},// 品牌1
            {view_well_24_4x,view_well_24_10x,view_well_24_20x,view_well_24_40x},// 品牌2
            {view_well_96_4x,view_well_96_10x,view_well_96_20x,view_well_96_40x},// 品牌3
            {view_well_384_4x,view_well_384_10x,view_well_384_20x,view_well_384_40x},// 品牌4
        },

        {
                {view_well_6_4x,view_well_6_10x,view_well_6_20x,view_well_6_40x},// 品牌1
                {view_well_24_4x,view_well_24_10x,view_well_24_20x,view_well_24_40x},// 品牌2
                {view_well_96_4x,view_well_96_10x,view_well_96_20x,view_well_96_40x},// 品牌3
                {view_well_384_4x,view_well_384_10x,view_well_384_20x,view_well_384_40x},// 品牌4
        },

        {
                {view_well_6_4x,view_well_6_10x,view_well_6_20x,view_well_6_40x},// 品牌1
                {view_well_24_4x,view_well_24_10x,view_well_24_20x,view_well_24_40x},// 品牌2
                {view_well_96_4x,view_well_96_10x,view_well_96_20x,view_well_96_40x},// 品牌3
                {view_well_384_4x,view_well_384_10x,view_well_384_20x,view_well_384_40x},// 品牌4
        },

        {
            {view_well_6_4x,view_well_6_10x,view_well_6_20x,view_well_6_40x},// 品牌1
            {view_well_24_4x,view_well_24_10x,view_well_24_20x,view_well_24_40x},// 品牌2
            {view_well_96_4x,view_well_96_10x,view_well_96_20x,view_well_96_40x},// 品牌3
            {view_well_384_4x,view_well_384_10x,view_well_384_20x,view_well_384_40x},// 品牌4
        },
};
static const char* BrandField = "brand";
static const char* BrandFieldLabel = "brand: ";


static const QFieldList DishFields = {
        "dish1","dish2","dish3"
};
static const char* DishField = "dish";
static const char* DishFieldLabel = "dish: ";

static const QFieldList FlaskFields = {
        "flask1","flask2","flask3"
};
static const char* FlaskField = "flask";
static const char* FlaskFieldLabel = "flask: ";

static const QFieldList SlideFields = {
        "slide1","slide2","slide3"
};
static const char* SlideField = "slide";
static const char* SlideFieldLabel = "slide: ";

static const char* ExposureField = "exposure";
static const char* ExposureFieldLabel = "exposure: ";
static const char* ExposureFieldUnit = "ms";
#define ExposureLowLimit 0
#define ExposureUpLimit 15000
static const char* GainField = "gain";
static const char* GainFieldLabel = "gain: ";
#define GainLowLimit 100
#define GainUpLimit 5000
static const char* GainFieldUnit = "%";
static const char* BrightField = "bright";
static const char* BrightFieldLabel = "bright: ";
#define BrightLowLimit 0
#define BrightUpLimit 100

static const char* IsScheduleField = "is_schedule";
static const char* TotalTimeField = "total_time";
static const char* DurationTimeField = "duration_time";
static const char* StartTimeField = "start_time";
static const char* ExperChannelField = "exper_channel";

static const QList<QFieldList> SocketNeedMapFields{
        Brand1Fields,Brand2Fields,Brand3Fields,Brand4Fields,
        ChannelFields,ManufacturerFields,ObjectiveMagnificationFields,
};

static QString getIndexFromFields (QCString field)
{ // 用于把这些字段统一映射为0,1,2,3方便下位机读取
    // 例如4x,jpg,bright,avi都应该映射为0
    QString idx = "";
    foreach(auto fields, SocketNeedMapFields) { // 仅限于从这些字段查找
    auto index = fields.indexOf(field);
        if (index>=0){
            idx = QString::number(index);
            break; // 找到
        }
    }
    return idx;
}

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

// (3) 导出定义
#if defined(COMPONENT_LIBRARY)
#  define COMPONENT_IMEXPORT Q_DECL_EXPORT
#else
#  define COMPONENT_IMEXPORT Q_DECL_IMPORT
#endif

typedef struct { // 注册过的帧头命令
    const QString frame0x0000 = "0x0000";
    const QString frame0x0001 = "0x0001";
    const QString frame0x0002 = "0x0002";
    const QString frame0x0003 = "0x0003";
    const QString frame0x0004 = "0x0004";
    const QString frame0x1000 = "0x1000";
} TcpFrameList;

struct Field0x0001{
    // 不需要传递给下位机(但是Tcp用过的字段)
    const QString path = "path"; // 本命令需要回复路径
    // 传给下位机json涉及的字段
    // wellbox
    const QString manufacturer = ManufacturerField;
    const QString wellbrand = BrandField;
    // objective
    const QString objective = ObjectiveField;
    const QString objective_magnification = ObjectiveMagnificationField;
    //
    const QString wellsize = WellsizeField;

    const QString channel = ChannelField;
    const QString ph = PHField;
    const QString gfp = GFPField;
    const QString rfp = RFPField;
    const QString dapi = DAPIField;
    const QString exposure = ExposureField;
    const QString gain = GainField;
    const QString bright = BrightField;

//    const QString welldish = DishField;
//    const QString wellflask = FlaskField;
//    const QString wellslide = SlideField;
    //const QString imageformat = "imageformat";
    //const QString videoformat = "videoformat";
    //const QString x = "x";
    //const QString y = "y";
    //const QString order = "order";
};

struct Field0x0002 {
    const QString state = "state"; // 程序启动时发送命令询问是否连接上了
};

struct Field0x0003 {
    const QString activate_code = "activate_code"; // 询问激活码
};

typedef struct {
    //Field0x0000 field0x0000;
    Field0x0001 field0x0001;
    Field0x0002 field0x0002;
    Field0x0003 field0x0003;
} TcpFieldList;

static QJsonDocument TcpAssemblerDoc;
static const TcpFrameList TcpFramePool;
static const TcpFieldList TcpFieldPool;

//#define Field0x0000 TcpFieldPool.field0x0000
#define Field0x0001 TcpFieldPool.field0x0001
#define Field0x0002 TcpFieldPool.field0x0002
#define Field0x0003 TcpFieldPool.field0x0003

static QVariant parse0x0000(QCVariantMap m)
{
    Q_UNUSED(m);
    QVariant d;
    return d;
}

static QByteArray assemble0x0000(QCVariantMap m)
{
    Q_UNUSED(m);
    QJsonObject object;
    object[FrameField] = TcpFramePool.frame0x0000;

    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

static QVariant parse0x0001(QCVariantMap m)
{ // preview界面调整各种参数时发送的toolInfo+patternInfo 回复要显示的图片路径
    if (!m.keys().contains(Field0x0001.path)) return QVariant();
    if (!m.keys().contains(FrameField)) return QVariant();
    auto path = m[Field0x0001.path].toString();
    return path;
}

static QByteArray assemble0x0001(QCVariantMap m)
{// preview界面调整各种参数时发送的toolInfo+patternInfo
    QJsonObject object;
    object[FrameField] = TcpFramePool.frame0x0001;
//
//    object[Field0x0001.wellbrand] = getIndexFromFields(m[Field0x0001.wellbrand].toString());
//    object[Field0x0001.wellsize] = getIndexFromFields(m[Field0x0001.wellsize].toString());
//    object[Field0x0001.welldish] = getIndexFromFields(m[Field0x0001.welldish].toString());
//    object[Field0x0001.wellslide] = getIndexFromFields(m[Field0x0001.wellslide].toString());
//    object[Field0x0001.wellflask] = getIndexFromFields(m[Field0x0001.wellflask].toString());
//    object[Field0x0001.objective] = getIndexFromFields(m[Field0x0001.objective].toString());
//
//    auto channels = m[Field0x0001.channel].value<QStringList>();
//    // 应该是个QStringList对象,previewtool.cpp加进去的
//
//    QJsonArray channelCameraInfo; // 组列表
//    if (!channels.isEmpty()) { // 例如channels= {"PH","GFP"}
//
//        QJsonArray channelInfoArr; // 保存所有通道信息的列表,这个是channel字段的值
//
//        foreach(auto currentChannel, channels) {
//            // 通道信息用QVarintMap保存的,有4个key,channel,exposure,gain,bright
//            auto channelInfo = m[currentChannel].value<QVariantMap>();
//
//            Q_ASSERT(currentChannel == channelInfo[ChannelField]);
//            auto exposure = channelInfo[ExposureField].toString();
//            auto gain = channelInfo[GainField].toString();
//            auto bright = channelInfo[BrightField].toString();
//            //LOG<<exposure<<gain<<bright;
//
//            QJsonObject currentChannelInfoObject; // 每个通道的3个信息
//            currentChannelInfoObject[ChannelField] = getIndexFromFields(currentChannel); // 如PH转为0
//            currentChannelInfoObject[ExposureField] = exposure;
//            currentChannelInfoObject[GainField] = gain;
//            currentChannelInfoObject[BrightField] = bright;
//
//            channelInfoArr.append(currentChannelInfoObject); // 所有通道的信息组成列表,这个列表是channel字段的值
//        }
//
//        object[ChannelField] = channelInfoArr; // channel字段
//    }

    //LOG<<object;

    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

static QVariant parse0x0002(QCVariantMap m)
{ // 用于询问是否连接的命令,只要有回复不为空就ok,随便发
    if (!m.keys().contains(Field0x0002.state)) return QVariant();
    if (!m.keys().contains(FrameField)) return QVariant();
    auto text = m[Field0x0002.state].toString(); // 只要有回复就可
    return !text.isEmpty();
}

static QByteArray assemble0x0002(QCVariantMap m)
{ // 用于询问是否连接的命令,只要有回复不为空就ok,随便发
    Q_UNUSED(m);
    QJsonObject object;
    object[FrameField] = TcpFramePool.frame0x0002;
    object[Field0x0002.state] = "socket is connected?";
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
}

static QVariant parse0x0003(QCVariantMap m)
{ // 询问设备激活码,回复激活码
    if (!m.keys().contains(Field0x0003.activate_code)) return QVariant();
    if (!m.keys().contains(FrameField)) return QVariant();
    auto code = m[Field0x0003.activate_code].toString();
    return code;
}

static QByteArray assemble0x0003(QCVariantMap m)
{ // 询问设备激活码,回复激活码
    Q_UNUSED(m);
    QJsonObject object;
    object[FrameField] = TcpFramePool.frame0x0003;
    object[Field0x0003.activate_code] = "ask activate_code";
    TcpAssemblerDoc.setObject(object);
    auto json = TcpAssemblerDoc.toJson();
    return AppendSeparateField(json);
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
        {TcpFramePool.frame0x0000,parse0x0000},
        {TcpFramePool.frame0x0001,parse0x0001},
        {TcpFramePool.frame0x0002,parse0x0002},
        {TcpFramePool.frame0x0003,parse0x0003},
//        {TcpFramePool.frame0x0004,parse0x0004},
//        {TcpFramePool.frame0x1000,parse0x1000},
        {"test0x0",parse_test0x0},
        {"test0x1",parse_test0x1},
};

static QMap<QString,TcpAssembleFuncPointer>  TcpAssembleFunctions = {
        {TcpFramePool.frame0x0000,assemble0x0000},
        {TcpFramePool.frame0x0001,assemble0x0001},
        {TcpFramePool.frame0x0002,assemble0x0002},
        {TcpFramePool.frame0x0003,assemble0x0003},
//        {TcpFramePool.frame0x0004,assemble0x0004},
//        {TcpFramePool.frame0x1000,assemble0x1000},

        {"test0x0",assemble_test0x0},
        {"test0x1",assemble_test0x1},
};


#endif //EOSI_SOCKET_H
