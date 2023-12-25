/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-16 14:17:22
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 16:23:37
 * @FilePath: \EOS\component\include\widgets\widgets.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOS_WIDGETS_H
#define EOS_WIDGETS_H

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
#include "qmessagebox.h"
#include "qradiobutton.h"
#include "qbuttongroup.h"

#include "qfiledialog.h"
#include "qfile.h"
#include "qfileinfo.h"

#include "qlayout.h"
#include "qboxlayout.h"
#include "qformlayout.h"
#include "qgridlayout.h"
#include "qstackedlayout.h"

#include "qmediaplayer.h"
#include "qvideowidget.h"
#include "qmediaplaylist.h"

#include "qvalidator.h"
#include "qpainter.h"
#include "qevent.h"
#include "qaction.h"
#include "qelapsedtimer.h"
#include "qtimer.h"
#include "qmenu.h"
#include "qmath.h"
#include "qpropertyanimation.h"
#include "qgraphicseffect.h"

#include "fields.h"
#include "constant.h"

// (3) 常量定义
#define SplitterHandleWidth 2
#define SplitterStyle ("QSplitter::handle { background-color: #F2A072 }")
#define ProgressBarStyle ("QProgressBar{border:2px solid grey;border-radius:5px;text-align:center;}"\
                                    "QProgressBar::chunk{background-color: #CD96CD;width: 10px;margin: 0.5px;}")
#define ColorButtonStyle QString("width:40px;height:40px;border:2px solid #3282F6;border-radius:20px;background-color:%1;")
#define ProgressBarMinHeight 40
#define ColorButtonSize 40
#define RoundButtonSize 20
#define InscribedRectDefaultRows 3
#define InscribedRectDefaultCols 3
#define InscribedRectSize 40
#define InscribedRectColor QColor("#00A2E8")
#define TriangleLength 30 // 绘制三角形的长度
#define TriangleGap 10 // 三角到边缘的距离
#define TriangleMinHeight 100
#define TriangleMinWidth (TriangleMinHeight/0.618)
#define PhotoCanvasTriangleLength 100
#define PhotoCanvasTriangleGap 10
#define ViewTriangleLength 30
#define PictureDescriptionMaxHeight 30
#define ChannelPictureLabelMinHeight 100
#define ChannelPictureLabelMaxWidth 150
#define ChannelPictureLabelDefaultWidth 400
#endif //EOS_WIDGETS_H
