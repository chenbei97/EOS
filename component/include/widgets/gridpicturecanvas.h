

#ifndef EOS_GRIDPICTURECANVAS_H
#define EOS_GRIDPICTURECANVAS_H

#include "widgets.h"
#include "tableview.h"
#include "standarditemmodel.h"
#include "imagedelegate.h"

struct RunningParams{
    long imageCount = 0;
    long currentRow = 0;
    QList<QImage> images;
};

struct CompleteParams {
    long imageCount = 0;
};

class COMPONENT_IMEXPORT GridPictureCanvas: public QWidget
{
    Q_OBJECT
public:
    explicit GridPictureCanvas(QWidget*parent= nullptr);
    void resizeEvent(QResizeEvent*event) override;
    void appendImage(const QString& path);
    void showImages(const QString& dir);
    void setState(bool isRunning);
    QImageVectorInfo listDirAllImages(const QString& dirpath,const QString& filter = JPGSuffixFilter);
    Q_INVOKABLE void test();
protected:
    StandardItemModel * imagemodel;
    TableView * imageview;
    ImageDelegate * imagedelegate;
    const int cols = 10;
    RunningParams runningParams;
    CompleteParams completeParams;
    qreal imagesize = 0.0;
    bool isRunningState = false;
protected:
    void updateImageSize();
};

#endif //EOS_GRIDPICTURECANVAS_H
