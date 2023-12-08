/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-08 17:16:02
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-08 17:17:19
 * @FilePath: \EOS\component\include\tool\configreadwrite.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_MAIN_CORE_PY_CONFIGREADWRITE_H
#define EOS_MAIN_CORE_PY_CONFIGREADWRITE_H

#include "jsonreadwrite.h"
#include "fields.h"
#include "utilities.h"

class COMPONENT_IMEXPORT ConfigReadWrite: public JsonReadWrite
{
    Q_OBJECT
public:
    explicit ConfigReadWrite(QObject *parent = nullptr);
private:
    virtual void parseObject(const QJsonObject& object) override;
    void parseGroupField(const QJsonValue &key,const QJsonValue &value);
    void parseCameraChannelField(const QJsonValue &key,const QJsonValue &value);
};

#endif //EOS_MAIN_CORE_PY_CONFIGREADWRITE_H
