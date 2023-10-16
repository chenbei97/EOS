/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 16:35:23
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 16:36:31
 * @FilePath: \EOS\component\include\tool\jsonreadwrite.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_JSONREADWRITE_H
#define EOSI_JSONREADWRITE_H

#include "tool.h"

class COMPONENT_IMEXPORT JsonReadWrite : public QObject
{
Q_OBJECT
public:
    explicit JsonReadWrite(QObject *parent = nullptr);
    QByteArray readJson(const QString&path);

    void parseJson(const QByteArray&json);

    void writeJson(const QString&path,const QJsonObject&object);
    void writeJson(const QString&path,const QJsonArray&array);
    void writeJson(const QString&path,const QByteArray&json);

    QVariantMap map() const;
private:
    void parseObject(const QJsonObject& object);
    void parseArray(const QJsonArray& array);
protected:
    QVariantMap parseData;
signals:

};
#endif //EOSI_JSONREADWRITE_H
