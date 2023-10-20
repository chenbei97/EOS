/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:17:22
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-16 15:09:26
 * @FilePath: \EOS\component\include\widgets\widgets.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_WIDGETS_H
#define EOSI_WIDGETS_H

// (1) 头文件

#include "qcombobox.h"
#include "qlineedit.h"
#include "qtextedit.h"
#include "qlabel.h"
#include "qgroupbox.h"
#include "qpushbutton.h"
#include "qsplitter.h"
#include "qspinbox.h"
#include "qcheckbox.h"
#include "qprogressbar.h"
#include "qtableview.h"
#include "qheaderview.h"
#include "qslider.h"
#include "qtimeline.h"
#include "qdialog.h"
#include "qcolordialog.h"
#include "qmainwindow.h"

#include "qlayout.h"
#include "qboxlayout.h"
#include "qformlayout.h"
#include "qgridlayout.h"
#include "qstackedlayout.h"

#include "qvalidator.h"
#include "qpainter.h"
#include "qevent.h"
#include "qaction.h"
#include "qmath.h"
#include "qdatetime.h"
#include "qdebug.h"
#include "qscreen.h"
#include "qdesktopwidget.h"
#include "qapplication.h"

// (2) 常量定义
#define DefaultFontSize 10
#define DefaultFontFamily ("Times New Roman")
#define PatternColorAlpha 125
#define SplitterHandleWidth 2
#define SplitterStyle ("QSplitter::handle { background-color: #F2A072 }")
#define ProgressBarStyle ("QProgressBar{border:2px solid grey;border-radius:5px;text-align:center;}"\
                                    "QProgressBar::chunk{background-color: #CD96CD;width: 10px;margin: 0.5px;}")
#define ColorButtonStyle QString("border:2px solid black;border-radius:20px;background-color:%1;")
#define LOG (qDebug()<<"["<<QTime::currentTime().toString("h:mm:ss:zzz")<<__FUNCTION__<<"] ")

#define UserScreenWidth (QApplication::desktop()->screenGeometry().width())
#define UserScreenHeight (QApplication::desktop()->screenGeometry().height())
#define ProgressBarMinHeight 40
#define ColorButtonSize 40

// (3) 导出定义
#if defined(COMPONENT_LIBRARY)
#  define COMPONENT_IMEXPORT Q_DECL_EXPORT
#else
#  define COMPONENT_IMEXPORT Q_DECL_IMPORT
#endif


#endif //EOSI_WIDGETS_H
