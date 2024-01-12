/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2024-01-08 15:51:26
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2024-01-12 17:22:05
 * @FilePath: \EOS\component\src\widgets\gridpicturecanvas.cpp
 * @Copyright (c) 2024 by ${chenbei}, All Rights Reserved. 
 */
#include "gridpicturecanvas.h"
#include "utilities.h"
#include "tcpsocket.h"
#include <QtConcurrent/QtConcurrent>

void GridPictureCanvas::appendImage(const QString &path)
{
    GridPictureParams *pointer = nullptr;
    if (isRunningState) {
        pointer = runningParams;
    } else pointer = completeParams;

    pointer->imageCount++;
    pointer->currentRow = pointer->imageCount / cols; // 当前所在行索引
    if (pointer->currentRow + 1 > imagemodel->rowCount()) { // 99张=9,100张=10,9还不需要添加新行
        // 假设imagecount=100=>current_row=10+1>10,就会添加新行,实际上正好100行进1时也不需要
        if (pointer->imageCount != imagemodel->rowCount() * imagemodel->columnCount())
            imagemodel->appendRowItems();
    }
    auto rows = imagemodel->rowCount();
    auto r = (pointer->imageCount - 1) / cols;
    auto c = (pointer->imageCount - 1) % cols; // 注意要添加的位置应该是imageCount++之前的

    auto img_w = width() / cols;
    int img_h = 0;
    if (rows == 0)
        img_h = height() / rows;
    imagesize = img_w >= img_h ? img_w : img_h; // 取较大者

    auto img = loadImageByImageReader(path, imagesize, imagesize);
    if (img.isNull()) {LOG<<"load img is null!"<<path;return;};
    //LOG<<"imageCount:"<<pointer->imageCount<<"currentRow:"<<pointer->currentRow<<"loc:"<<r<<c;
    imagemodel->setData(r, c, img, ImageDataRole);
    imagemodel->setData(r, c, path, ImagePathRole); // 显示图像
    pointer->images.append(img); // 存储图像

    updateImageSize();
}

void GridPictureCanvas::showImages(const QString &dir,const QString& filter)
{
    if (!isRunningState) {
        QDir qdir(dir);
        qdir.setFilter(QDir::Files);// 少1次循环,把listDirAllImages的代码直接拿到这里
        QFileInfoList list = qdir.entryInfoList(QStringList()<<filter);

        if (completeParams->lastLoadImageCount == list.count())
            return; // 如果上次加载的图片个数没变,说明已经加载过无需再加载
        emit readyRead();
        //imageview->blockSignals(true);
        imagemodel->clear();
        imagemodel->setModelSize(0,cols);
        completeParams->clear();
        QtConcurrent::run([=]{
            for(auto info: list) {
                auto imgpath = info.filePath();
                appendImage(imgpath);
            }
            //waitDlg->accept(); // 子线程不能操作GUI
            emit finished(); // 发信号的方式
        });
        completeParams->lastLoadImageCount = list.count();
        //emit finished();
        //imageview->blockSignals(false);
        updateImageSize();
    }
}

void GridPictureCanvas::clickImage(const QModelIndex &index)
{
    if (index.isValid()) {
        auto image = imagemodel->data(index,ImageDataRole).value<QImage>();
        auto path = imagemodel->data(index,ImagePathRole).toString();
        //LOG<<index<<path;
        emit imageClicked(image,path);
    }
}

GridPictureCanvas::GridPictureCanvas(QWidget *parent) : QWidget(parent)
{
    waitDlg = new WaitMessageBox(WaitLoadImageMsg);
    isRunningState = false;
    runningParams = new GridPictureParams;
    completeParams = new GridPictureParams;
    imageview = new TableView;
    imagemodel = new StandardItemModel;
    imagedelegate = new ImageDelegate;
    imageview->setModel(imagemodel);
    imageview->horizontalHeader()->setVisible(false);
    imageview->horizontalHeader()->setStretchLastSection(false);
    imageview->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
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
    connect(imageview,&TableView::clicked,this,&GridPictureCanvas::clickImage);
    connect(this,&GridPictureCanvas::readyRead,waitDlg,QOverload<>::of(&WaitMessageBox::wait),Qt::QueuedConnection);
    connect(this,&GridPictureCanvas::finished,waitDlg,QOverload<>::of(&WaitMessageBox::accept),Qt::AutoConnection);
}

void GridPictureCanvas::setState(bool isRunning)
{
    isRunningState = isRunning;
}

bool GridPictureCanvas::isRunning() const
{
    return isRunningState;
}

QImageVectorInfo GridPictureCanvas::listDirAllImages(const QString &dirpath, const QString &filter)
{
    QImageVectorInfo imageInfo;
    QDir dir(dirpath);
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList(QStringList()<<filter);
    for(auto info: list) {
        auto imgpath = info.filePath();
        //LOG<<imgpath;
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

