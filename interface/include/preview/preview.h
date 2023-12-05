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
#include "cameramode.h"
#include "photocanvastriangle.h"
#include "wellpattern.h"
#include "viewpattern.h"
#include "summarydialog.h"
#include "videowidget.h"

//#define notusetoupcamera 0
//#define uselabelcanvas 0

#ifdef notusetoupcamera
#include "toupcam.h"
#else
#include "toupcamera.h"
#endif

#ifdef usetab
#include "expertool.h"
#endif

class INTERFACE_IMEXPORT Preview : public QWidget
{
    Q_OBJECT
public:
    explicit Preview(QWidget*parent= nullptr);
    void setAppInfo(int app);// 用于appselect传递当前app信息
    PreviewPatternInfo patternInfo() const; // WellPattern的数据
    PreviewToolInfo toolInfo() const;
#ifdef notusetoupcamera
    bool isCameraOpen() const;
    void openCamera();
    void closeCamera();
    bool haveCamera() const;
    unsigned cameraCount() const;
#endif
private:
    PreviewInfo previewinfo;
    CameraMode * cameramode;
    QTimer timer;
#ifdef uselabelcanvas
    LabelTriangle * livecanvas;
#else
    PhotoCanvasTriangle * livecanvas;
#endif
#ifdef usetab
    QTabWidget * tab;
    ExperTool * expertool;
#endif
    PhotoCanvas * photocanvas;
    ViewPattern * viewpattern;
    VideoWidget * videocanvas;
    QStackedWidget * stack;
    PreviewTool * previewtool;
    WellPattern * wellpattern;
    DockWidget * dock;
    QMainWindow * dockcanvas;
    GroupInfo * groupinfo;
    QScrollArea * scrollarea;
private:
#ifdef notusetoupcamera
    HToupcam toupcam = nullptr;
    ToupcamDeviceV2  camera ;
    QSharedPointer<uchar> imgdata = nullptr;
    QSize cameraResolution;
    static void __stdcall eventCallBack(unsigned nEvent, void* ctxEvent);
    void processCallback(unsigned nEvent);
    void captureLiveImage();
    void exposureEvent();
    QImage capture();
    void setByteOrder(int option);
    int byteOrder() const;
    void setRgbBit(int option);
    int rgbBit() const;
    void setExposureOption(int option);
    int exposureOption() const;
    void setExposure(unsigned exp);
    unsigned exposure() const;
    void setGain(ushort gain);
    ushort gain() const;
    void allocateImageBuffer();
#endif
private:
    void initObjects();
    void initAttributes();
    void initLayout();
    void initConnections();
private:
    void updateViewWindow(const QVariantMap& m);
    void updateSetGroupWindow(const QVariantMap& m);
private:
    void setSelectMode(int mode);
    void toggleManufacturer(int option);
    void toggleBrand(int option);
    void onObjectiveChanged(const QString& obj);
    void playVideo(const QString& path);
    void stopVideo();
    void pauseVideo();
    void takingPhoto();
    void toggleObjective(int objective,int objective_loc,int isPh);
    void adjustCamera(int exp,int gain,int br);
    void onAdjustCamera(const QString &f,const QVariant & d);
    void adjustLens(int option);
    void manualFocus(double val);
    void toggleChannel(int option);
    void previewViewByClickView(const QPointF& viewPoint);
    void previewViewByClickHole(const QPoint& holePoint);
    void exportExperConfig(const QString& path);
    void importExperConfig(const QString& path);
    void loadExper();
    void stopExper();
#ifndef notusetoupcamera
    void showCapturedImage(const QImage& image);
#endif
signals:
    void objectiveSettingChanged(const LocationObjectiveInfo& m);
#ifdef notusetoupcamera
    void evtCallback(unsigned nEvent);
#endif
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
