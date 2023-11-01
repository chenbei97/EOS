/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 16:35:31
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 16:36:19
 * @FilePath: \EOS\component\src\tool\jsonreadwrite.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "jsonreadwrite.h"

JsonReadWrite::JsonReadWrite(QObject *parent) : QObject(parent)
{

}

void JsonReadWrite::parseJson(const QByteArray &json)
{
    parseData.clear();
    auto doc = QJsonDocument::fromJson(json);
    //LOG<<"json = "<<json<<" \ndoc = "<<doc; // 这里json可能是多条命令不符合格式导致doc为空
    if (!doc.object().isEmpty()) {
        parseObject(doc.object());
        return;
    }

    if (!doc.array().isEmpty()) {
        parseArray(doc.array());
        return;
    }
}

void JsonReadWrite::parseObject(const QJsonObject& object)
{// 递归解析对象,取出对象的所有key然后最终存放到字典内
    foreach(auto key,object.keys()) // 对象下的所有键值
    {
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

void JsonReadWrite::parseArray(const QJsonArray& array)
{ // 递归解析数组直到遇到常量,常量对象单独放置
    foreach(auto var, array)
    {// 特殊常量的处理,针对特定的值处理没有这个必要
        if (var.isObject())
            parseObject(var.toObject());
        else if (var.isArray())
            parseArray(var.toArray());
        else {}; // 发送json不会发送无意义的常量,这里无需处理
    }
}

QByteArray JsonReadWrite::readJson(const QString& path)
{ // 直接传递json文件的路径，外部调用有义务检测path的合法性
    // 只是返回纯文本,一般不使用而是直接解析parseJson
    QByteArray json;

    QFile file(path);
    if (file.open(QIODevice::ReadOnly|QIODevice::Text|QIODevice::Truncate))
    {
        QTextStream stream(&file);
        stream.setCodec("utf-8");
        //json = stream.readAll().toUtf8();
        json = file.readAll();
        file.close();
    }
    return json;
}

void JsonReadWrite::writeJson(const QString&path,const QJsonObject&object)
{
    auto doc = QJsonDocument(object);
    auto json = doc.toJson();
    writeJson(path,json);
}

void JsonReadWrite::writeJson(const QString&path,const QJsonArray&array)
{
    auto doc = QJsonDocument(array);
    auto json = doc.toJson();
    writeJson(path,json);
}

void JsonReadWrite::writeJson(const QString&path,const QByteArray&json)
{ // 调用本函数外部有义务检测path的合法性
    QFile file(path);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.setCodec("utf-8");
        stream << json;
        //file.write(json);
        file.close();
    }
}

QVariantMap JsonReadWrite::map() const
{
    return parseData;
}
