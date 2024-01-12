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

typedef QPoint HoleCoordinate;
struct ViewCoordinate
{
    double x = 0.0;
    double y = 0.0;
    HoleCoordinate hole;
};

struct ImageInfo
{
    QString path;
    QDateTime stamp;
    QString channel;
    ViewCoordinate view;

    void setHoleCoordinate(const HoleCoordinate& hole) {
        view.hole = hole;
    }

    HoleCoordinate getHoleCoordinate() const {
        return view.hole;
    }

    void setHoleCoordinate(int x,int y) {
        view.hole = HoleCoordinate(x,y);
    }
    void setViewCoordinate(double x,double y) {
        view.x = x;
        view.y = y;
    }

    QPointF getViewCoordinate() const {
        return QPointF(view.x,view.y);
    }
};
typedef QVector<ImageInfo> ImageInfoVector;

struct PlateImageInfo
{
    ImageInfoVector images;
    bool isRunning = false;
    QDateTime experTime;
    QString objective; // 4xBR
    QSize plateSize;
    QString experName;
    // 其它实验信息参数等

    void clear() {
        images.clear();
        isRunning = false;
        experName.clear();
        objective.clear();
        plateSize = QSize();
        experTime = QDateTime();
    }

    // 获取使用的所有孔坐标
    QPointVector holeCoordinates() const {
        QPointVector vec;
        for(auto image: images) {
            vec.append(image.view.hole);
        }
        return vec;
    }

    // 获取指定孔的所有不重复视野坐标,因为可能坐标相同只是时间戳不同
    QPointFVector viewCoordinates(int x, int y) const {
        QPointFVector vec;
        for(auto image: images) {
            if (image.view.hole == QPoint(x,y)) {
                if (!vec.contains(QPointF(image.view.x,image.view.y)))
                    vec.append(QPointF(image.view.x,image.view.y));
            }
        }
        return vec;
    }

    // 获取指定孔所有信息
    ImageInfoVector holeImages(int hole_x,int hole_y) const
    {
        ImageInfoVector vec;
        for(auto image: images) {
            if (image.view.hole.x() == hole_x && image.view.hole.y() == hole_y) {
                vec.append(image);
            }
        }
        return vec;
    }

    // 获取指定孔内指定点的信息
    ImageInfoVector viewImages(int hole_x,int hole_y,double view_x,double view_y) const {
        ImageInfoVector vec;
        for(auto image: images) {
            if (image.view.hole.x() == hole_x && image.view.hole.y() == hole_y) { // 指定孔
                //qDebug()<<(QString::number(0.000000000011) == QString::number(0.0000000000111));
                if (qAbs(view_x - image.view.x) < 1e-6
                    && qAbs(view_y - image.view.y) < 1e-6) { // 指定视野
                    vec.append(image);
                }
            }
        }
        return vec;
    }

    // 获取指定孔内指定点且是某个通道的图像,注意不区分时间戳,所以视野坐标可能是相同的
    ImageInfoVector viewChannelImages(int hole_x,int hole_y,double view_x,double view_y, const QString& channel) {
        ImageInfoVector vec;
        for(auto image: images) {
            if (image.view.hole.x() == hole_x && image.view.hole.y() == hole_y && channel == image.channel) { // 指定孔和通道
//                LOG<< image.getViewCoordinate() << QPointF(view_x,view_y)
//                    <<(qAbs(view_x - image.view.x) < 1e-6)<<(qAbs(view_y - image.view.y) < 1e-6);
                if (qAbs(view_x - image.view.x) < 1e-6
                    && qAbs(view_y - image.view.y) < 1e-6 ) { // 指定视野和通道
                    vec.append(image);
                }
            }
        }
        return vec;
    }

    ImageInfoVector viewChannelImages(const HoleCoordinate& hole,const QPointF& view, const QString& channel) {
        return viewChannelImages(hole.x(),hole.y(),view.x(),view.y(),channel);
    }

    // 获取指定孔的指定视野的照片都涉及哪些通道
    QStringList getViewChannels(int hole_x,int hole_y,double view_x,double view_y) const {
        QStringList vec;
        for(auto image: images) {
            if (image.view.hole.x() == hole_x && image.view.hole.y() == hole_y
                && qAbs(view_x - image.view.x) < 1e-6
                && qAbs(view_y - image.view.y) < 1e-6) { // 指定孔和视野
                if (!vec.contains(image.channel))
                    vec.append(image.channel);
            }
        }
        return vec;
    }
};

struct DataPatternHoleInfo
{
    bool isSelected = false;
    QPoint hole;
    PlateImageInfo info;
};
typedef QVector<DataPatternHoleInfo> DataPatternHoleInfoVector;
typedef QVector<DataPatternHoleInfoVector> DataPatternHoleInfo2DVector;


#endif //EOSI_WINDOW_H
