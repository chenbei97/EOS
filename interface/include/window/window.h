/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 10:51:33
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 10:54:28
 * @FilePath: \EOS\interface\include\window\window.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_WINDOW_H
#define EOSI_WINDOW_H

// (1)头文件
#include "public.h"
#include "alias.h"

// (2) 导出定义
#if defined(INTERFACE_LIBRARY)
#  define INTERFACE_IMEXPORT Q_DECL_EXPORT
#else
#  define INTERFACE_IMEXPORT Q_DECL_IMPORT
#endif

// (3) 常量
#define NavigBarColor QColor("#767C85")
#define AppSelectColor QColor("#2F7CAD")

static const QFieldList NavigBarFields = {
    "Logo","Main","Preview","Protocol","Data","Analysis"
};
#define NavigBarFieldsCount  (NavigBarFields.count())

static const QFieldList AppFields = {
    QObject::tr("增殖"),QObject::tr("划痕"),QObject::tr("侵袭"),QObject::tr("转染"),
    QObject::tr("形态学"),QObject::tr("类器官"),QObject::tr("药效和毒理"),QObject::tr("自定义")
};

static const QFieldList WellsizeFields = {
        "6","24","96","384",
};
static const char* WellsizeField = "wellsize";
static const char* WellsizeFieldLabel = "wellsize: ";

static const QFieldList ObjectiveFields = {
        "4x","10x","20x","40x",
};
static const char* ObjectiveField = "objective";
static const char* ObjectiveFieldLabel = "objective: ";

static const QFieldList ChannelFields = {
        "PH","GFP","RFP","DAPI"
};
static const char* PHField = "PH";
static const char* GFPField = "GFP";
static const char* RFPField = "RFP";
static const char* DAPIField = "DAPI";
static const char* ChannelField = "channel";
static const char* IsCheckedField = "isChecked";
static const char* ChannelFieldLabel = "channel: ";

static const QFieldList ImageFormatFields = {
        "jpg","png","ico","bmp"
};

static const QFieldList VideoFormatFields = {
        "avi","wmv"
};

static const QFieldList BrandFields = {
        "brand1","brand2","brand3"
};
static const char* BrandField = "brand";
static const char* BrandFieldLabel = "brand: ";


static const QFieldList DishFields = {
        "dish1","dish2","dish3"
};
static const char* DishField = "dish";
static const char* DishFieldLabel = "dish: ";

static const QFieldList FlaskFields = {
        "flask1","flask2","flask3"
};
static const char* FlaskField = "flask";
static const char* FlaskFieldLabel = "flask: ";


static const QFieldList SlideFields = {
        "slide1","slide2","slide3"
};
static const char* SlideField = "slide";
static const char* SlideFieldLabel = "slide: ";

static const char* ExposureField = "exposure";
static const char* ExposureFieldLabel = "exposure: ";
static const char* ExposureFieldUnit = "ms";
#define ExposureLowLimit 0
#define ExposureUpLimit 15000
static const char* GainField = "gain";
static const char* GainFieldLabel = "gain: ";
#define GainLowLimit 100
#define GainUpLimit 5000
static const char* GainFieldUnit = "%";
static const char* BrightField = "bright";
static const char* BrightFieldLabel = "bright: ";
#define BrightLowLimit 0
#define BrightUpLimit 100

static const QList<QFieldList> SocketNeedMapFields{
        BrandFields,VideoFormatFields,ImageFormatFields,
        ChannelFields,ObjectiveFields,WellsizeFields,
        DishFields,FlaskFields,SlideFields
};

static QString getIndexFromFields (QCString field)
{ // 用于把这些字段统一映射为0,1,2,3方便下位机读取
    // 例如4x,jpg,bright,avi都应该映射为0
    QString idx = "";
            foreach(auto fields, SocketNeedMapFields) { // 仅限于从这些字段查找
            auto index = fields.indexOf(field);
            if (index>=0){
                idx = QString::number(index);
                break; // 找到
            }
        }
    return idx;
}

#define ScreenWidth 2560

#ifdef ScreenWidth
    #define MainWindowWidth 2400
    #define MainWindowHeight 1200

    #define PainterFontSize 20
    #define PainterFont (QFont(DefaultFontFamily,PainterFontSize,QFont::Bold))
    #define PainterMetric (QFontMetrics(PainterFont))
    #define PainterPixelHeight (PainterMetric.height()) //指定font下文字像素的高度

    #define NavigBarWidth 200 // 导航条的宽度
    #define NavigBarHeight 100 // 导航条的高度
    #define NavigBarGap 2 // 导航按钮之间的间隔
    //#define NavigBarButtonCount (MainWindowWidth+NavigBarGap)/(NavigBarWidth+NavigBarGap) // x * bw + (x-1)gap = w换算
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

    #define WellBoxGridLayHorSpacing 30
    #define PreviewToolBarMaxWidth 800
#endif



#endif //EOSI_WINDOW_H
