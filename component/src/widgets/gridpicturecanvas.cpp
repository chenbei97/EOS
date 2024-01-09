#include "gridpicturecanvas.h"

GridPictureCanvas::GridPictureCanvas(QWidget *parent) : QWidget(parent)
{
    imageview = new TableView;
    imagemodel = new StandardItemModel;
    imagedelegate = new ImageDelegate;
    imageview->setModel(imagemodel);
    imageview->horizontalHeader()->setVisible(false);
    imageview->verticalHeader()->setVisible(false);
    imagemodel->setRowCount(rows);
    imagemodel->setColumnCount(cols);
    imageview->setItemDelegate(imagedelegate);
    auto lay = new QHBoxLayout;
    lay->addWidget(imageview);
    lay->setMargin(0);
    lay->setSpacing(0);
    setLayout(lay);

    auto infos = listDirAllImages(CURRENT_PATH+"/images","*.png");
    for(int i = 0; i < infos.count(); ++i) {
        auto r = i / rows;
        auto c = i % rows;
        auto info = infos.at(i);
        //LOG<<r<<c;
        imagemodel->setData(imagemodel->index(r,c),info.first,ImageDataRole);
        imagemodel->setData(imagemodel->index(r,c),info.second,ImagePathRole);
    }
}

QImageVectorInfo GridPictureCanvas::listDirAllImages(const QString &dirpath, const QString &filter)
{
    QImageVectorInfo imageInfo;
    QDir dir(dirpath);
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList(QStringList()<<filter);
    for(auto info: list) {
        auto imgpath = info.filePath();
        imageInfo.append(qMakePair(QImage(imgpath),imgpath));
    }
    return imageInfo;
}

void GridPictureCanvas::resizeEvent(QResizeEvent *event)
{
    updateImageSize();
    event->accept();
}

void GridPictureCanvas::updateImageSize()
{
    auto img_w = width() / rows;
    auto img_h = height() / cols;
    auto sz = img_w>=img_h?img_w:img_h; // 取较大者
    imageview->updateCellSize(sz,sz);
}



