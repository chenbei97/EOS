/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 16:37:55
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 16:46:32
 * @FilePath: \EOS\component\include\tool\tool.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_TOOL_H
#define EOSI_TOOL_H

// (1) 头文件
#include "qjsonarray.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qjsonvalue.h"
#include "qsqldatabase.h"
#include "qsqlquery.h"
#include "qsqlerror.h"
#include "qsqldriver.h"
#include "qsqlfield.h"
#include "qsqlindex.h"
#include "qsqlrecord.h"
#include "qsqlresult.h"
#include "qfile.h"
#include "qfileinfo.h"
#include "qtextstream.h"
#include "qdebug.h"
#include "qdatetime.h"

#include "sqlstatements.h"

// (2) 定义常量
#define LOG (qDebug()<<"["<<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] ")
#define SqlExecFailedLOG (qDebug()<<"[" \
    <<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] sql exec failed! error is ")

// (3) 导出定义
#if defined(COMPONENT_LIBRARY)
#  define COMPONENT_IMEXPORT Q_DECL_EXPORT
#else
#  define COMPONENT_IMEXPORT Q_DECL_IMPORT
#endif

extern SQLType CurrentSqlType; // 包含本头文件的能够使用这个变量
#define CurrentSqliteType (CurrentSqlType==SQLType::Sqlite)
// 读写的指针
class SqliteReadWrite;
class MySqlReadWrite;
#define SqliteReadWritePointer (&SqliteReadWrite::instance())
#define MysqlReadWritePointer (&MySqlReadWrite::instance())
#endif //EOSI_TOOL_H
