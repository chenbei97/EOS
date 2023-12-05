/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-25 09:48:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 09:49:29
 * @FilePath: \EOS\interface\src\preview\photocanvas.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "photocanvas.h"

void PhotoCanvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    /*高频率绘图的注意事项:
     * 1. 不要在paintEvent内做QVariantMap到QImage的变换,会卡死
     * 2. 不要在paintEvent内做QImage的scaled变换
     * 3. QPixmap::fromImage到QPixmap的变换好像没有影响,不过端到端都是QImage,也不要做此类变换耗时
     * 4. 所以现在最好就是调用setImage直接传递数据,没有QVariantMap到QImage
     * 5. 使用定时器定时的刷新减少重绘频率也可以降低cpu占用
     * */
    switch (strategy) {
        case NoStrategy:
            break;
        case SinglePixmap:
            if(mimage.isNull()) return;
            painter.drawImage(QRect(0,0,width(),height()),mimage);
            //painter.drawPixmap(targetRect,QPixmap::fromImage(mimage));
            break;
        case GridPixmap:
            drawGridLine(painter);
            break;
    }
    // 绘制框
    if (!mDrapRect.isNull()) {
        auto pen = painter.pen();
        pen.setColor(Qt::blue);
        painter.setPen(pen);
        painter.drawRect(mDrapRect);
        pen.setColor(Qt::black); // 恢复,否则绘制其他的都变颜色了
        painter.setPen(pen);
    }

    event->accept();
}

void PhotoCanvas::mousePressEvent(QMouseEvent *event)
{ // 左键点击清除框选,计算鼠标点击的小矩形区域坐标
    if (event->button() == Qt::LeftButton) {
        mDrapRect.setWidth(0);
        mDrapRect.setHeight(0);
        mDrapRect = QRectF();
        update();
    } // 右键是菜单
    mLastPos = event->pos();
    //mMousePoint = {-1,-1};
    //emit mouseClicked(mMousePoint);
    emit mouseClicked();
    event->accept();
    update();
}

void PhotoCanvas::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit doubleMouseClicked();
}

void PhotoCanvas::mouseMoveEvent(QMouseEvent *event)
{ // 绘制拖拽框
    if (event->buttons() & Qt::LeftButton){
        auto end = event->pos(); // 鼠标停下的点
        mDrapRect = QRectF(mLastPos,end); // 鼠标形成的矩形框
    }
    update();
    event->accept();
}

void PhotoCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
}

void PhotoCanvas::setStrategy(PhotoCanvas::DrawStrategy s, const QVariantMap& m)
{ //
    // 1. 初始化通用信息
    strategy = s;
    mMousePoint = {-1,-1};
    mLastPos = {-1,-1};
    mDrapRect = QRectF();

    // 2. 初始化xx策略需要的信息
    mStrategyInfo = m; // 目前这个设计用不上,直接使用setImage/setData比较快捷,setImage最快捷
    update();
}

void PhotoCanvas::setData(const QVariantMap &m)
{
    mStrategyInfo = m;
    // paintEvent避免做QVariantMap=>QImage转换,提前转换好
    mimage = m[ImageField].value<QImage>()
        .scaled(width(),height(),
        Qt::KeepAspectRatio,Qt::FastTransformation);
    //update();
}

void PhotoCanvas::setImage(const QImage &img, int duration)
{
    static long long count = 0;
    static long c = 0;
    if (count % duration == 0) { // 不能能用count % 10取非好像long会有问题
        // 一种除了定时update辅助减少界面刷新的功能,10张图显示1次
        if (!img.isNull())
            mimage = img.scaled(width(),height(),Qt::KeepAspectRatio,Qt::FastTransformation);
        else
            mimage = QImage();
        //LOG<<count<<c;
        //update();// 这是定时器来帮助刷新
        ++c;
    }
    count++;

    if (count > 1000 ){
        count = 0; // 防止一直累计溢出
        c = 0;
    }
}

void PhotoCanvas::setImage(const QImage &img)
{
    if (!img.isNull())
        mimage = img.scaled(width(),height(),Qt::KeepAspectRatio,Qt::FastTransformation);
    else
        mimage = QImage();
    update(); // 这里是立即刷新的
}

void PhotoCanvas::setPixmap(const QPixmap &pix)
{
    setImage(pix.toImage());
}

QImage PhotoCanvas::image() const
{
    return mimage;
}

QPixmap PhotoCanvas::pixmap() const
{
    return QPixmap::fromImage(mimage);
}

PhotoCanvas::PhotoCanvas(QWidget *parent) : QWidget(parent)
{
    strategy = NoStrategy;
    mDrapRect = QRect();
    mMousePoint = QPoint(-1,-1);
    mLastPos = {-1,-1};
    mMouseClickColor.setAlpha(DefaultColorAlpha);
    setContextMenuPolicy(Qt::ActionsContextMenu);

//    static bool flag = false;
//    connect(&timer,&QTimer::timeout,[this]{setUpdatesEnabled(flag);flag=!flag;});
    connect(&timer,&QTimer::timeout,[this]{update();});
}

void PhotoCanvas::optimizePaint(int ms)
{
    timer.start(ms);
}

void PhotoCanvas::stopOptimizePaint()
{
    timer.stop();
}

void PhotoCanvas::drawGridLine(QPainter &painter)
{
    mGridSize = 10;
    if (mGridSize == 0) return;
    painter.drawRect(0,0,width(),height());
    auto rw = getInnerRectWidth();
    auto rh = getInnerRectHeight();
    for(int i = 0; i < mGridSize; i++) {
        auto left = QPointF(0.0,rh) + QPointF(0.0,rh*i);
        auto right = QPointF(width(),rh) + QPointF(width(),rh*i);
        auto top =QPointF(rw,0.0)+QPointF(rw*i,0.0);
        auto bottom =QPointF(rw,height())+QPointF(rw*i,height());
        painter.drawLine(left,right);
        painter.drawLine(top,bottom);
    }
}

double PhotoCanvas::getInnerRectWidth() const
{
    return width() * 1.0 / mGridSize;
}

double PhotoCanvas::getInnerRectHeight() const
{
    return height() * 1.0 / mGridSize;
}

