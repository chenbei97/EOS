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
#include "qmutex.h"
#include "qthread.h"
#include "alias.h"
#include "qtimer.h"
#include "fields.h"
#include "constant.h"

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
//    object[FrameField] = TcpFramePool.frame0x0001;
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
