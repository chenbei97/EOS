/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-25 09:48:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-15 16:11:37
 * @FilePath: \EOS\component\src\widgets\photocanvas.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "photocanvas.h"

void PhotoCanvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(DefaultPainterPenWidth);
    painter.setPen(pen);

    /*高频率绘图的注意事项:
     * 1. 不要在paintEvent内做QVariantMap到QImage的变换,会卡死
     * 2. 不要在paintEvent内做QImage的scaled变换
     * 3. QPixmap::fromImage到QPixmap的变换好像没有影响,不过端到端都是QImage,也不要做此类变换耗时
     * 4. 所以现在最好就是调用setImage直接传递数据,没有QVariantMap到QImage
     * 5. 使用定时器定时的刷新减少重绘频率也可以降低cpu占用
     * */
    switch (mStrategy) {
        case NoStrategy:
            break;
        case SinglePixmap:
            if(!mimage.isNull()) {
//                painter.drawImage(QRectF(width()*(1.0-zoomRate)/2.0,height()*(1.0-zoomRate)/2.0,
//                                         width()*zoomRate,height()*zoomRate),mimage);
                painter.translate(width()/2,height()/2); // 或者把画笔挪到中间再画
                painter.drawImage(QRectF(-width()*zoomRate/2,-height()*zoomRate/2,
                                         width()*zoomRate,height()*zoomRate),mimage);
                painter.translate(0,0); // 防止对继承的或者后面的产生影响
            }
            break;
        case GridPixmap:
            drawGridImage(painter);
            break;
    }
    // 绘制框
//    if (!mDrapRect.isNull()) {
//        auto pen = painter.pen();
//        pen.setColor(Qt::blue);
//        painter.setPen(pen);
//        painter.drawRect(mDrapRect);
//        pen.setColor(Qt::black); // 恢复,否则绘制其他的都变颜色了
//        painter.setPen(pen);
//    }

    event->accept();
}

void PhotoCanvas::drawGridImage(QPainter &painter)
{
    if (mGridSize == 0) return;

    // 1. 绘制网格线
    auto pen = painter.pen();
    painter.drawRect(0,0,width(),height());
    auto rw = getInnerRectWidth();
    auto rh = getInnerRectHeight();
    pen.setColor(Qt::darkGray);
    pen.setWidth(DefaultPainterPenWidth/2);
    painter.setPen(pen);
    for(int i = 0; i < mGridSize; i++) {
        auto left = QPointF(0.0,rh) + QPointF(0.0,rh*i);
        auto right = QPointF(width(),rh) + QPointF(width(),rh*i);
        auto top =QPointF(rw,0.0)+QPointF(rw*i,0.0);
        auto bottom =QPointF(rw,height())+QPointF(rw*i,height());
        painter.drawLine(left,right);
        painter.drawLine(top,bottom);
    }

//    // 2. 绘制图列表
    if (!mGridImageVector.isEmpty()) {
        for(auto gridimage: mGridImageVector) {
            auto rect = gridimage.first;
            auto img = gridimage.second;
            painter.drawImage(rect,img);
        }
    }

    // 3. 绘制框线
    pen.setStyle(Qt::DashLine);
    pen.setColor(Qt::blue);
    pen.setWidth(DefaultPainterPenWidth);
    painter.setPen(pen);
    painter.drawRect(mBoundingRect);

    // 4. 绘制图的中心和虚线框
    if (!mGridCenterPoints.isEmpty()) {
        pen.setColor(PurpleEA3FF7);
        pen.setWidth(DefaultPainterPenWidth*2);
        painter.setPen(pen);
        for(auto pt: mGridCenterPoints) {
            painter.drawPoint(pt);
        }
        pen.setStyle(Qt::DashLine);
        pen.setWidth(DefaultPainterPenWidth);
        painter.setPen(pen);
        for(auto pt: mGridCenterPoints) {
            auto rect = QRectF(pt.x()-rw/2,pt.y()-rh/2,rw,rh);
            painter.drawRect(rect);
        }
    }

    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::black);
    painter.setPen(pen);
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
{ // 对于SinglePixmap模式快捷方法是setImage
    // 1. 初始化通用信息
    mStrategy = s;
    mMousePoint = {-1,-1};
    mLastPos = {-1,-1};
    mDrapRect = QRectF();

    // 2. 初始化xx策略需要的信息
    mStrategyInfo = m;
    if (!m.isEmpty()) {
        // paintEvent避免做QVariantMap=>QImage转换,提前转换好
        if (mStrategy == SinglePixmap) { // 使用setData外部要传递ImageField字段
            if(m.keys().contains(ImageField)) {
                mimage = m[ImageField].value<QImage>()
                        .scaled(width(),height(),
                                Qt::KeepAspectRatio,Qt::FastTransformation);
            }
        } else if (mStrategy == GridPixmap){
            if(m.keys().contains(GridSizeField)) {
                mGridSize = m[GridSizeField].toInt();
            }
        }
    }
    update();
}

void PhotoCanvas::setGridSize(int gridsize)
{
    Q_ASSERT(mStrategy == GridPixmap);
    mGridSize = gridsize;
    update();
}

void PhotoCanvas::clearGridImage()
{
    Q_ASSERT(mStrategy == GridPixmap);
    mGridImageVector.clear();
    mGridCenterPoints.clear();
    update();
}

void PhotoCanvas::appendImage(const QImage &img, const QPointF &point)
{ // point是归一化的中心坐标
    Q_ASSERT(mStrategy == GridPixmap);
    auto w = getInnerRectWidth();
    auto h = getInnerRectHeight();
    auto x = point.x() * width()-w/2.0; // 中心坐标变成左上角顶点坐标
    auto y = point.y() * height()-h/2.0;
    auto rect = QRectF(x,y,w,h);
    mGridImageVector.append(qMakePair(rect,img));
    mGridCenterPoints.append(QPointF(x+w/2.0,y+h/2.0));
    //LOG<<mGridCenterPoints.count()<<mGridImageVector.count();
    update();
}

void PhotoCanvas::setImage(const QImage &img, int duration)
{ // 手动掉过duration张然后update
    Q_ASSERT(mStrategy == SinglePixmap);
    static long long count = 0;
    if (count % duration == 0) { // 不能能用count % 10取非好像long会有问题
        // 一种除了定时update辅助减少界面刷新的功能,10张图显示1次
#ifdef use_imagetransformthread
        if (!img.isNull()) {
            //LOG<<rotateAngle<<(int)mirrorType;
            ImageTransformThreadPointer->setImage(img,rotateAngle,mirrorType);
        }
        else
            ImageTransformThreadPointer->setImage(QImage(),0.0,MirrorType::NoMirror);
#else
        if (!img.isNull()) {
            switch (mirrorType) {
                case MirrorType::NoMirror:
                    //LOG<<"no mirror";
                    mimage = img;
                    break;
                case MirrorType::HorMirror:
                    mimage = img.mirrored(true,false);
                    //LOG<<"hor mirror";
                    break;
                case MirrorType::VerMirror:
                    mimage = img.mirrored(false,true);
                    //LOG<<"ver mirror";
                    break;
                case MirrorType::AllMirror:
                    mimage = img.mirrored(true,true);
                    //LOG<<"all mirror";
                    break;
            }
            if (rotateAngle > 0.0 && rotateAngle < 360.0) {
                QTransform transform;
                transform.rotate(rotateAngle);
                transform.scale(width()*1.0/img.width(),height()*1.0/img.height());
                mimage = mimage.transformed(transform,Qt::FastTransformation);
            } else {
                mimage = mimage.scaled(width(),height(),Qt::KeepAspectRatio,Qt::FastTransformation);
            }
        }
        else
            mimage = QImage();
#endif
        //update();
    }
    count++;

    if (count > 1000 ){
        count = 0; // 防止一直累计溢出
    }
}

void PhotoCanvas::setImage(const QImage &img)
{ // 立即update,本方法不参与变换只用于静态图非live模式因为可能会卡
    Q_ASSERT(mStrategy == SinglePixmap);
    if (!img.isNull()) {
        QTransform transform;
        //transform.rotate(rotateAngle); //本方法不参与变换
        transform.scale(width()*1.0/img.width(),height()*1.0/img.height());
        //mimage = img.scaled(width(),height(),Qt::KeepAspectRatio,Qt::FastTransformation);
        mimage = img.transformed(transform,Qt::FastTransformation);
    }
    else
        mimage = QImage();
    update(); // 这里是立即刷新的可能会卡
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

PhotoCanvas::DrawStrategy PhotoCanvas::strategy() const
{
    return mStrategy;
}

PhotoCanvas::PhotoCanvas(QWidget *parent) : QWidget(parent)
{
    mStrategy = NoStrategy;
    mDrapRect = QRect();
    mBoundingRect = QRectF();
    mMousePoint = QPoint(-1,-1);
    mLastPos = {-1,-1};
    mMouseClickColor.setAlpha(DefaultColorAlpha);
    setContextMenuPolicy(Qt::ActionsContextMenu);

#ifdef use_imagetransformthread
    ImageTransformThreadPointer->stopThread();
    ImageTransformThreadPointer->setImageSize(this->size());
#endif

//    static bool flag = false;
//    connect(&timer,&QTimer::timeout,[this]{setUpdatesEnabled(flag);flag=!flag;});
    connect(&timer,&QTimer::timeout,[this]{update();});
}

PhotoCanvas::~PhotoCanvas() noexcept
{
#ifdef use_imagetransformthread
    if (ImageTransformThreadPointer->isRunning()) {
        ImageTransformThreadPointer->stopThread();
    }
#endif
}

void PhotoCanvas::upateImageByThread(const QImage& img)
{
    static long count = 0;
    static long count_c = 0;
    mimage = img;
    count++;
    //LOG<<"accept image count = "<<count<<" _c = "<<count_c;
    if (count > LONG_MAX-1)
    {
        count = 0;
        count_c++;
    }
    update();
}

void PhotoCanvas::enableTransformThread(bool e)
{
#ifdef use_imagetransformthread
    e?
    ImageTransformThreadPointer->startThread():
    ImageTransformThreadPointer->stopThread();
    disconnect(ImageTransformThreadPointer,&ImageTransformThread::imageTransformed,this,&PhotoCanvas::upateImageByThread);
    e?
    connect(ImageTransformThreadPointer,&ImageTransformThread::imageTransformed,this,&PhotoCanvas::upateImageByThread):
    disconnect(ImageTransformThreadPointer,&ImageTransformThread::imageTransformed,this,&PhotoCanvas::upateImageByThread);
#endif
}

void PhotoCanvas::updateRect(const QRectF &rect)
{
    Q_ASSERT(mStrategy == GridPixmap);
    auto x = rect.topLeft().x() * width();
    auto y = rect.topLeft().y() * height();
    auto w = rect.width() * width();
    auto h = rect.height() * height();
    mBoundingRect = QRectF(x,y,w,h);

    mGridCenterPoints.clear();
    auto ref_w = getInnerRectWidth();
    auto ref_h = getInnerRectHeight();
    auto x0 = qCeil(x);// 电机坐标起点
    auto y0 = qCeil(y);
    auto step_x = qCeil(width() / mGridSize); // 遍历的整数步进
    auto step_y = qCeil(height() / mGridSize);

    for(int x = x0; x < x0 + w; x += step_x) {
        for(int y = y0; y < y0 + h; y += step_y) {
            auto center_x = x+step_x/2.0;
            auto center_y = y+step_y/2.0;
            mGridCenterPoints.append(QPointF(center_x,center_y));
        }
    }
    update();
}

bool PhotoCanvas::hasBoundingRect() const
{
    return !mBoundingRect.isEmpty();
}

void PhotoCanvas::optimizePaint(int ms)
{
    timer.start(ms);
}

void PhotoCanvas::stopOptimizePaint()
{
    timer.stop();
}

double PhotoCanvas::getInnerRectWidth() const
{
    return width() * 1.0 / mGridSize;
}

double PhotoCanvas::getInnerRectHeight() const
{
    return height() * 1.0 / mGridSize;
}

