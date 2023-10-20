/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 08:42:11
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 08:43:14
 * @FilePath: \EOS\component\include\socket\parsecontrol.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_PARSECONTROL_H
#define EOSI_PARSECONTROL_H

#include "parse.h"
#include "eventloop.h"

class COMPONENT_IMEXPORT ParserControl : public QObject
{
    Q_OBJECT
public:
    static ParserControl& instance();
    QVariant result() const;
    QString frame() const;
    QVariantMap response() const;
private:
    explicit ParserControl(QObject *parent = nullptr);
    ~ParserControl();
    QThread parsethread;
    Parse * parser;
    EventLoop loop;
    QVariant res;
    QString fram;
    QVariantMap parseresult;
signals:
    void parse(const QString& f, const QByteArray& msg);
    void parseResult(const QString& f, const QVariant& d);
};
#endif //EOSI_PARSECONTROL_H
