

#ifndef EOS_GRIDPICTURECANVAS_H
#define EOS_GRIDPICTURECANVAS_H

#include "widgets.h"
#include "tableview.h"
#include "standarditemmodel.h"
#include "imagedelegate.h"

class COMPONENT_IMEXPORT GridPictureCanvas: public QWidget
{
    Q_OBJECT
public:
    explicit GridPictureCanvas(QWidget*parent= nullptr);
    void resizeEvent(QResizeEvent*event) override;
    QImageVectorInfo listDirAllImages(const QString& dirpath,const QString& filter = JPGSuffixFilter);
//    void setImage(const QImage&img);
//    void setPixmap(const QPixmap& pix);
private:
    QImage mimage; // 单图模式使用
    StandardItemModel * imagemodel;
    TableView * imageview;
    ImageDelegate * imagedelegate;
    const int rows = 10;
    const int cols = 10;
private:
    void updateImageSize();
};

#endif //EOS_GRIDPICTURECANVAS_H
