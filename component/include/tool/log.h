/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 15:44:38
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 16:02:41
 * @FilePath: \EOS\component\include\tool\log.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_LOG_H
#define EOSI_LOG_H

#include "tool.h"
#include "utilities.h"

class COMPONENT_IMEXPORT Log : public QObject
{
    Q_OBJECT
public:
    static Log& instance();
private:
    explicit Log(QObject*parent = nullptr);
    static void messageToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static QString filename;
    static QString path;
    static QtMessageHandler gDefaultHandler;
};
#endif //EOSI_LOG_H
