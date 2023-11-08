/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-08 17:16:50
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-08 17:17:29
 * @FilePath: \EOS\component\src\tool\configreadwrite.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "configreadwrite.h"

ConfigReadWrite::ConfigReadWrite(QObject*parent):JsonReadWrite(parent)
{

}

void ConfigReadWrite::parseObject(const QJsonObject &object)
{// 递归解析对象,取出对象的所有key然后最终存放到字典内
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