/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 13:22:02
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 14:06:26
 * @FilePath: \EOS\component\include\public.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_PUBLIC_H
#define EOSI_PUBLIC_H

// (1) 头文件包含
#include "widgets.h"
#include "datastructure.h"

#include "qlayout.h"
#include "qboxlayout.h"
#include "qformlayout.h"
#include "qgridlayout.h"

#include "qapplication.h"

#include "qfile.h"
#include "qfileinfo.h"
#include "qstandardpaths.h"
#include "qdir.h"
#include "qmessagebox.h"

#include "qpixmap.h"
#include "qimagereader.h"

#include "qaction.h"
#include "qtimer.h"
#include "qdatetime.h"

#include "qthread.h"
#include "qdebug.h"

// (2) 常量定义
#define LOG (qDebug()<<"["<<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] ")



#endif //EOSI_PUBLIC_H
