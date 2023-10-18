/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 08:33:47
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 08:43:10
 * @FilePath: \EOS\component\include\socket\parse.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_PARSE_H
#define EOSI_PARSE_H

#include "socket.h"
#include "jsonreadwrite.h"

class COMPONENT_IMEXPORT Parse: public QObject
{
    Q_OBJECT
public:
    explicit Parse(QObject*parent= nullptr);
    void parse(const QString&f,const QByteArray &msg);
private:
    QMutex mutex;
signals:
    void parseResult(const QString& f,const QVariant& d);
};
#endif //EOSI_PARSE_H
