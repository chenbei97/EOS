/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-05 09:15:24
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-05 09:15:40
 * @FilePath: \EOS\component\src\widgets\view.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "view.h"

View::View(QWidget *parent) : QWidget(parent)
{
    initDispersedMask();
    saveviewact = new QAction(tr("Selecting Points"));
    removeviewact = new QAction(tr("Remove Points"));
    applyholeact = new QAction(tr("Apply To Hole"));
    applygroupact = new QAction(tr("Apply To Group"));
    applyallact = new QAction(tr("Apply To All"));
    addAction(saveviewact);
    addAction(removeviewact);
    //addAction(applyholeact); // 不显式添加
    addAction(applygroupact);
    addAction(applyallact);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(saveviewact,&QAction::triggered,this,&View::onSaveViewAct);
    connect(removeviewact,&QAction::triggered,this,&View::onRemoveViewAct);
    connect(applyholeact,&QAction::triggered,this,&View::onApplyHoleAct);
    connect(applygroupact,&QAction::triggered,this,&View::onApplyGroupAct);
    connect(applyallact,&QAction::triggered,this,&View::onApplyAllAct);
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

QPointF View::mapFromPointF(const QPointF &point) const
{ // 把坐标归一化到0-1
    auto pos = point-getInnerRectTopLeftPoint();
    //LOG<<point<<getInnerRectTopLeftPoint()<<pos;
    pos = pos / (getCircleRadius() * 2.0);
    return pos;
}

QPointF View::mapToPointF(const QPointF &point) const
{ // 把0-1的坐标放大到ui尺寸上
    auto pos = QPointF(point.x() * getCircleRadius() * 2.0,
                       point.y() * getCircleRadius() * 2.0);

    pos = pos+getInnerRectTopLeftPoint();
    return pos;
}

QRectF View::mapToSize(const QRectF &source,const QPointF&ref_point,int ref_w,int ref_h)
{ // source已经是0-1的等比例缩放了
    auto topleft = source.topLeft();
    auto size = source.size();

    auto x = topleft.x() * ref_w + ref_point.x();// 左上角顶点(0.2,0.3),长度(0.25,0.35)
    auto y = topleft.y() * ref_h + ref_point.y();// 放大到尺寸为(ref_w,ref_h),例如100
    auto w = size.width() * ref_w; // 那么结果就是(20,30,25,35),如果左上角参考点还需要移动就变成 (30,50,25,35)
    auto h = size.height() * ref_h;

    return QRectF(x,y,w,h);
}

QRectF View::mapFromSize(const QRectF &source,const QPointF&ref_point,int ref_w,int ref_h)
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
    mMouseRect = QRectF();
    mDrapRectF = QRectF();

    auto size =  mViewInfo[HoleViewSizeField].toInt();

    mSize = size;
    update();
}

ViewInfo View::viewInfo() const
{
    return mViewInfo;
}

int View::holeID() const
{ // 每个孔双击打开视野窗口都是一对一的
    auto coordinate = mViewInfo[HoleCoordinateField].toPoint();
    if (coordinate.isNull()) return -1;

    auto id = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
    return id;
}

int View::holeID(const QPoint& holePoint) const
{
    return holePoint.x() * PointToIDCoefficient + holePoint.y();
}

void View::mousePressEvent(QMouseEvent *event)
{
    mMousePos = event->pos();
    if (!getValidRect().contains(mMousePos)) // 如果不是有效区域的鼠标点击无效
        mMousePos = {-1.0,-1.0};

    // 1.框选后随机点一下要清除框选框(只能拖拽生成),同时出现单击框
    if (event->button() == Qt::LeftButton) {
        mDrapRectF = QRectF();

        auto diameter = width()>=height()?height():width();
        auto mouseRect_topleft = mMousePos-QPointF(getInnerRectWidth()/2,getInnerRectHeight()/2);
        auto mouseRect_bottomright = mMousePos+QPointF(getInnerRectWidth()/2,getInnerRectHeight()/2);
        if (getValidRect().contains(mouseRect_topleft) && getValidRect().contains(mouseRect_topleft)) {
            mMouseRect = mapFromSize(QRectF(mouseRect_topleft,mouseRect_bottomright),
                                     getInnerRectTopLeftPoint(),diameter,diameter);
        }
        update();
    } else if (event->button() == Qt::RightButton) {
        if (mViewInfo[HoleGroupNameField].toString().isEmpty()
            || mMousePos == QPointF(-1.0, -1.0)) {
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
    // 将鼠标坐标映射为相对圆外接正方形左上角的相对坐标,并将其归一化
    if (mMousePos != QPointF(-1.0,-1.0)) {
        auto pos = mapFromPointF(mMousePos);
        emit previewEvent(pos);
    }
    update();
    event->accept();
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
   event->accept();
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton){
        auto end = event->pos(); // 鼠标停下的点
        if (!getValidRect().contains(mMousePos) || !getValidRect().contains(end))
            return; // 框选到无效区域
        mMouseRect = QRectF();
        auto diameter = width()>=height()?height():width();
        // 鼠标形成的矩形框将其等比例缩放到0-1
        mDrapRectF = mapFromSize(QRectF(mMousePos,end),
                                 getInnerRectTopLeftPoint(),diameter,diameter);
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
    //painter.drawRect(rect());
    auto radius = width()>=height()?height()/2.0:width()/2.0;
    painter.drawEllipse(QPointF(width()/2.0,height()/2.0),radius,radius);
    event->accept();
}

QRectF View::getValidRect() const
{ //有效的区域是整个圆和外接正方形,点在外边不认为有效,防止用户把viewwindow窗口拉伸后点
    return QRectF(getInnerRectTopLeftPoint(),QSize(getCircleRadius()*2.0,getCircleRadius()*2.0));
}

double View::getCircleRadius() const
{ // 视野圆半径
    return width()>=height()?height()/2.0:width()/2.0;
}

double View::getInnerRectWidth() const
{// 小矩形区域的宽度
    return 2.0 * getCircleRadius() /  mSize *1.0;
}

double View::getInnerRectHeight() const
{// 小矩形区域的高度
    return 2.0 * getCircleRadius() / mSize *1.0;
}

QPointF View::getInnerRectTopLeftPoint() const
{// 外接正方形左上角顶点
    if (width()>=height())
        return QPointF(width()/2.0-getCircleRadius(),0.0);
    else
        return QPointF(0.0,height()/2.0-getCircleRadius()); // 左上角x=0
}

QPointF View::getInnerRectTopRightPoint() const
{// 外接正方形右上角顶点
    if (width()>=height())
        return QPointF(width()/2.0+getCircleRadius(),0.0);
    else
        return QPointF(width(),height()/2.0-getCircleRadius());
}

QPointF View::getInnerRectBottomLeftPoint() const
{// 外接正方形左下角顶点
    if (width()>=height())
        return QPointF(width()/2.0-getCircleRadius(),height());
    else
        return QPointF(0,height()/2.0+getCircleRadius());
}

QPointF View::getInnerRectBottomRightPoint() const
{// 外接正方形右下角顶点
    if (width()>=height())
        return QPointF(width()/2.0+getCircleRadius(),height());
    else
        return QPointF(width(),height()/2.0+getCircleRadius());
}

bool View::checkField() const
{   // 避免bug或者漏掉信息,审查mViewInfo的字段是否完整
    // wellpattern组装6个信息,viewpattern自行组装4个信息,共10个,其中HoleViewRectsField其实不需要了
    // 但是为了兼容老接口,先如此保留,绘图时不使用HoleViewRectsField的信息去绘制了
    auto fields = mViewInfo.keys();

    bool r1 = fields.contains(HoleGroupNameField); // 组名称
    bool r2 = fields.contains(HoleGroupColorField);// 组颜色
    bool r3 = fields.contains(WellAllGroupsField); // 所有组名
    bool r4 = fields.contains(HoleCoordinateField); // 当前孔坐标
    bool r5 = fields.contains(WellAllHolesField); // 所有孔坐标
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

void View::setDisablePoint(QCPoint point, bool enable)
{


}

void View::setDisablePoints(QCPointVector points, bool enable)
{

}

void View::setDisablePoints(bool enable)
{

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
