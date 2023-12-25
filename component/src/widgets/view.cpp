/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-05 09:15:24
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-06 10:24:18
 * @FilePath: \EOS\component\src\widgets\view.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "view.h"

View::View(QWidget *parent) : QWidget(parent)
{
    highcolor  = Qt::green;
    highcolor.setAlpha(DefaultColorAlpha);
    trianglen = ViewTriangleLength;

    mDisableRectRates[Qt::AlignLeft] = 0.0;
    mDisableRectRates[Qt::AlignRight] = 0.0;
    mDisableRectRates[Qt::AlignTop] = 0.0;
    mDisableRectRates[Qt::AlignBottom] = 0.0;
    initDispersedMask();
    saveviewact = new QAction(tr(SaveViewActTitle));
    removeviewact = new QAction(tr(RemoveViewActTitle));
    applyholeact = new QAction(tr(ApplyHoleActTitle));
    applygroupact = new QAction(tr(ApplyGroupActTitle));
    applyallact = new QAction(tr(ApplyAllActTitle));
    quitviewact = new QAction(tr(QuitViewActTitle));

    addAction(saveviewact);
    addAction(removeviewact);
    //addAction(applyholeact); // 不显式添加
    addAction(applygroupact);
    addAction(applyallact);
    addAction(quitviewact);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(saveviewact,&QAction::triggered,this,&View::onSaveViewAct);
    connect(removeviewact,&QAction::triggered,this,&View::onRemoveViewAct);
    connect(applyholeact,&QAction::triggered,this,&View::onApplyHoleAct);
    connect(applygroupact,&QAction::triggered,this,&View::onApplyGroupAct);
    connect(applyallact,&QAction::triggered,this,&View::onApplyAllAct);
    connect(quitviewact,&QAction::triggered,this,&View::quitView);
}

void View::initDispersedMask()
{
    mDispersedMask.clear();
    for(int r = 0; r < mDispersedMaskSize; ++r)
    {
        QBoolVector vec;
        for(int c = 0; c < mDispersedMaskSize; ++c) {
            vec.append(false);
        }
        mDispersedMask.append(vec);
    }
}

QPointFVector View::overlap(const QPointFVector &points, double rate)
{
    if (points.isEmpty())  // 删点时可能为空,下边索引0就越界了
        return points;

    QPointFVector pts;
    pts.append(points[0]);
    auto x0 = points[0].x();
    auto y0 = points[0].y();

    for(int i = 1; i < points.count(); ++i) {
        auto x = points[i].x();
        auto y = points[i].y();

        if (x >= x0) x *= (1.0-rate); // 在基点右侧,x应该减少才是靠近
        else x *= (1.0+rate); // 基点左侧,x增加才是靠近
        if (y < y0) y *= (1.0+rate); // 在基点上边,y应该增加是靠近
        else y *= (1.0-rate);

        pts.append(QPointF(x,y));
    }
    return pts;
}

QPointF View::mapFromPointF(const QPointF &point) const
{ // 把坐标归一化到0-1
    auto pos = point-getExternalRectTopLeftPoint();
    //LOG<<point<<getExternalRectTopLeftPoint()<<pos;
    pos = pos / (getCircleRadius() * 2.0);
    return pos;
}

QPointF View::mapToPointF(const QPointF &point) const
{ // 把0-1的坐标放大到ui尺寸上
    auto pos = QPointF(point.x() * getCircleRadius() * 2.0,
                       point.y() * getCircleRadius() * 2.0);

    pos = pos+getExternalRectTopLeftPoint();
    return pos;
}

QRectF View::mapToSize(const QRectF &source,const QPointF&ref_point,int ref_w,int ref_h) const
{ // source已经是0-1的等比例缩放了
    auto topleft = source.topLeft();
    auto size = source.size();

    auto x = topleft.x() * ref_w + ref_point.x();// 左上角顶点(0.2,0.3),长度(0.25,0.35)
    auto y = topleft.y() * ref_h + ref_point.y();// 放大到尺寸为(ref_w,ref_h),例如100
    auto w = size.width() * ref_w; // 那么结果就是(20,30,25,35),如果左上角参考点还需要移动就变成 (30,50,25,35)
    auto h = size.height() * ref_h;

    return QRectF(x,y,w,h);
}

QRectF View::mapFromSize(const QRectF &source,const QPointF&ref_point,int ref_w,int ref_h) const
{ //source非标准,将其标准化到0-1
    auto topleft = source.topLeft();
    auto size = source.size();

    auto x = (topleft.x() - ref_point.x()) / ref_w;// 相对窗口左上角的区域(100,0,50,50),参考区域是(50,0,100,100)
    auto y = (topleft.y() - ref_point.y()) / ref_h;// [(100-50)/100,(0-0)/100,50/100,50/100]
    auto w = size.width() / ref_w; // 那么结果就是(0.5,0,0.5,0.5)
    auto h = size.height() / ref_h;
    //LOG<<QRectF(x,y,w,h);

    return QRectF(x,y,w,h);
}

void View::setViewInfo(const ViewInfo &info)
{
    mViewInfo = info;
    checkField();

    mMousePos = {-1.0,-1.0};
    mValidMousePos = {-1.0,-1.0};
    mMouseRect = QRectF();
    mDrapRectF = QRectF();

#ifdef viewRowColUnEqual
    auto dimension = mViewInfo[HoleViewSizeField].value<Dimension2D>();
    mDimension = dimension;
#else
    auto size =  mViewInfo[HoleViewSizeField].toInt();
    mSize = size;
#endif
    update();
}

ViewInfo View::viewInfo() const
{
    return mViewInfo;
}

#ifdef viewRowColUnEqual
void View::setViewSize(int rows,int cols)
{
    mDimension.rows = rows;
    mDimension.cols = cols;
    update();
}

Dimension2D View::viewSize() const
{
    return mDimension;
}
#else
void View::setViewSize(int size)
{
    mSize = size;
    update();
}

int View::viewSize() const
{
    return mSize;
}
#endif

int View::holeID() const
{ // 每个孔双击打开视野窗口都是一对一的
    auto coordinate = mViewInfo[HoleCoordinateField].toPoint();
//    if (coordinate.isNull()) return -1;

    auto id = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
    return id;
}

int View::holeID(const QPoint& holePoint) const
{
    return holePoint.x() * PointToIDCoefficient + holePoint.y();
}

void View::mousePressEvent(QMouseEvent *event)
{
    // 1.框选后随机点一下要清除框选框(只能拖拽生成),同时出现单击框
    if (event->button() == Qt::LeftButton) {
        mMousePos = event->pos();
        if (isValidPoint(mMousePos)) {
            mValidMousePos = mMousePos;
        }

        mDrapRectF = QRectF();

        auto diameter = width()>=height()?height():width();
        auto mouseRect_topleft = mValidMousePos-QPointF(getInnerRectWidth()/2,getInnerRectHeight()/2);
        auto mouseRect_bottomright = mValidMousePos+QPointF(getInnerRectWidth()/2,getInnerRectHeight()/2);

        if (isValidRect(QRectF(mouseRect_topleft,mouseRect_bottomright))) {
            mMouseRect = mapFromSize(QRectF(mouseRect_topleft,mouseRect_bottomright),
                                     getExternalRectTopLeftPoint(),diameter,diameter);
        }
        update();
    } else if (event->button() == Qt::RightButton) {
        if (mViewInfo[HoleGroupNameField].toString().isEmpty()) { //
            applyallact->setEnabled(false);
            applygroupact->setEnabled(false);
            saveviewact->setEnabled(false);
            removeviewact->setEnabled(false);
        } else {
            applyallact->setEnabled(true);
            applygroupact->setEnabled(true);
            saveviewact->setEnabled(true);
            removeviewact->setEnabled(true);
        }
    }
    update();
    event->accept();
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    if (mDrapRectF.isEmpty() && event->button() == Qt::LeftButton) { // 释放时判断如果是拖动事件或者是右键不发射移动电机
        auto pos = mapFromPointF(mMousePos);
        emit previewEvent(pos);//注意:这里不能用mValidMousePos,他可能没变,鼠标点击无效区域也触发了,另外只能左键
    }
    update();
    event->accept();
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton){
        auto end = event->pos(); // 鼠标停下的点
        if (!isValidRect(QRectF(mValidMousePos,end)))
            return; // 框选到无效区域
        mMouseRect = QRectF();
        auto diameter = getCircleRadius()*2.0;
        // 鼠标形成的矩形框将其等比例缩放到0-1
        mDrapRectF = mapFromSize(QRectF(mValidMousePos,end),
                                 getExternalRectTopLeftPoint(),diameter,diameter);
    }
    update();
    event->accept();
}

void View::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    auto pen = painter.pen();
    pen.setWidth(DefaultPainterPenWidth);
    painter.setPen(pen);
    auto gc = QColor(Qt::gray);
    gc.setAlpha(DefaultColorAlpha);
    painter.fillRect(rect(),gc);


    mSelectMode == ViewMode::PointMode? trianglen = ViewTriangleLength: trianglen = 0;
    auto radius = getCircleRadius();
    auto diameter = radius * 2.0;
    QPainterPath path;
    path.addEllipse(QPointF(width()/2.0,height()/2.0),radius,radius);
    painter.drawPath(path);
    painter.fillPath(path,DefaultNativeColor);

    drawDisableLines(painter,getLeftDisableRect(),gc,Qt::Horizontal);
    drawDisableLines(painter,getRightDisableRect(),gc,Qt::Horizontal);
    drawDisableLines(painter,getTopDisableRect(),gc,Qt::Vertical);
    drawDisableLines(painter,getBottomDisableRect(),gc,Qt::Vertical);


//    auto topleft = getExternalRectTopLeftPoint()+QPointF(mDisableRectRates[Qt::AlignLeft]*diameter,mDisableRectRates[Qt::AlignTop]*diameter);
//    auto validSize = QSizeF(diameter*(1.0-mDisableRectRates[Qt::AlignLeft]-mDisableRectRates[Qt::AlignRight]),
//                            diameter*(1.0-mDisableRectRates[Qt::AlignTop]-mDisableRectRates[Qt::AlignBottom]));
//    auto validRect = QRectF(topleft,validSize);
//    auto innerRect = getCircleInnerRect(); // 圆内接正方形
//    if (innerRect.contains(validRect))
//        painter.drawRect(validRect);
//    else painter.drawRect(innerRect);

    auto left = getLeftTrianglePoints();
    auto right = getRightTrianglePoints();
    auto top = getTopTrianglePoints();
    auto bottom = getBottomTrianglePoints();

    painter.drawPolygon(left);
    painter.drawPolygon(right);
    painter.drawPolygon(top);
    painter.drawPolygon(bottom);

    // 绘制点击三角形高亮又取消的效果
    if (!mViewInfo[HoleGroupNameField].toString().isEmpty()) {
        if (isHighlight) {
            path.clear();
            if (left.containsPoint(mMousePos,Qt::WindingFill))
                path.addPolygon(left);
            else if (right.containsPoint(mMousePos,Qt::WindingFill))
                path.addPolygon(right);
            else if (top.containsPoint(mMousePos,Qt::WindingFill))
                path.addPolygon(top);
            else if (bottom.containsPoint(mMousePos,Qt::WindingFill))
                path.addPolygon(bottom);

            painter.fillPath(path,highcolor);
        }
    }
    else {
        path.clear();
        path.addPolygon(left);
        path.addPolygon(right);
        path.addPolygon(bottom);
        path.addPolygon(top);
        painter.fillPath(path,gc);
    }
    event->accept();
}

QRectF View::getLeftDisableRect() const
{
    auto topleft = getExternalRectTopLeftPoint();
    auto diameter = getCircleRadius() * 2.0;
    auto rect = QRectF(topleft,QSizeF(diameter*mDisableRectRates[Qt::AlignLeft],diameter));
    return rect;
}

QRectF View::getRightDisableRect() const
{
    auto diameter = getCircleRadius() * 2.0;
    auto topleft = getExternalRectTopRightPoint()-QPointF(diameter*mDisableRectRates[Qt::AlignRight],0.0);
    auto right_disable_rect = QRectF(topleft,QSizeF(diameter*mDisableRectRates[Qt::AlignRight],diameter));
    return right_disable_rect;
}

QRectF View::getTopDisableRect() const
{
    auto diameter = getCircleRadius() * 2.0;
    auto topleft = getExternalRectTopLeftPoint();
    auto top_disable_rect = QRectF(topleft,QSizeF(diameter,diameter*mDisableRectRates[Qt::AlignTop]));
    return top_disable_rect;
}

QRectF View::getBottomDisableRect() const
{
    auto diameter = getCircleRadius() * 2.0;
    auto topleft = getExternalRectBottomLeftPoint() - QPointF(0.0,diameter*mDisableRectRates[Qt::AlignBottom]);
    auto bottom_disable_rect = QRectF(topleft,QSizeF(diameter,diameter*mDisableRectRates[Qt::AlignBottom]));
    return bottom_disable_rect;
}

void View::drawDisableLines(QPainter& painter,const QRectF& rect,const QColor& color,Qt::Orientation oriention)
{
    //auto diameter = getCircleRadius() * 2.0;
    painter.fillRect(rect,color);

//    if (!rect.isEmpty()) { // 画8条斜线+3条直线
//            if (oriention == Qt::Horizontal)  {
//                for(int i = 0; i < 4; ++i ){
//                    painter.drawLine(rect.topRight()+QPointF(0.0,diameter/4.0 * i),rect.topLeft()+QPointF(0.0,diameter/4.0*(i+1)));
//                    painter.drawLine(rect.topLeft()+QPointF(0.0,diameter/4.0 * i),rect.topRight()+QPointF(0.0,diameter/4.0*(i+1)));
//                    if (i > 0) {
//                        painter.drawLine(rect.topLeft()+QPointF(0.0,diameter/4.0 * i),rect.topRight()+QPointF(0.0,diameter/4.0*i));
//                    }
//                }
//            } else {
//                for(int i = 0; i < 4; ++i) {
//                    painter.drawLine(rect.topRight()-QPointF(diameter/4.0 * i,0.0),rect.bottomRight()-QPointF(diameter/4.0*(i+1),0.0));
//                    painter.drawLine(rect.topLeft() + QPointF(diameter / 4.0 * i, 0.0),
//                                     rect.bottomLeft() + QPointF(diameter / 4.0 * (i + 1), 0.0));
//                    if (i > 0) {
//                        painter.drawLine(rect.topLeft()+QPointF(diameter/4.0 * i,0.0),rect.bottomLeft()+QPointF(diameter/4.0 * i,0.0));
//                    }
//                }
//            }
//        painter.drawLine(rect.topRight()+QPointF(0.0,diameter/4.0 * 0.0),rect.topLeft()+QPointF(0.0,diameter/4.0*1.0));
//        painter.drawLine(rect.topRight()+QPointF(0.0,diameter/4.0 * 1.0),rect.topLeft()+QPointF(0.0,diameter/4.0*2.0));
//        painter.drawLine(rect.topRight()+QPointF(0.0,diameter/4.0 * 2.0),rect.topLeft()+QPointF(0.0,diameter/4.0*3.0));
//        painter.drawLine(rect.topRight()+QPointF(0.0,diameter/4.0 * 3.0),rect.topLeft()+QPointF(0.0,diameter/4.0*4.0));
//
//        painter.drawLine(rect.topLeft()+QPointF(0.0,diameter/4.0 * 0.0),rect.topRight()+QPointF(0.0,diameter/4.0*1.0));
//        painter.drawLine(rect.topLeft()+QPointF(0.0,diameter/4.0 * 1.0),rect.topRight()+QPointF(0.0,diameter/4.0*2.0));
//        painter.drawLine(rect.topLeft()+QPointF(0.0,diameter/4.0 * 2.0),rect.topRight()+QPointF(0.0,diameter/4.0*3.0));
//        painter.drawLine(rect.topLeft()+QPointF(0.0,diameter/4.0 * 3.0),rect.topRight()+QPointF(0.0,diameter/4.0*4.0));
//
//        painter.drawLine(rect.topLeft()+QPointF(0.0,diameter/4.0 * 1.0),rect.topRight()+QPointF(0.0,diameter/4.0*1.0));
//        painter.drawLine(rect.topLeft()+QPointF(0.0,diameter/4.0 * 2.0),rect.topRight()+QPointF(0.0,diameter/4.0*2.0));
//        painter.drawLine(rect.topLeft()+QPointF(0.0,diameter/4.0 * 3.0),rect.topRight()+QPointF(0.0,diameter/4.0*3.0));
//    }
}

QRectF View::getCircleInnerRect() const
{ // 圆内接正方形
    auto radius = getCircleRadius();
    auto center = QPointF(width()/2.0,height()/2.0);
    auto topleft = center - QPointF(sqrt(2)/2*radius, sqrt(2)/2*radius);

    return QRectF(topleft,QSizeF(sqrt(2)*radius, sqrt(2)*radius));
}

QRectF View::getCircleExternalRect() const
{ // 圆外接正方形
    return QRectF(getExternalRectTopLeftPoint(),QSizeF(2*getCircleRadius(),2*getCircleRadius()));
}

bool View::isValidPoint(const QPointF &point) const
{ // 判断点是否在圆内依赖于直径,且不在禁止区域
    auto center_x = width() / 2.0;
    auto center_y = height() / 2.0;

    if ((point.x()-center_x)*(point.x()-center_x) + (point.y()-center_y)*(point.y()-center_y)
        > getCircleRadius()*getCircleRadius()) { // 距离大于半径认为在圆外
        return false;
    }

    // 点在禁止区域内也无效
    if (getLeftDisableRect().contains(point) || getRightDisableRect().contains(point)
        || getTopDisableRect().contains(point) || getBottomDisableRect().contains(point)) {
        return false;
    }
    return true;
}

bool View::isValidRect(const QRectF &rect) const
{ // 区域的话检测4个端点: 在圆外的充要条件: 4个顶点到圆心的距离最小的也大于半径
    // 或者说4个端点都在圆内必定区域在圆内
    QPointFVector points = {rect.topLeft(),rect.topRight(),rect.bottomLeft(),rect.bottomRight()};
    for(auto pt: points) {
        if (!isValidPoint(pt))
            return false;
    }

    // 和禁止区域有交集也无效
    if (getLeftDisableRect().intersects(rect) || getRightDisableRect().intersects(rect)
        || getTopDisableRect().intersects(rect) || getBottomDisableRect().intersects(rect)) {
        return false;
    }
    return true; //
}

double View::getCircleRadius() const
{ // 视野圆半径
    return width()>=height()?(height()-trianglen*2)/2.0:(width()-trianglen*2)/2.0;
    //return width()>=height()?height()/2.0:width()/2.0;
}

double View::getInnerRectWidth() const
{// 小矩形区域的宽度
#ifdef viewRowColUnEqual
    return 2.0 * getCircleRadius() /  mDimension.cols *1.0;
#else
    return 2.0 * getCircleRadius() /  mSize *1.0;
#endif
}

double View::getInnerRectHeight() const
{// 小矩形区域的高度
#ifdef viewRowColUnEqual
    return 2.0 * getCircleRadius() / mDimension.rows *1.0;
#else
    return 2.0 * getCircleRadius() / mSize *1.0;
#endif
}

QPointF View::getExternalRectTopLeftPoint() const
{// 外接正方形左上角顶点
    if (width()>=height())
        return QPointF(width()/2.0-getCircleRadius(),trianglen);
    else
        return QPointF(trianglen,height()/2.0-getCircleRadius()); // 左上角x=0
}

QPointF View::getExternalRectTopRightPoint() const
{// 外接正方形右上角顶点
    if (width()>=height())
        return QPointF(width()/2.0+getCircleRadius(),trianglen);
    else
        return QPointF(width()-trianglen,height()/2.0-getCircleRadius());
}

QPointF View::getExternalRectBottomLeftPoint() const
{// 外接正方形左下角顶点
    if (width()>=height())
        return QPointF(width()/2.0-getCircleRadius(),height()-trianglen);
    else
        return QPointF(trianglen,height()/2.0+getCircleRadius());
}

QPointF View::getExternalRectBottomRightPoint() const
{// 外接正方形右下角顶点
    if (width()>=height())
        return QPointF(width()/2.0+getCircleRadius(),height()-trianglen);
    else
        return QPointF(width()-trianglen,height()/2.0+getCircleRadius());
}

QRectF View::getTriangleInnerRect() const
{
    return QRectF(trianglen,trianglen,width()-2*trianglen,height()-2*trianglen);
}

QPolygonF View::getLeftTrianglePoints() const
{
    //auto ref_pt = getTriangleInnerRect().topLeft();
    auto ref_pt = getExternalRectTopLeftPoint();

    auto vertex = QPointF(ref_pt.x()-trianglen*sqrt(3)/2,height()/2);
    auto p_top = QPointF(ref_pt.x(),height()/2-trianglen/2);
    auto p_bottom = QPointF(ref_pt.x(),height()/2+trianglen/2);

    return QPolygonF(QPointFVector()<<vertex<<p_top<<p_bottom);
}

QPolygonF View::getRightTrianglePoints() const
{
    //auto ref_pt = getTriangleInnerRect().topRight();
    auto ref_pt = getExternalRectTopRightPoint();

    auto vertex = QPointF(ref_pt.x()+trianglen*sqrt(3)/2,height()/2);
    auto p_top = QPointF(ref_pt.x(),height()/2-trianglen/2);
    auto p_bottom = QPointF(ref_pt.x(),height()/2+trianglen/2);

    return QPolygonF(QPointFVector()<<vertex<<p_top<<p_bottom);
}

QPolygonF View::getTopTrianglePoints() const
{
    //auto ref_pt = getTriangleInnerRect().topLeft();
    auto ref_pt = getExternalRectTopLeftPoint();

    auto vertex = QPointF(width()/2,ref_pt.y()-trianglen*sqrt(3)/2);
    auto p_left = QPointF(width()/2-trianglen/2,ref_pt.y());
    auto p_right = QPointF(width()/2+trianglen/2,ref_pt.y());

    return QPolygonF(QPointFVector()<<vertex<<p_left<<p_right);
}

QPolygonF View::getBottomTrianglePoints() const
{
    //auto ref_pt = getTriangleInnerRect().bottomLeft();
    auto ref_pt = getExternalRectBottomLeftPoint();

    auto vertex = QPointF(width()/2,ref_pt.y()+trianglen*sqrt(3)/2);
    auto p_left = QPointF(width()/2-trianglen/2,ref_pt.y());
    auto p_right = QPointF(width()/2+trianglen/2,ref_pt.y());

    return QPolygonF(QPointFVector()<<vertex<<p_left<<p_right);
}

bool View::checkField() const
{   // 避免bug或者漏掉信息,审查mViewInfo的字段是否完整
    // wellpattern组装6个信息,viewpattern自行组装4个信息,共10个,其中HoleViewRectsField其实不需要了
    // 但是为了兼容老接口,先如此保留,绘图时不使用HoleViewRectsField的信息去绘制了
    auto fields = mViewInfo.keys();

    bool r1 = fields.contains(HoleGroupNameField); // 组名称
    bool r2 = fields.contains(HoleGroupColorField);// 组颜色
    bool r3 = fields.contains(HoleAllGroupsField); // 所有组名
    bool r4 = fields.contains(HoleCoordinateField); // 当前孔坐标
    bool r5 = fields.contains(HoleAllCoordinatesField); // 所有孔坐标
    bool r6 = fields.contains(HoleGroupCoordinatesField); // 本组的所有孔坐标
    bool r7 = fields.contains(HoleViewSizeField);// 视野内部划分尺寸
    bool r8 = fields.contains(HoleViewRectsField); // 视野区域
    bool r9 = fields.contains(HoleViewUiPointsField); // 视野掩码映射的坐标信息
    bool ra = fields.contains(HoleViewPointsField); // 视野掩码映射的坐标信息

    Q_ASSERT(r1 == true);
    Q_ASSERT(r2 == true);
    Q_ASSERT(r3 == true);
    Q_ASSERT(r4 == true);
    Q_ASSERT(r5 == true);
    Q_ASSERT(r6 == true);
    Q_ASSERT(r7 == true);
    Q_ASSERT(r8 == true);
    Q_ASSERT(r9 == true);
    Q_ASSERT(ra == true);

    return r1&&r2&&r3&&r4&&r5&&r6&&r7&&r8&&r9&&ra;
}

void View::setDisableRect(Qt::Alignment direction, double rate)
{
    if (direction != Qt::AlignBottom && direction != Qt::AlignTop
        && direction != Qt::AlignLeft && direction != Qt::AlignRight) {
        return;
    }

    mDisableRectRates[direction] = rate;
    update();
}

void View::setDisableRect(double rate)
{
    Q_ASSERT(rate<1.0);
    Q_ASSERT(rate>=0.0);
    mDisableRectRates[Qt::AlignLeft] = rate;
    mDisableRectRates[Qt::AlignRight] = rate;
    mDisableRectRates[Qt::AlignBottom] = rate;
    mDisableRectRates[Qt::AlignTop] = rate;
    update();
}

void View::setDisableRect(const QMap<Qt::Alignment, double> &map)
{ // 外部有义务传递正确的map
    Q_ASSERT(map.keys().count() == 4);
    Q_ASSERT(map.keys().contains(Qt::AlignTop));
    Q_ASSERT(map.keys().contains(Qt::AlignLeft));
    Q_ASSERT(map.keys().contains(Qt::AlignBottom));
    Q_ASSERT(map.keys().contains(Qt::AlignRight));
    for(auto key: map.keys()) {
        Q_ASSERT(map[key]<1.0);
        Q_ASSERT(map[key]>=0.0);
    }
    mDisableRectRates = map;
    update();
}

void View::clearDisableRect()
{
    mDisableRectRates[Qt::AlignLeft] = 0.0;
    mDisableRectRates[Qt::AlignRight] = 0.0;
    mDisableRectRates[Qt::AlignBottom] = 0.0;
    mDisableRectRates[Qt::AlignTop] = 0.0;
    update();
}

void View::onApplyAllAct()
{

}

void View::onApplyGroupAct()
{

}

void View::onApplyHoleAct()
{

}

void View::onRemoveViewAct()
{

}

void View::onSaveViewAct()
{

}
