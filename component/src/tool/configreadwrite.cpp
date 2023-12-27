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

    for(auto key: object.keys()) // 对象下的所有键值
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
    for(auto obj: arr) {
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

    auto arr = value.toArray(); // group字段的值是个列表[],有多个{},{}对象,每个{}对象是1个组别的信息

    QVariantMap m;
    for(auto obj: arr) {
        auto object = obj.toObject(); // 组信息的{}对象,只有1个关键字,是以组名命名

        Q_ASSERT(object.keys().count() == 1);
        auto groupName = object.keys().at(0); // 用户的组名称
        auto groupHoleInfo = object[groupName].toArray();// "Negative Control 2"的值是个列表,列表的每个值都是该组的所有孔信息

        WellGroupInfo groupInfoMap;
        for(int i = 0; i < groupHoleInfo.count(); ++i) {
            auto holeInfoObject = groupHoleInfo[i].toObject();

            HoleInfo holeInfo;
            holeInfo.medicine = holeInfoObject[HoleMedicineField].toString();
            holeInfo.dose = holeInfoObject[HoleDoseField].toString();
            holeInfo.doseunit = holeInfoObject[HoleDoseUnitField].toString();

            holeInfo.coordinate = convertToPointF(holeInfoObject[HoleCoordinateField].toString()).toPoint();
            holeInfo.color = holeInfoObject[HoleGroupColorField].toString();
            holeInfo.group = holeInfoObject[HoleGroupNameField].toString();
            Q_ASSERT(holeInfo.group == groupName);
            //auto groupHoles = convertToPointFVector(holeInfoObject[HoleGroupCoordinatesField].toString()); // 不提供组坐标的写入和解析
            //holeInfo.allcoordinate = convertTo2DPointVector(holeInfoObject[HoleAllCoordinatesField].toString()); // 组信息和所有点信息可以自行计算也无需导入
            //holeInfo.allgroup = convertToSet(holeInfoObject[HoleAllGroupsField].toString());

#ifdef viewRowColUnEqual
            holeInfo.dimension = convertToDimension2D(holeInfoObject[HoleViewSizeField].toString());
#else
            holeInfo.viewsize = holeInfoObject[HoleViewSizeField].toInt();
#endif
            auto rect_str = holeInfoObject[HoleViewRectsField].toString();
            if (rect_str.isEmpty())
                holeInfo.viewrects = QRectFVector(); // 点模式下一定为空,区域模式可能没选视野也为空
            else holeInfo.viewrects = QRectFVector()<<convertToRectF(rect_str); // (0.71,0.205,0.1725,0.35)
            auto viewpt_str = holeInfoObject[HoleViewPointsField].toString();
            if (!viewpt_str.isEmpty()) // 有可能用户分了组但是却忘记选择视野导致viewpoints是空的
                holeInfo.viewpoints = convertToPointFVector(viewpt_str);
            else holeInfo.viewpoints = QPointFVector();
            //没有uipoints不导入

            groupInfoMap[holeInfoObject[HoleCoordinateField].toString()].setValue(holeInfo); // 用孔坐标做本孔信息的指代名称
        }

        m[groupName] = groupInfoMap;
    }

    parseData[GroupField] = m;
}