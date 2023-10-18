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
#define DefaultFontSize 12
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

#define ScreenWidth 2560
#ifdef ScreenWidth
    #define MainWindowWidth 2000
    #define MainWindowHeight 1200

    #define PainterFontSize 20
    #define PainterFont (QFont(DefaultFontFamily,PainterFontSize,QFont::Bold))
    #define PainterMetric (QFontMetrics(PainterFont))
    #define PainterPixelHeight (PainterMetric.height()) //指定font下文字像素的高度

    #define NavigBarWidth 200 // 导航条的宽度
    #define NavigBarHeight 100 // 导航条的高度
    #define NavigBarGap 2 // 导航按钮之间的间隔
    #define NavigBarButtonCount (MainWindowWidth+NavigBarGap)/(NavigBarWidth+NavigBarGap) // x * bw + (x-1)gap = w换算
    #define NavigBarFontHeight (NavigBarHeight-PainterPixelHeight)  // 文字在按钮内部,按钮高度和文字高度的差值
    #define NavigBarMoveDistance (NavigBarGap+NavigBarWidth) // 下一个按钮相对于移动距离=gap+width

    #define AppSelectButtonCountPerCol 4 // 每行4个按钮
    #define AppSelectButtonCountPerRow 2 // 共2行
    #define AppSelectButtonWidth 250 // 按钮的宽度
    #define AppSelectButtonHeight 155 // 按钮高度
    #define AppSelectButtonHorGap 150 // 按钮之间水平间隔
    #define AppSelectButtonVerGap 150 // 按钮垂直水平间隔
    #define AppSelectHorGap (MainWindowWidth-AppSelectButtonCountPerCol*AppSelectButtonWidth- \
        (AppSelectButtonCountPerCol-1)*AppSelectButtonHorGap)/2 // 到窗口左右两侧的间隔距离=(窗口距离-4个按钮宽度-3个gap)/2
    #define AppSelectVerGap (MainWindowHeight-AppSelectButtonCountPerRow*AppSelectButtonHeight- \
        (AppSelectButtonCountPerRow-1)*AppSelectButtonVerGap)/2-NavigBarHeight // 到窗口上下两侧的间隔距离=(窗口距离-2个按钮高度-1个gap)/2-导航条高度
    #define AppSelectButtonFontHeight (AppSelectButtonHeight-PainterPixelHeight)*4/5 // 按钮高度-文字高度 文字有些偏下需要适当微调
    #define AppSelectMoveHorDistance (AppSelectButtonHorGap+AppSelectButtonWidth) // 水平方向下一个按钮相对移动距离=horbtngap+btnwidth
    #define AppSelectMoveVerDistance (AppSelectButtonVerGap+AppSelectButtonHeight) // 垂直方向下一个按钮相对移动距离=verbtngap+btnheight
    #define AppSelectButtonRoundRadius 25 // 圆角半径
#endif

// (3) 导出定义
#if defined(COMPONENT_LIBRARY)
#  define COMPONENT_IMEXPORT Q_DECL_EXPORT
#else
#  define COMPONENT_IMEXPORT Q_DECL_IMPORT
#endif


#endif //EOSI_WIDGETS_H
