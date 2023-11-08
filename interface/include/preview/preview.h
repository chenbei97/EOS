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
#include "photocanvas.h"
#include "wellpattern.h"
#include "viewpattern.h"
#include "summarydialog.h"

#ifdef notusetoupcamera
#include "toupcam.h"
#else
#include "toupcamera.h"
#endif

class INTERFACE_IMEXPORT Preview : public QWidget
{
    Q_OBJECT
public:
    explicit Preview(QWidget*parent= nullptr);
    void setAppInfo(int app);// 用于appselect传递当前app信息
#ifdef notusetoupcamera
    void openCamera();
    void closeCamera();
#endif
private:
    PreviewInfo previewinfo;
    CameraMode * cameramode;
    PhotoCanvas * livecanvas;
    //Label * livecanvas;
    PhotoCanvas * photocanvas;
    ViewPattern * viewpattern;
    QStackedWidget * stack;
    PreviewTool * toolbar;
    WellPattern * pattern;
    DockWidget * dock;
    QMainWindow * dockcanvas;
    GroupInfo * groupinfo;
    QScrollArea * scrollarea;
private:
#ifdef notusetoupcamera
    HToupcam toupcam = nullptr;
    ToupcamDeviceV2 * camera = nullptr;
    QSharedPointer<uchar> imgdata = nullptr;
    QSize cameraResolution;
    static void __stdcall eventCallBack(unsigned nEvent, void* ctxEvent);
    void processCallback(unsigned nEvent);
    void captureLiveImage();
    QImage capture();
    QSize resolution() const;
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
    void updateViewPatternUi();
private:
    void onManufacturerChanged(int option);
    void onWellbrandChanged(int option);
    void onObjectiveChanged(const QString& obj);
    void takingPhoto();
    void adjustCamera(int exp,int gain,int br);
    void onAdjustCamera(const QString &f,const QVariant & d);
    void adjustLens(int option);
    void toggleChannel(int option);
    void previewViewByClickView(const QPoint& viewpoint);
    void previewViewByClickHole(const QPoint& holepoint);
    void exportExperConfig(const QString& path);
    void importExperConfig(const QString& path);
    void loadExper();
#ifndef notusetoupcamera
    void showCapturedImage(const QImage& image);
#endif
signals:
    void objectiveSettingChanged(const LocationObjectiveInfo& m);
#ifdef notusetoupcamera
    void evtCallback(unsigned nEvent);
#endif
};
#endif //EOSI_PREVIEW_H
