#include "gridpicturecanvas.h"
#include "utilities.h"
#include "tcpsocket.h"
#include <QtConcurrent/QtConcurrent>

void GridPictureCanvas::appendImage(const QString &path)
{
    if (isRunningState) {
        runningParams.imageCount++;
        runningParams.currentRow = runningParams.imageCount / cols; // 当前所在行索引
        if (runningParams.currentRow + 1 > imagemodel->rowCount()) { // 99张=9,100张=10,9还不需要添加新行
            // 假设imagecount=100=>current_row=10+1>10,就会添加新行,实际上正好100行进1时也不需要
            if (runningParams.imageCount != imagemodel->rowCount() * imagemodel->columnCount())
                imagemodel->appendRowItems();
        }
        auto rows = imagemodel->rowCount();
        auto r = (runningParams.imageCount - 1) / cols;
        auto c = (runningParams.imageCount - 1) % cols; // 注意要添加的位置应该是imageCount++之前的

        auto img_w = width() / cols;
        int img_h = 0;
        if (rows == 0)
            img_h = height() / rows;
        imagesize = img_w>=img_h?img_w:img_h; // 取较大者
        LOG<<rows<<img_w<<img_h<<imagesize;

        auto img = loadImageByImageReader(path,imagesize,imagesize);
        imagemodel->setData(r,c,img,ImageDataRole);
        imagemodel->setData(r,c,path,ImagePathRole); // 显示图像
        runningParams.images.append(img); // 存储图像

        updateImageSize();
    }
}

void GridPictureCanvas::showImages(const QString &dir)
{
    if (!isRunningState) {
        auto infos = listDirAllImages(dir);
        for(auto info: infos) {
            appendImage(info.second);
        }
    }
}

GridPictureCanvas::GridPictureCanvas(QWidget *parent) : QWidget(parent)
{
    isRunningState = false;
    imageview = new TableView;
    imagemodel = new StandardItemModel;
    imagedelegate = new ImageDelegate;
    imageview->setModel(imagemodel);
    imageview->horizontalHeader()->setVisible(false);
    imagemodel->setRowCount(0);
    imagemodel->setColumnCount(cols);
    imageview->setItemDelegate(imagedelegate);
    auto lay = new QHBoxLayout;
    lay->addWidget(imageview);
    lay->setMargin(0);
    lay->setSpacing(0);
    setLayout(lay);


    //QTimer::singleShot(5000,this,SLOT(test()));
    //QMetaObject::invokeMethod(this, "test", Qt::QueuedConnection);
    //QtConcurrent::run([=]{test();});
    //test();
}

void GridPictureCanvas::setState(bool isRunning)
{
    isRunningState = isRunning;
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
    auto img_w = width() / cols;
    auto img_h = 0;
    if (imagemodel->rowCount() != 0) // 小心零除错误
        img_h = height() / imagemodel->rowCount();
    imagesize = img_w>=img_h?img_w:img_h; // 取较大者
    imageview->updateCellSize(imagesize,imagesize);
}

void GridPictureCanvas::test()
{
//    int i = 0;
//    while (i++ < 1001) {
//        appendImage(CURRENT_PATH+"/images/cell.png");
//    }

    auto infos = listDirAllImages(CURRENT_PATH+"/images","*.png");
    for(auto info: infos) {
        appendImage(info.second);
    }
}

