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

// (1) 导出定义
#if defined(COMPONENT_LIBRARY)
#  define COMPONENT_IMEXPORT Q_DECL_EXPORT
#else
#  define COMPONENT_IMEXPORT Q_DECL_IMPORT
#endif

// (2) 头文件
#include "qcombobox.h"
#include "qlineedit.h"
#include "qtextedit.h"
#include "qdockwidget.h"
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
#include "qstackedwidget.h"

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

// (3) 常量定义
#define CURRENT_PATH (QDir::currentPath())
#define CURRENT_THREAD (QThread::currentThread())
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
#define GroupInfoSize QSize(400,250)
#define RoundButtonSize 20
#define InscribedRectDefaultRows 3
#define InscribedRectDefaultCols 3
#define InscribedRectSize 40
#define InscribedRectColor QColor("#00A2E8")
#define TriangleLength 40
#define TriangleGap 10
#define TriangleMinSize 150
static const char* GroupTypeField = "grouptype";
static const char* GroupNameField = "groupname";
static const char* GroupColorField = "groupcolor";
static const char* GroupPointsField = "grouppoints";
static const char* GroupMedicineField = "groupmedicine";
static const char* GroupDoseField = "groupdose";
static const char* GroupDoseUnitField = "groupdoseunit";
static const char* AllGroupsField = "allgroups";
static const char* CoordinateField = "coordinate";
static const char* ViewSizeField = "viewsize";
static const char* ViewPointsField = "viewpoints";
static const QStringList GroupDoseUnitFields =  {
        "mg/mL","mg/L","g/mL","g/L",
};
static const QStringList GroupTypeFields = {
        QObject::tr("控制组"),QObject::tr("对照组"),QObject::tr("对照3"),
};

#endif //EOSI_WIDGETS_H
