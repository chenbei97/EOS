#ifndef EOSI_PUBLIC_H
#define EOSI_PUBLIC_H

// (1) 头文件包含
#include "qapplication.h"

#include "qlayout.h"
#include "qboxlayout.h"
#include "qformlayout.h"
#include "qgridlayout.h"

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
#include "qeventloop.h"
#include "qevent.h"
#include "qthread.h"
#include "qdebug.h"

// (2) 常量定义
#define LOG (qDebug()<<"["<<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] ")

#endif //EOSI_PUBLIC_H
