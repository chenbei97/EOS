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
#include "previewphotocanvas.h"
#include "wellpattern.h"
#include "viewpattern.h"

class INTERFACE_IMEXPORT Preview : public QWidget
{
    Q_OBJECT
public:
    explicit Preview(QWidget*parent= nullptr);
    void setAppInfo(int app);// 用于appselect传递当前app信息
private:
    PreviewInfo previewinfo;
    CameraMode * cameramode;
    QWidget * livecanvas;
    PreviewPhotoCanvas * photocanvas;
    ViewPattern * viewpattern;
    QStackedWidget * stack;
    PreviewTool * toolbar;
    WellPattern * pattern;
    DockWidget * dock;
    QMainWindow * dockcanvas;
    GroupInfo * groupinfo;
    QScrollArea * scrollarea;
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
    void previewView(const QPoint& viewpoint);
    void saveExperConfig(const QString& path);
signals:
    void objectiveSettingChanged(const LocationObjectiveInfo& m);
};
#endif //EOSI_PREVIEW_H
