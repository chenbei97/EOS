/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:40:35
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:42:12
 * @FilePath: \EOS\interface\include\window\preview.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOS_PREVIEW_H
#define EOS_PREVIEW_H

#include "window.h"
#include "canvasmode.h"
#include "photocanvastriangle.h"
#include "wellpattern.h"
#include "wellview.h"
#include "summarydialog.h"
#include "videowidget.h"
#include "slidepattern.h"
#include "wellbox.h"
#include "toupcamera.h"
#include "historybox.h"
#include "objectivebox.h"
#include "objectivebox.h"
#include "viewmode.h"
#include "channelbox.h"
#include "camerabox.h"
#include "recordbox.h"
#include "timebox.h"
#include "zstackbox.h"
#include "savebox.h"
#include "focusbox.h"

//#define uselabelcanvas 1
//#define usegraphicscanvas 1

class INTERFACE_IMEXPORT Preview : public QWidget
{
    Q_OBJECT
public:
    explicit Preview(QWidget*parent= nullptr);
    void setAppInfo(int app);// 用于appselect传递当前app信息
    WellPatternInfo patternInfo() const; // WellPattern的数据
    void askConnectState();
    void askActivateCode();
private:
    PreviewInfo previewinfo;
    CanvasMode * canvasmode;
    GroupInfo * groupinfo;
    QTimer timer;
#ifdef uselabelcanvas
    LabelTriangle * livecanvas;
#elif defined(usegraphicscanvas)
    PhotoGraphics * livecanvas;
#else
    PhotoCanvasTriangle * livecanvas;
#endif
    PhotoCanvas * photocanvas;
    VideoWidget * videocanvas;
    QStackedWidget * stackcanvas;
    GroupBox * canvasbox;

    HistoryBox * historybox;
    WellBox * wellbox;
    ObjectiveBox * objectivebox;
    ViewModeBox * viewModeBox;
    ChannelBox * channelbox;
    CameraBox * camerabox;
    RecordBox * recordbox;
    TimeBox * timebox;
    ZStackBox * zstackbox;
    SaveBox * savebox;
    FocusBox * focusbox;
    DiskSizeTable * diskTable;

    GroupBox * stackbox;
    WellPattern * wellpattern;
    SlidePattern * slidepattern;
    QStackedWidget * stackpattern;
    WellView * wellview;
    QStackedWidget * stack_view_pattern;

    QTabWidget * tab;
    QScrollArea * scrollarea_preview;
    QScrollArea * scrollarea_experiment;
private:
    void initObjects();
    void initAttributes();
    void initLayout();
    void initConnections();
private:
    void openWellViewWindow(const QVariantMap& m);
    void openWellGroupWindow(const QVariantMap& m);
private:
    void setViewMode(int mode);
    void toggleStack(int option);
    void toggleBrand(int option);
    void toggleObjective(const QString& obj);
    void playVideo(const QString& path);
    void stopVideo();
    void pauseVideo();
    void takingPhoto();
    void stitchSlide();
    void adjustObjective(int objective,int objective_loc,int isPh);
    void adjustBright(int br);
    void adjustLens(int option);
    void adjustFocus(double val);
    void autoFocus();
    void toggleChannel(int option);
    void closeChannel(int option);
    void previewViewEvent(const QPointF& viewPoint);
    void previewHoleEvent(const QPoint& holePoint);
    void previewSlideEvent(const QPointF& point);
    void exportExperConfig(const QString& path);
    void importExperConfig(const QString& path);
    void loadExper();
    void stopExper();
    void parseResult(const QString &f,const QVariant & d);
    void showCapturedImage(const QImage& image);
signals:
    void objectiveSettingChanged(const LocationObjectiveInfo& m);
};

/*每张图分辨率为2048*2048,大小暂时不确定,2048*2048/(1024*1024)=4M,imgSize暂时取个估计值4M*/
const double PerImageSpace = 4.0; // MB
static double calculateExperSpaceMB(int numOfViews,int numOfChannel,double perImgSpace = PerImageSpace)
{ // 占用空间 = 所有视野数 x 通道数 x 每张图的大小（4mb暂定）
    return numOfViews * numOfChannel * perImgSpace; // MB
}

static double calculateExperSpaceGB(int numOfViews,int numOfChannel,double perImgSpace = PerImageSpace)
{ // 占用空间 = 所有视野数 x 通道数 x 每张图的大小
    return numOfViews * numOfChannel * perImgSpace / 1024.0; // GB
}
#endif //EOS_PREVIEW_H
