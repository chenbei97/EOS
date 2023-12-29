/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-27 17:27:13
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-27 17:29:50
 * @FilePath: \EOS\component\src\widgets\photographic.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "photographic.h"

PhotoGraphics::PhotoGraphics(QWidget *parent) : QWidget(parent)
{
    pix = new GraphicsPixmapItem(QPixmap());
    graphicsscene = new QGraphicsScene();
    graphicsscene->setSceneRect(sceneRefX,sceneRefY,sceneWidth,sceneHeight);
    graphicsscene->addItem(pix);
    pix->setPos(sceneRefX,sceneRefY); // 将场景的起点坐标映射到pix的坐标系下,这样pix可以在场景的左上角保持一致,pix和场景的起点一样
    // 起点一致后,然后pix缩放到和场景一样就不会出问题了,整体覆盖
    // 如果pix缩放的时候缩放到sceneWidth,sceneHeight的一半,这样pix就在四分区域的左上角
    // 这个时候还想挪到中间,就是让pix的左上角在场景四分左上区域的中心就可以,这个位置就是sceneRefX/2,sceneRefY/2
    // pix->setPos(pix->mapFromScene(sceneRefX/2,sceneRefY/2));

    graphicsview = new GraphicsView(graphicsscene);
    graphicsview->fitInView(pix,Qt::KeepAspectRatio); // 确保item紧密的适合视图,

    rotateact = new QAction("rotate");
    resetact =  new QAction("reset");
    addAction(rotateact);
    addAction(resetact);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(rotateact,&QAction::triggered,this,&PhotoGraphics::rotate);
    connect(resetact,&QAction::triggered,this,&PhotoGraphics::reset);

    auto lay = new QHBoxLayout;
    lay->addWidget(graphicsview);
    lay->setMargin(0); // 这个需要否则size()比view的size()稍大
    lay->setSpacing(0); // view的size()总是跟随窗体,不同于场景矩形
    setLayout(lay);
}

void PhotoGraphics::reset()
{
#ifdef use_transform_view
    pix->setPos(sceneRefX,sceneRefY); // 可能有平移操作,也要复位
    graphicsview->resetTransform(); // 平移操作是对pix而不是view的所以这里reset不能取消平移操作
#else
    pix->reset();
#endif
}

void PhotoGraphics::rotate()
{
#ifdef use_transform_view
    graphicsview->rotate(90); // 旋转view也可以
    ////graphicsview->fitInView(pix); // 设置这个的话场景和窗体之间的间距就没有了
    //graphicsview->centerOn(pix); // 不设置也行
#else
    pix->rotate(90);
#endif
}

void PhotoGraphics::setImage(const QImage &img, int duration)
{
    static long long count = 0;
    if (count % duration == 0) { // 不能能用count % 10取非好像long会有问题
        // 一种除了定时update辅助减少界面刷新的功能,10张图显示1次
        if (!img.isNull()) {
//            LOG<<graphicsview->size()<<size() // 2个一样
//                <<graphicsscene->sceneRect().size() // 固定不变了
//                <<pix->scenePos()<<pix->pos() // pix的场景坐标就是(sceneRefX,sceneRefY)
//                <<pix->boundingRect(); // pix的起点和场景已经一样,缩放的尺寸又是场景的尺寸,所有恰好填充场景
            pix->setImage(img);
        }
        else
            pix->setPixmap(QPixmap());
    }
    count++;
    if (count > 1000 ){
        count = 0; // 防止一直累计溢出
    }
    graphicsview->update();
}

void PhotoGraphics::wheelEvent(QWheelEvent *event)
{
    auto pixel = event->angleDelta();

#ifdef use_transform_view
    pixel.y()>0 ? graphicsview->scale(zoomOutRate,zoomOutRate):graphicsview->scale(zoomInRate,zoomInRate);
    //graphicsview->fitInView(pix); // 会导致无法缩放不能设置
    // 没有缩放限制,实测好像可以一直缩小
#else
    pixel.y() > 0 ? pix->scaling(zoomOutRate):pix->scaling(zoomInRate); // 理论可以无限大
#endif
    event->accept();
}

void PhotoGraphics::resizeEvent(QResizeEvent*event)
{
     //尺寸变了,可能变大变小,变大还好,变小的话场景尺寸或者说pix尺寸超过了窗体本身,需要重新调整
    // LOG<<graphicsview->size()<<size()<<event->size(); // 都相等
    sceneHeight = height() - 2.* sceneMargin; // 场景总是小于窗体尺寸固定的距离
    sceneWidth = width() - 2.* sceneMargin;
    sceneRefX = - sceneWidth / 2.;
    sceneRefY = - sceneHeight / 2.;
    graphicsscene->setSceneRect(sceneRefX,sceneRefY,sceneWidth,sceneHeight);
    //LOG<<sceneWidth<<sceneHeight<<sceneRefX<<sceneRefY<<graphicsscene->sceneRect();
    //graphicsview->fitInView(graphicsscene->sceneRect(), Qt::KeepAspectRatio);//不能加这个
    //LOG<<pix->scenePos()<<pix->pos()<<pix->pixmap().size();
    pix->setPos(sceneRefX,sceneRefY);//不知为啥这里直接设置而不是pix->mapFromScene(sceneRefX,sceneRefY)
    //LOG<<pix->scenePos()<<pix->pos(); // 确实变化重新计算了
    event->accept();
    update();
}