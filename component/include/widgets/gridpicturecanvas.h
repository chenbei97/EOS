/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-08 15:48:31
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-12 17:22:11
 * @FilePath: \EOS\component\include\widgets\gridpicturecanvas.h
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */

#ifndef EOS_GRIDPICTURECANVAS_H
#define EOS_GRIDPICTURECANVAS_H

#include "widgets.h"
#include "tableview.h"
#include "standarditemmodel.h"
#include "imagedelegate.h"
#include "waitmessagebox.h"

struct GridPictureParams{
    long imageCount = 0;
    long currentRow = 0;
    QImageVector images;
    long lastLoadImageCount = 0;
    void clear() {
        imageCount = 0;
        currentRow = 0;
        images.clear();
    }
};
Q_DECLARE_METATYPE(GridPictureParams);

#define WaitLoadImageMsg "please wait for loading image..."

class COMPONENT_IMEXPORT GridPictureCanvas: public QWidget
{
    Q_OBJECT
public:
    explicit GridPictureCanvas(QWidget*parent= nullptr);
    void resizeEvent(QResizeEvent*event) override;
    void appendImage(const QString& path);
    void showImages(const QString& dir,const QString& filter = JPGSuffixFilter);
    void setState(bool isRunning);
    bool isRunning() const;
    QImageVectorInfo listDirAllImages(const QString& dirpath,const QString& filter = JPGSuffixFilter);
    Q_INVOKABLE void test();
protected:
    StandardItemModel * imagemodel;
    TableView * imageview;
    ImageDelegate * imagedelegate;
    const int cols = 10;
    GridPictureParams * runningParams;
    GridPictureParams * completeParams;
    qreal imagesize = 0.0;
    bool isRunningState = false;
    WaitMessageBox * waitDlg;
protected:
    void updateImageSize();
    void clickImage(const QModelIndex& index);
signals:
    void imageClicked(const QImage&img,const QString& path);
private:signals:
    void readyRead();
    void finished();

};

#endif //EOS_GRIDPICTURECANVAS_H
