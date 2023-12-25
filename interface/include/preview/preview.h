/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-18 15:40:35
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-18 15:42:12
 * @FilePath: \EOS\interface\include\window\preview.h
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#ifndef EOSI_PREVIEW_H
#define EOSI_PREVIEW_H

#include "window.h"
#include "previewtool.h"
#include "canvasmode.h"
#include "photocanvastriangle.h"
#include "wellpattern.h"
#include "wellview.h"
#include "summarydialog.h"
#include "videowidget.h"
#include "slidepattern.h"
#include "wellbox.h"

//#define uselabelcanvas 0
#include "toupcamera.h"
#include "expertool.h"

class INTERFACE_IMEXPORT Preview : public QWidget
{
    Q_OBJECT
public:
    explicit Preview(QWidget*parent= nullptr);
    void setAppInfo(int app);// 用于appselect传递当前app信息
    WellPatternInfo patternInfo() const; // WellPattern的数据
    PreviewToolInfo toolInfo() const;
private:
    PreviewInfo previewinfo;
    CanvasMode * canvasmode;
    GroupInfo * groupinfo;
    QTimer timer;
#ifdef uselabelcanvas
    LabelTriangle * livecanvas;
#else
    PhotoCanvasTriangle * livecanvas;
#endif
    PhotoCanvas * photocanvas;
    VideoWidget * videocanvas;
    QStackedWidget * stackcanvas;

    WellBox * wellbox;

    WellPattern * wellpattern;
    SlidePattern * slidepattern;
    QStackedWidget * stackpattern;

    WellView * wellview;
    SlidePattern * slideview;
    QStackedWidget * stackview;

    QStackedWidget * stack_view_pattern;

    //DockWidget * dock;
    //QMainWindow * dockcanvas;

    QTabWidget * tab;
    ExperTool * expertool;
    PreviewTool * previewtool;
    QScrollArea * scrollarea;
private:
    void initObjects();
    void initAttributes();
    void initLayout();
    void initConnections();
private:
    void openWellViewWindow(const QVariantMap& m);
    void openSlideViewWindow();
    void openWellGroupWindow(const QVariantMap& m);
private:
    void setViewMode(int mode);
    void toggleStack(int option);
    void toggleBrand(int option);
    void onObjectiveChanged(const QString& obj);
    void playVideo(const QString& path);
    void stopVideo();
    void pauseVideo();
    void takingPhoto();
    void stitchSlide();
    void toggleObjective(int objective,int objective_loc,int isPh);
    void adjustCamera(int exp,int gain);
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
    //void importExperConfigV1(const QString& path);
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
#endif //EOSI_PREVIEW_H
