/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-17 15:44:08
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-17 16:02:46
 * @FilePath: \EOS\component\src\tool\log.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "log.h"

QString Log::path = "";
QString Log::filename = "";
QtMessageHandler Log::gDefaultHandler = nullptr;

Log& Log::instance()
{
    static Log log;
    return log;
}

Log::Log(QObject*parent):QObject(parent)
{
    path = QDir::currentPath()+"/log/";
    if (!pathExisted(path)) createPath(path);
    filename = path+QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss") + ".log";
    gDefaultHandler = qInstallMessageHandler(messageToFile);
}

void Log::messageToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
    QByteArray localMsg = msg.toUtf8();
//    auto name = QDir::currentPath()+"/log/"+QDateTime::currentDateTime()
//            .toString("yyyy_MM_dd") + ".log";
    //if (name != filename) name = filename;
    //QFile logfile(filename);
    QFile logfile(filename);
    logfile.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&logfile);
    stream.setCodec("utf-8");
    stream << localMsg << "\r\n";
    logfile.flush();
    logfile.close();
    mutex.unlock();
    if(gDefaultHandler)
    {
        gDefaultHandler(type,context,msg);
    }
}
