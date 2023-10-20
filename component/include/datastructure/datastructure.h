/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:55:56
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:10:34
 * @FilePath: \EOS\component\include\datastructure\datastructure.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_DATASTRUCTURE_H
#define EOSI_DATASTRUCTURE_H
// (1) 头文件
#include "qsqlquerymodel.h"
#include "qstandarditemmodel.h"
#include "qsqltablemodel.h"
#include "qeventloop.h"
#include "qdatetime.h"
#include "qdebug.h"

// (2) 定义常量
#define CURRENT_PATH (QDir::currentPath)
#define CURRENT_THREAD (QThread::currentThread())
#define LOG (qDebug()<<"["<<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] ")


// (3) 导出定义
#if defined(COMPONENT_LIBRARY)
#  define COMPONENT_IMEXPORT Q_DECL_EXPORT
#else
#  define COMPONENT_IMEXPORT Q_DECL_IMPORT
#endif

#endif //EOSI_DATASTRUCTURE_H
