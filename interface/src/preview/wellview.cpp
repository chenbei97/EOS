/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-25 09:48:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 09:49:29
 * @FilePath: \EOS\interface\src\preview\viewpattern.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#include "wellview.h"

void WellView::removeHole(const QPoint &holePoint)
{// 删孔时清除该孔对应的视野信息
    auto id = holeID(holePoint);
    mDrapRectF = QRectF();
    mViewRects[id].clear();
    mTmpRects[id].clear();
    mViewRectDispersedPoints[id].clear();
    mTmpRectDispersedPoints[id].clear();
    mViewPoints[id].clear();
    mTmpPoints[id].clear();
    mViewMachinePoints.clear();
    initDispersedMask();

    // 颜色+当前孔坐标+当前视野尺寸 无需清理（以下代码其实不清理也行,wellpattern删孔逻辑内重新触发了openviewwindow刷新了）
    mViewInfo[HoleGroupNameField] = "";
    mViewInfo[WellAllHolesField].setValue(QPoint2DVector());
    mViewInfo[HoleGroupCoordinatesField].setValue(QPointVector());
    mViewInfo[WellAllGroupsField].setValue(QSet<QString>());
    mViewInfo[HoleViewRectsField].setValue(QRectFVector());
    mViewInfo[HoleViewUiPointsField].setValue(QPointFVector());
    mViewInfo[HoleViewPointsField].setValue(QPointFVector());
    checkField();

    applyallact->setEnabled(false);
    applygroupact->setEnabled(false);
    removeviewact->setEnabled(false);
    saveviewact->setEnabled(false);

    update();
}

void WellView::toggleBrandObjective(int viewSize,bool toggleObjective)
{// 切换厂家或者物镜倍数时把所有的区域信息都要清空,对于mViewInfo只需要清理视野相关的2个信息
    mDrapRectF = QRectF();
    mViewRects.clear();
    mTmpRects.clear();
    mViewRectDispersedPoints.clear();
    mTmpRectDispersedPoints.clear();
    mViewPoints.clear();
    mTmpPoints.clear();
    initDispersedMask(); // 这个掩码矩阵也要清理
    mViewMachinePoints.clear(); // 电机坐标清理

    // 视野的尺寸发生了变化,才清理掉视野的4个信息,否则不用动
    if (viewSize != mViewInfo[HoleViewSizeField].toInt()) {
        mViewInfo[HoleViewSizeField] = viewSize;
        mViewInfo[HoleViewRectsField].setValue(QRectFVector()); // 清空视野信息
        mViewInfo[HoleViewUiPointsField].setValue(QPointFVector());
        mViewInfo[HoleViewPointsField].setValue(QPointFVector());
    } // 其它与孔有关的信息只有setViewInfo时才会传递,如果清除了就会丢掉这些信息
    // 如果是切物镜,wellpattern没有清理掉well相关的信息,view也不需要清除
    // 如果是切厂家,wellpattern完全重置,view也必须清理掉这些信息,否则信息传递的一些Q_Assert不成立
    if (!toggleObjective) {
        mViewInfo[HoleGroupColorField] = QColor(Qt::red);
        mViewInfo[HoleGroupNameField] = "";
        mViewInfo[WellAllHolesField].setValue(QPoint2DVector());
        mViewInfo[HoleGroupCoordinatesField].setValue(QPointVector());
        mViewInfo[WellAllGroupsField].setValue(QSet<QString>());
    }

    //m[HoleCoordinateField] = mViewInfo[HoleCoordinateField]; // 坐标信息顺带组装
    //m[WellAllGroupsField] = mViewInfo[WellAllGroupsField]; // 孔板所有组名信息顺带组装
    // HoleGroupCoordinatesField 该组的所有孔坐标不组装
    mSize = viewSize;
    update();
}

void WellView::importViewInfo(QCPoint holePoint, QCPointFVector viewPoints,int viewSize)
{// 导入实验配置时去更新view的ui信息(和setViewInfo初始化的代码差不多)
    mViewInfo[HoleCoordinateField] = holePoint;
    mViewInfo[HoleGroupNameField] = "";
    mViewInfo[HoleGroupColorField] = QColor(Qt::red);

    mViewInfo[HoleGroupCoordinatesField].setValue(QPointVector());
    mViewInfo[WellAllGroupsField].setValue(QSet<QString>());
    mViewInfo[WellAllHolesField].setValue(QPoint2DVector());

    mViewInfo[HoleViewSizeField] = viewSize;
    mViewInfo[HoleViewRectsField].setValue(QRectFVector());
    mViewInfo[HoleViewUiPointsField].setValue(QPointFVector());
    mViewInfo[HoleViewPointsField].setValue(QPointFVector());

    mMousePos = {-1.0,-1.0};
    mMouseRect = QRectF();
    mDrapRectF = QRectF();
    mSize = viewSize;

    auto id = holeID();

    mViewRectDispersedPoints[id].clear();
    mTmpRectDispersedPoints[id].clear();
    mViewRects[id].clear();
    mTmpRects[id].clear();

    for(auto pt: viewPoints) { // 电机坐标要转成UI区域,然后转成UI坐标
        mViewRects[id].append(QRectF(QRectF(pt.x(),pt.y(),
               1.0/mSize,1.0/mSize)));// 这个坐标当初导出时根据每个小视野的尺寸为单位导出,小视野的比例就是1/mSize
    }
    mTmpRects[id] = mViewRects[id];

    for(int r = 0; r < mDispersedMaskSize; ++r) {
        for (int c = 0; c < mDispersedMaskSize; ++c) {
            for(auto viewRect: mViewRects[id]) {
                auto rect = mapToSize(viewRect,QPointF(0.0,0.0),
                                      mDispersedMaskSize,mDispersedMaskSize); // 等比例放大尺寸到mDispersedMaskSize
                if (rect.intersects(QRectF(c,r,1.0,1.0))) {
                    mViewRectDispersedPoints[id].append(QPointF((r+0.5)/mDispersedMaskSize,(c+0.5)/mDispersedMaskSize));
                }
            }  // end 2层for
        }
    }

    mTmpRectDispersedPoints[id] = mViewRectDispersedPoints[id];
    applyholeact->trigger();
    update();
}

void WellView::onApplyAllAct()
{
    checkField();
    if (mViewInfo[HoleGroupNameField].toString().isEmpty())
        return;

    auto id = holeID();
    QVariantMap m;
    m[HoleViewSizeField] = mViewInfo[HoleViewSizeField]; // 组装视野窗口尺寸
    if (mSelectMode == RectMode) {
        m[HoleViewRectsField].setValue(mViewRects[id]); // 视野窗口的区域信息
        m[HoleViewUiPointsField].setValue(mViewRectDispersedPoints[id]);
    }
    m[HoleViewPointsField].setValue(mViewMachinePoints);
    m[HoleGroupColorField] = mViewInfo[HoleGroupColorField]; // 组装组颜色,可以把所有组颜色统一(可能没分过组默认颜色红色,无所谓)
    m[HoleGroupNameField] = mViewInfo[HoleGroupNameField];// 组装组名信息(应用到所有组时只有同组的组颜色需要覆盖)
    m[WellAllHolesField] = mViewInfo[WellAllHolesField]; // 孔板所有选择的孔坐标信息顺带组装
    //m[HoleCoordinateField] = mViewInfo[HoleCoordinateField]; // 坐标信息顺带组装
    //m[WellAllGroupsField] = mViewInfo[WellAllGroupsField]; // 孔板所有组名信息顺带组装
    // HoleGroupCoordinatesField 该组的所有孔坐标不组装
    emit applyAllEvent(m);// 5个信息足够

    auto allholes = mViewInfo[WellAllHolesField].value<QPoint2DVector>();//拿到所有分过组的孔坐标
    foreach(auto holes, allholes) {
        foreach (auto hole, holes) {
            auto pt_idx = holeID(hole);// 所有其他孔的临时数据区更新为当前孔的视野信息
            if (mSelectMode == RectMode) {
                mViewRects[pt_idx] = mViewRects[id];
                mTmpRects[pt_idx] = mViewRects[id];
                mViewRectDispersedPoints[pt_idx] = mViewRectDispersedPoints[id];
                mTmpRectDispersedPoints[pt_idx] = mViewRectDispersedPoints[id];
            } else {
                mViewPoints[pt_idx] = mViewPoints[id];
                mTmpPoints[pt_idx] = mViewPoints[id];
            }

        }
    }
    update();
}

void WellView::onApplyGroupAct()
{
    checkField();
    if (mViewInfo[HoleGroupNameField].toString().isEmpty())
        return;

    auto id = holeID();
    QVariantMap m;
    m[HoleGroupNameField] = mViewInfo[HoleGroupNameField];// 组装组名称,方便pattern依据组名查找所有孔
    m[HoleGroupColorField] = mViewInfo[HoleGroupColorField]; // 组装组颜色,可以让pattern把同组内其他可能不相同的颜色全部统一
    m[HoleCoordinateField] = mViewInfo[HoleCoordinateField]; // 坐标信息顺带组装
    m[WellAllGroupsField] = mViewInfo[WellAllGroupsField]; // 孔板所有组名信息顺带组装
    m[WellAllHolesField] = mViewInfo[WellAllHolesField]; // 孔板所有选择的孔坐标信息顺带组装
    m[HoleViewSizeField] = mViewInfo[HoleViewSizeField]; // 组装视野窗口尺寸
    // HoleGroupCoordinatesField 该组的所有孔坐标不组装
    if (mSelectMode == RectMode) {
        m[HoleViewRectsField].setValue(mViewRects[id]); // 视野窗口的区域信息
        m[HoleViewUiPointsField].setValue(mViewRectDispersedPoints[id]);
    }
    m[HoleViewPointsField].setValue(mViewMachinePoints);
    emit applyGroupEvent(m);

    auto holeCoordinates = mViewInfo[HoleGroupCoordinatesField].value<QPointVector>();//拿到本组其它孔的所有孔坐标
    foreach(auto pt, holeCoordinates) {
        auto pt_idx = holeID(pt);// 本组其他孔的临时数据区更新为当前孔的视野信息
        if (mSelectMode == RectMode) {
            mTmpRects[pt_idx] = mViewRects[id];
            mViewRects[pt_idx] = mViewRects[id];
            mViewRectDispersedPoints[pt_idx] = mViewRectDispersedPoints[id];
            mTmpRectDispersedPoints[pt_idx] = mViewRectDispersedPoints[id];
        } else {
            mViewPoints[pt_idx] = mViewPoints[id];
            mTmpPoints[pt_idx] = mViewPoints[id];
        }

    }
    update();
}

void WellView::onApplyHoleAct()
{
    checkField();
    if (mViewInfo[HoleGroupNameField].toString().isEmpty())
        return;

    auto id = holeID();
    QVariantMap m;
    m[HoleGroupNameField] = mViewInfo[HoleGroupNameField];// 组装组名称,方便pattern依据组名查找所有孔
    m[HoleGroupColorField] = mViewInfo[HoleGroupColorField]; // 组装组颜色,可以让pattern把同组内其他可能不相同的颜色全部统一
    m[HoleCoordinateField] = mViewInfo[HoleCoordinateField]; // 坐标信息顺带组装
    m[WellAllGroupsField] = mViewInfo[WellAllGroupsField]; // 孔板所有组名信息顺带组装
    m[WellAllHolesField] = mViewInfo[WellAllHolesField]; // 孔板所有选择的孔坐标信息顺带组装
    m[HoleViewSizeField] = mViewInfo[HoleViewSizeField]; // 组装视野窗口尺寸
    // HoleGroupCoordinatesField 该组的所有孔坐标不组装
    if (mSelectMode == RectMode) {
        m[HoleViewRectsField].setValue(mViewRects[id]); // 视野窗口的区域信息
        m[HoleViewUiPointsField].setValue(mViewRectDispersedPoints[id]);
    }
    m[HoleViewPointsField].setValue(mViewMachinePoints);
    emit applyHoleEvent(m);
}

void WellView::onRemoveViewAct()
{
    auto id = holeID();
    if (mSelectMode == RectMode) {
        // 不是原来的方式:删除添加flag为false,保存添加flag为true
        // 现在改为整体删除,那么只需要检测已有的矩形是否和框选的矩形有交集,直接删除之前保存的就可以
        int i = 0;
        if (!mDrapRectF.isEmpty()) {
            for(i = 0; i < mViewRects[id].count(); ++i) {
                if (mViewRects[id][i].intersects(mDrapRectF))
                    break;
            }
        } else {
            for(i = 0; i < mViewRects[id].count(); ++i) {
                if (mViewRects[id][i].intersects(mMouseRect))
                    break;
            }
        }
        // 已经没有了不能继续删,否则越界; 没有交集也不能删除,此时i变成mViewRects[id].count()
        if (!mViewRects[id].isEmpty() && i < mViewRects[id].count())
            mViewRects[id].remove(i);

        dispersedViewRects();
        mMouseRect = QRectF();
        mTmpRects[id] = mViewRects[id];
    } else {
        if (!mDrapRectF.isEmpty()) {
            QPointFVector points;
            for(auto pt: mViewPoints[id]) {
                if (!mDrapRectF.contains(pt)) {
                    points.append(pt);
                }
            }
            mViewPoints[id] = points;
            mTmpRects[id] = mViewRects[id];
            mViewMachinePoints = overlap(mViewPoints[id],overlapRate);
        }
    }
    applyholeact->trigger();
    update();
}

void WellView::onSaveViewAct()
{
    auto id = holeID();
    if(mSelectMode == RectMode) {
        // 框选的时候保存框选的矩形,不保存单击生成的矩形
        if (!mDrapRectF.isEmpty()) {
            if (mViewRects[id].isEmpty())
                mViewRects[id].append(mDrapRectF);
            else mViewRects[id][0] = mDrapRectF; // 但是懒得改以前的代码设计,兼容一下
        } else {
            if (mViewRects[id].isEmpty())
                mViewRects[id].append(mMouseRect);
            else mViewRects[id][0] = mMouseRect; // 变为覆盖处理
        }
        mTmpRects[id] = mViewRects[id];
        dispersedViewRects();
        mMouseRect = QRectF();
    } else {
        mViewPoints[id].append(mapFromPointF(mMousePos));
        mTmpPoints[id] = mViewPoints[id]; // 需要重叠一定比例
        mViewMachinePoints = overlap(mViewPoints[id],overlapRate);
    }
    applyholeact->trigger();
    update();
}

void WellView::dispersedViewRects()
{ // 把保存的视野区域离散化得到离散区域的所有归一化视野坐标,以及电机坐标
  // 区别在于离散电机的掩码尺寸取决于mSize,离散视野的掩码尺寸固定是DefaultUiMaskSize,后者更精细用于绘图
    auto id = holeID();
    auto viewRects = mViewRects[id];

    mViewRectDispersedPoints[id].clear();
    mTmpRectDispersedPoints[id].clear();
    mViewMachinePoints.clear();//电机坐标

    // 1. 获取离散视野坐标,和视野区域有交集就视为1
    for(int r = 0; r < mDispersedMaskSize; ++r) {
        for(int c = 0; c < mDispersedMaskSize; ++c) {
            mDispersedMask[r][c] = false; // 先清理掉否则重复添加

            for(auto viewRect: viewRects) {
                auto rect = mapToSize(viewRect,QPointF(0.0,0.0),
                                      mDispersedMaskSize,mDispersedMaskSize); // 等比例放大尺寸到mDispersedMaskSize
                if (rect.intersects(QRectF(c,r,1.0,1.0))) {
                    mDispersedMask[r][c] = true; //掩码对应位置根据flag标记是保存还是删除的区域
                }
            }  // end 2层for

            if (mDispersedMask[r][c]) { // 顺便把不为0的位置记录下来,其它地方就不再需要重复遍历了
                mViewRectDispersedPoints[id].append(QPointF((c+0.5)/mDispersedMaskSize,(r+0.5)/mDispersedMaskSize));
            }
        }
    }
    mTmpRectDispersedPoints[id] = mViewRectDispersedPoints[id];

    // 2. 获取离散电机坐标
    QPointFVector points;
    auto view_w = qCeil(getInnerRectWidth());
    auto view_h = qCeil(getInnerRectHeight());
    for(auto viewRect: viewRects) {
        auto rect = mapToSize(viewRect,QPointF(0.0,0.0),
                              getCircleRadius()*2.0,getCircleRadius()*2.0);
        auto x0 = qCeil(rect.topLeft().x()); // 像上取整
        auto y0 = qCeil(rect.topLeft().y());
        auto w = qCeil(rect.width());
        auto h = qCeil((rect.height()));
        //rect = QRectF(x0,y0,w,h); // 得到这个矩形在ui界面的实际大小

        // 现在要使用view_w,view_h(1个小视野)来遍历rect,得到所有遍历后的中心坐标
        for(int x = x0; x < x0+w ; x += view_w) {
            for(int y = y0; y < y0+h; y += view_h) {
                auto center_y = x+view_w/2.0;
                auto center_x = y+view_h/2.0;
                auto threshold = view_w*view_w/4.0+view_h*view_h/4.0;//这个可以改
                if (true) { // 如果是标记为保存的区域,2个小矩形的中心距离小于阈值就认为是靠近,只保留原来的
                    bool hasSaved = false;
                    for(int i = 0; i < points.count(); ++i) {
                        auto pt = points[i];
                        if ((pt.x()-center_x)*(pt.x()-center_x) + (pt.y()-center_y)*(pt.y()-center_y)<threshold) {
                            // 如果 2个点的距离没有超过指定阈值,认为要保存的这个点已经被保存过了
                            hasSaved = true;
                            break;
                        }
                    }
                    if (!hasSaved) { // 如果超过阈值认为是新的要保存的小矩形
                        points.append(QPointF(center_x,center_y));
                    }
                } else { // 如果标记为要删除的点(现在改了写法,没有移除区域了,else不会执行不需要删除)
                    QPointFVector holdPoints; // 保留的点
                    for(int i = 0; i < points.count(); ++i) {
                        auto pt = points[i];
                        if (!((pt.x()-center_x)*(pt.x()-center_x) + (pt.y()-center_y)*(pt.y()-center_y)<threshold)) {
                            // 如果 2个矩形中心的距离没有超过指定阈值,认为要删除这个中心点,取反也就是中心仍然要保留
                            holdPoints.append(pt);
                        }
                    }
                    points = holdPoints; // 覆盖更新
                }
            }
        }
    }

    // 3. 离散电机坐标归一化
    for(auto pt: points) {
        mViewMachinePoints.append(QPointF(pt.x()/(getCircleRadius()*2.0),pt.y()/(getCircleRadius()*2.0)));
    }

    // 4. 重叠一定比例映射
    mViewMachinePoints = overlap(mViewMachinePoints,overlapRate);

    LOG<<"ui count = "<<mViewRectDispersedPoints[id].count()
    <<" mask count = "<<mViewMachinePoints.count()<<points.count();
}

void WellView::paintEvent(QPaintEvent *event)
{
    View::paintEvent(event);
    QPainter painter(this);
    painter.drawRect(rect());
    auto pen = painter.pen();

    // 1. 变量准备
    auto radius = width()>=height()?height()/2.0:width()/2.0;
    auto diameter = 2.0 * radius;
    auto p11 = getInnerRectTopLeftPoint();
    auto p12 = getInnerRectTopRightPoint();
    auto p21 = getInnerRectBottomLeftPoint();
    auto p22 = getInnerRectBottomRightPoint();
    auto groupcolor = mViewInfo[HoleGroupColorField].toString();
    auto id = holeID();

    // 2. 绘制选中的区域,区域模式绘制区域,点模式绘制点
    if (mSelectMode == RectMode) { // 区域模式
        // 以下2种绘制法都可以,和wellpattern绘制对应,wellpattern也是2种绘制法
        // 1. 直接绘制视野区域 对应wellpattern的(3.2)画法
        if (!mViewRects[id].isEmpty()) {
            for(auto viewRect: mViewRects[id]) {
                painter.fillRect(mapToSize(viewRect,p11,diameter,diameter),groupcolor);
            }
        }

        // 2. 使用掩码矩阵点的缩放区域绘制,和wellpattern的mViewMaskSize一致就可 对应(3.4)画法
//        for(auto pt: mViewRectDispersedPoints[id]) {
//            auto w = 1.0 / mDispersedMaskSize;
//            auto h = 1.0 / mDispersedMaskSize;
//            auto viewRect = QRectF(pt.x(),pt.y(),w,h);
//            //LOG<<mapToSize(viewRect,p11,diameter,diameter);
//            painter.fillRect(mapToSize(viewRect,p11,diameter,diameter),groupcolor);
//        }

        // 3. 绘制圆,外接正方形和便于区分的灰色网格线
        painter.drawLine(p11,p21);
        painter.drawLine(p12,p22);
        painter.drawLine(p11,p12);
        painter.drawLine(p21,p22);
        pen.setWidth(1);
        pen.setColor(Qt::gray);
        painter.setPen(pen);
        auto hor_offset = getInnerRectWidth();// 绘制垂直线,2个y坐标固定
        for (int i = 1; i <= mSize-1; ++i) {
            auto top = p11 + QPointF(i*hor_offset,0);
            auto bottom = p21 + QPointF(i*hor_offset,0);
            painter.drawLine(top,bottom);
        }
        auto ver_offset = getInnerRectHeight();// 绘制水平线,2个x坐标固定
        for (int i = 1; i <= mSize-1; ++i){
            auto left = p11 + QPointF(0,ver_offset*i);
            auto right = p12 + QPointF(0,ver_offset*i);
            painter.drawLine(left,right);
        }

        pen.setWidth(DefaultPainterPenWidth);
        pen.setColor(Qt::blue);
        painter.setPen(pen);
        // 鼠标单击生成的矩形框
        painter.drawRect(mapToSize(mMouseRect,p11,diameter,diameter));
        pen.setWidth(DefaultPainterPenWidth);
        pen.setColor(Qt::black); // 恢复,否则绘制其他的都变颜色了
        painter.setPen(pen);
    } else { // 点模式
        pen.setWidth(DefaultDrawPointWidth);
        pen.setColor(groupcolor);
        painter.setPen(pen);
        for(auto pt:mViewPoints[id]) {
            painter.drawPoint(mapToPointF(pt));
        }
        pen.setColor(Qt::blue);
        painter.setPen(pen);
        if (mDrapRectF.isEmpty())
            painter.drawPoint(mMousePos); // 绘制框不出现点
        pen = painter.pen();
        pen.setWidth(DefaultPainterPenWidth);
        pen.setColor(Qt::black);
        painter.setPen(pen);
    }
    // 画圆
    painter.drawEllipse(QPointF(width()/2.0,height()/2.0),radius,radius);
    // 鼠标拖拽生成的矩形框
    pen.setWidth(DefaultPainterPenWidth);
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    if (!mDrapRectF.isEmpty()) {
        painter.drawRect(mapToSize(mDrapRectF,p11,diameter,diameter));
    }
    pen.setColor(Qt::black); // 恢复,否则绘制其他的都变颜色了
    painter.setPen(pen);
    event->accept();
}

void WellView::mousePressEvent(QMouseEvent *event)
{
    View::mousePressEvent(event);
}

void WellView::mouseReleaseEvent(QMouseEvent *event)
{
    View::mouseReleaseEvent(event);
}

void WellView::mouseMoveEvent(QMouseEvent *event)
{
    View::mouseMoveEvent(event);
}

void WellView::setSelectMode(WellView::ViewSelectMode mode)
{ // 重置选点模式效果等于切换物镜(不是厂家)
    mSelectMode = mode;
    toggleBrandObjective(mSize,true);
}

WellView::WellView(QWidget *parent) : View(parent)
{

}
