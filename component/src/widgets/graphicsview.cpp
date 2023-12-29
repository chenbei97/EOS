/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-29 14:12:06
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-29 14:18:05
 * @FilePath: \EOS\component\src\widgets\graphicsview.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */


#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget*parent):QGraphicsView(parent)
{
    initAttributes();
}

GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene,parent)
{
    initAttributes();
}

void GraphicsView::initAttributes()
{
    //setViewport(new QGLWidget(QGLFormat(QGL::DirectRendering))); // 启用opengl视口不过存在一些问题
    setCacheMode(QGraphicsView::CacheBackground);// 默认不缓存直接视口完成,缓存目的是加快渲染速度慢的区域
    setDragMode(QGraphicsView::ScrollHandDrag); // 整体拖拽/框选拖拽,因为只有1个item用整体,默认NoDrag
    //setOptimizationFlag(QGraphicsView::IndirectPainting); // 可能有副作用,默认没有任何设置
    setResizeAnchor(QGraphicsView::NoAnchor);// 默认没有锚点
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);//默认视图中心的场景点是锚点/鼠标下方的点用作锚点
    //SmartViewportUpdate智能分析 BoundingRectViewportUpdate重新绘制视口中所有更改的边界矩形
    // 视口如何更新内容,默认MinimalViewportUpdate 最大限度减少绘制时间,FullViewportUpdate任何可见部分更改都会更新
    setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    setAlignment(Qt::AlignCenter); // 没有滚动条出现时场景如何渲染,默认居中
    setInteractive(true);//默认允许交互,否则忽略鼠标和键盘事件
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform); // 默认TextAntialiasing
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setBackgroundBrush(Qt::gray);
    //setForegroundBrush(Qt::red); // 前景是图像本身,不要设置否则不能显示图像
    setWindowOpacity(0.2);
    setContentsMargins(0,0,0,0);
}