﻿/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-27 17:27:13
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-27 17:29:50
 * @FilePath: \EOS\component\src\widgets\photographic.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "photographic.h"
#include "qactiongroup.h"

PhotoGraphics::PhotoGraphics(QWidget *parent) : QWidget(parent)
{
    mirrorType = MirrorType::NoMirror;
    pix = new GraphicsPixmapItem(QPixmap());
    text = new QGraphicsTextItem(QString("cpu: %1% memory: %2%").arg(0).arg(0));
    graphicsscene = new QGraphicsScene();
    graphicsscene->setSceneRect(sceneRefX,sceneRefY,sceneWidth,sceneHeight);
    graphicsscene->addItem(pix);
    graphicsscene->addItem(text);
    pix->setPos(sceneRefX,sceneRefY); // 将场景的起点坐标映射到pix的坐标系下,这样pix可以在场景的左上角保持一致,pix和场景的起点一样
    text->setPos(-sceneRefX,sceneRefY);
    // 起点一致后,然后pix缩放到和场景一样就不会出问题了,整体覆盖
    // 如果pix缩放的时候缩放到sceneWidth,sceneHeight的一半,这样pix就在四分区域的左上角
    // 这个时候还想挪到中间,就是让pix的左上角在场景四分左上区域的中心就可以,这个位置就是sceneRefX/2,sceneRefY/2
    // pix->setPos(pix->mapFromScene(sceneRefX/2,sceneRefY/2));

    graphicsview = new GraphicsView(graphicsscene);
    graphicsview->fitInView(pix,Qt::KeepAspectRatio); // 确保item紧密的适合视图,

    rotateact = new QAction(tr(RotateField));
    resetact =  new QAction(tr(ResetField));
    auto sep = new QAction();
    sep->setSeparator(true);
    nomirroract = new QAction(tr(NoMirrorField));
    hormirroract = new QAction(tr(HorizontalMirrorField));
    vermirroract = new QAction(tr(VerticalMirrorField));
    allmirroract = new QAction(tr(AllMirrorField));

    QActionGroup * actionGroup = new QActionGroup(this);
    actionGroup->addAction(nomirroract);
    actionGroup->addAction(hormirroract);
    actionGroup->addAction(vermirroract);
    actionGroup->addAction(allmirroract);
    actionGroup->setExclusive(true);
    nomirroract->setCheckable(true);
    hormirroract->setCheckable(true);
    vermirroract->setCheckable(true);
    allmirroract->setCheckable(true);
    nomirroract->setChecked(true);

    addAction(rotateact);
    addAction(resetact);
    addAction(sep);
    addAction(nomirroract);
    addAction(hormirroract);
    addAction(vermirroract);
    addAction(allmirroract);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(rotateact,&QAction::triggered,this,&PhotoGraphics::rotate);
    connect(resetact,&QAction::triggered,this,&PhotoGraphics::reset);
    connect(actionGroup,&QActionGroup::triggered,this,&PhotoGraphics::mirror);
    connect(CpuMemoryMonitorPointer,&CpuMemoryMonitor::monitorValue,this,&PhotoGraphics::updateText);
    CpuMemoryMonitorPointer->start(1000);
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
    mirrorType = MirrorType::NoMirror;
    nomirroract->setChecked(true);
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

void PhotoGraphics::mirror(QAction *act)
{
    if (act == nomirroract) {
        mirrorType = MirrorType::NoMirror;
    } else if (act == hormirroract) {
        mirrorType = MirrorType::HorMirror;
    } else if (act == vermirroract) {
        mirrorType = MirrorType::VerMirror;
    } else if (act == allmirroract) {
        mirrorType = MirrorType::AllMirror;
    }
    graphicsview->update();
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
            auto img_ = img;
            switch (mirrorType) {
                case MirrorType::NoMirror:
                    //LOG<<"no mirror";
                    break;
                case MirrorType::HorMirror:
                    img_ = img.mirrored(true,false);
                    //LOG<<"hor mirror";
                    break;
                case MirrorType::VerMirror:
                    img_ = img.mirrored(false,true);
                    //LOG<<"ver mirror";
                    break;
                case MirrorType::AllMirror:
                    img_ = img.mirrored(true,true);
                    //LOG<<"all mirror";
                    break;
            }
            pix->setImage(img_);
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
    text->setPos(-sceneRefX-200,sceneRefY);
    //LOG<<pix->scenePos()<<pix->pos(); // 确实变化重新计算了
    event->accept();
    update();
}

void PhotoGraphics::updateText(quint64 cpu, quint64 memory)
{
    text->setPlainText(QString("cpu: %1% memory: %2%").arg(cpu).arg(memory));
    graphicsview->update();
}