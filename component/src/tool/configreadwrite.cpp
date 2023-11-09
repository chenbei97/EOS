/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-08 17:16:50
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-09 09:12:23
 * @FilePath: \EOS\component\src\tool\configreadwrite.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "configreadwrite.h"

ConfigReadWrite::ConfigReadWrite(QObject*parent):JsonReadWrite(parent)
{

}

void ConfigReadWrite::parseObject(const QJsonObject &object)
{// 递归解析对象,取出对象的所有key然后最终存放到字典内
    if (!object.keys().contains(CameraChannelField))
        parseData[CameraChannelField] = QVariantMap();

    foreach(auto key,object.keys()) // 对象下的所有键值
    {
        if (key == GroupField){
            parseGroupField(key,object.value(key));
            continue;
        }
        if (key == CameraChannelField) {
            parseCameraChannelField(key,object.value(key));
            continue;
        }

        auto var = object.value(key);
        if (var.isObject())
            parseObject(var.toObject());
        else if (var.isArray())
        {
            auto arr = var.toArray();
            if (arr.isEmpty()) parseData[key] = "[]";
            else parseArray(arr);
        }
        else {
            auto v = var.toVariant();
            parseData[key] = v;
        }
    }
}

void ConfigReadWrite::parseCameraChannelField(const QJsonValue &key, const QJsonValue &value)
{
    Q_ASSERT(key == CameraChannelField);

    auto arr = value.toArray(); // 字段的值是个列表[],有多个{},{}对象

    QVariantMap m;
    foreach(auto obj, arr) {
        auto object = obj.toObject();//obj是个jsonValue需要先转换
        QVariantMap tmp;
        tmp[BrightField] = object[BrightField].toString();
        tmp[ExposureField] = object[ExposureField].toString();
        tmp[GainField] = object[GainField].toString();

        auto channel_idx = object[ChannelField].toString().toUInt();
        auto channel = ChannelFields[channel_idx];
        m[channel] = tmp;
    }

    parseData[CameraChannelField] = m;
}

void ConfigReadWrite::parseGroupField(const QJsonValue &key, const QJsonValue &value)
{
    Q_ASSERT(key == GroupField);

    auto arr = value.toArray(); // 字段的值是个列表[],有多个{},{}对象

    QVariantMap m;
    foreach(auto obj, arr) {
        auto object = obj.toObject();
        Q_ASSERT(object.keys().count() == 1);

        QVariantMap  grouptmp;
        auto groupname = object.keys().at(0); // 用户的组名称

        // "A组"的值是个列表,里边的值都是{},{},{},每个{}都有3个key:points,x,y代表孔坐标和孔指定的视野坐标
        auto groupvalue = object[groupname].toArray();

        // A组的每个孔需要解析2个信息,孔坐标和视野坐标列表
        for(int i = 0; i < groupvalue.count(); ++i) {
            auto holeobject = groupvalue[i].toObject();
            auto viewpointsArr = holeobject[PointsField].toArray(); // viewpoints是个列表

            auto holepoint = QPoint(holeobject[XField].toInt(),holeobject[YField].toInt());
            QPointVector holeviewpoints;
            foreach(auto ptVal,viewpointsArr) {
                auto pt = ptVal.toObject();
                holeviewpoints<<QPoint(pt[XField].toInt(),pt[YField].toInt());
            }

            QVariantMap holetmp;
            holetmp[PointsField].setValue(holeviewpoints);
            holetmp[HoleCoordinateField] = holepoint;

            grouptmp[QString::number(i)] = holetmp;
        }

        m[groupname] = grouptmp;
    }

    parseData[GroupField] = m;
}