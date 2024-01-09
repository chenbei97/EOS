/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 10:51:33
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 10:34:35
 * @FilePath: \EOS\interface\include\mainwindow\window.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_WINDOW_H
#define EOSI_WINDOW_H

// (1)头文件
#include "public.h"
#include "alias.h"
#include "qradiobutton.h"
#include "qbuttongroup.h"
#include "qpropertyanimation.h"
#include "qscrollarea.h"
#include "qdatetimeedit.h"
#include "qprocess.h"
#include <cmath>
using std::modf;

// (2) 导出定义
#if defined(INTERFACE_LIBRARY)
#  define INTERFACE_IMEXPORT Q_DECL_EXPORT
#else
#  define INTERFACE_IMEXPORT Q_DECL_IMPORT
#endif

// (3) 常量
// navigbar
//#define use_opencv2 0 // 要和interface-cmakelist的USE_OPNECV一致
#define NavigPainterFontSize 20
#define NavigPainterFont (QFont(DefaultFontFamily,NavigPainterFontSize,QFont::Bold))
#define NavigPainterMetric (QFontMetrics(NavigPainterFont))
#define NavigPainterPixelHeight (NavigPainterMetric.height()) //指定font下文字像素的高度
#define NavigBarColor QColor("#767C85")

#define PointToIDCoefficient 1000 // kx1+y1=kx2+y2=> k=(y2-y1)/(x1-x2),k取什么值?找不到这样的2个坐标能等于k,由于wellsize最大384,实际上k>384就能保证id唯一了

#define ScreenWidth 2560

#ifdef ScreenWidth
    #define MainWindowWidth 2400
    #define MainWindowHeight 1300

    #define NavigBarWidth 300 // 导航条的宽度
    #define NavigBarHeight 100 // 导航条的高度
    #define NavigBarGap 2 // 导航按钮之间的间隔
    //#define NavigBarButtonCount (MainWindowWidth+NavigBarGap)/(NavigBarWidth+NavigBarGap) // x * bw + (x-1)gap = w换算
    #define NavigBarFontHeight (NavigBarHeight-NavigPainterPixelHeight)  // 文字在按钮内部,按钮高度和文字高度的差值
    #define NavigBarMoveDistance (NavigBarGap+NavigBarWidth) // 下一个按钮相对于移动距离=gap+width

    #define AppSelectButtonCountPerCol 4 // 每行4个按钮
    #define AppSelectButtonCountPerRow 2 // 共2行
    #define AppSelectButtonWidth 350 // 按钮的宽度
    #define AppSelectButtonHeight 185 // 按钮高度
    #define AppSelectButtonHorGap 150 // 按钮之间水平间隔
    #define AppSelectButtonVerGap 150 // 按钮垂直水平间隔
    #define AppSelectHorGap (MainWindowWidth-AppSelectButtonCountPerCol*AppSelectButtonWidth- \
            (AppSelectButtonCountPerCol-1)*AppSelectButtonHorGap)/2 // 到窗口左右两侧的间隔距离=(窗口距离-4个按钮宽度-3个gap)/2
    #define AppSelectVerGap (MainWindowHeight-AppSelectButtonCountPerRow*AppSelectButtonHeight- \
            (AppSelectButtonCountPerRow-1)*AppSelectButtonVerGap)/2-NavigBarHeight // 到窗口上下两侧的间隔距离=(窗口距离-2个按钮高度-1个gap)/2-导航条高度
    #define AppSelectButtonFontHeight (AppSelectButtonHeight-NavigPainterPixelHeight)*4/5 // 按钮高度-文字高度 文字有些偏下需要适当微调
    #define AppSelectMoveHorDistance (AppSelectButtonHorGap+AppSelectButtonWidth) // 水平方向下一个按钮相对移动距离=horbtngap+btnwidth
    #define AppSelectMoveVerDistance (AppSelectButtonVerGap+AppSelectButtonHeight) // 垂直方向下一个按钮相对移动距离=verbtngap+btnheight
    #define AppSelectButtonRoundRadius 25 // 圆角半径

    #define FocusToolStepSpinMaxWidth 100 // 设置焦距中间的step步进spibox设置的宽度不能太宽
    #define CameraBoxLeftBoxMinWidth 350 // 让滑动条看起来不要太窄
    #define CameraModeHeight 40 // 切换相机模式部件的高度
    #define ObjectiveSettingLeftSpacing 150

#endif



#endif //EOSI_WINDOW_H
