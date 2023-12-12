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
    mViewInfo[HoleAllCoordinatesField].setValue(QPoint2DVector());
    mViewInfo[HoleGroupCoordinatesField].setValue(QPointVector());
    mViewInfo[HoleAllGroupsField].setValue(QSet<QString>());
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
        mViewInfo[HoleAllCoordinatesField].setValue(QPoint2DVector());
        mViewInfo[HoleGroupCoordinatesField].setValue(QPointVector());
        mViewInfo[HoleAllGroupsField].setValue(QSet<QString>());
    }
    mSize = viewSize;
    update();
}

void WellView::adjustViewPoint(int option)
{
    Q_ASSERT(mSelectMode == PointMode);

    switch (option) {
        case 0: mValidMousePos += QPointF(-1.0,0.0);
            break;
        case 1: mValidMousePos += QPointF(0.0,-1.0);
            break;
        case 2: mValidMousePos += QPointF(1.0,0.0);
            break;
        case 3: mValidMousePos += QPointF(0.0,1.0);
            break;
    }
    update();
}

void WellView::importViewInfoV1(QCPoint holePoint, QCPointFVector viewPoints,int viewSize)
{// 导入实验配置时去更新view的ui信息(和setViewInfo初始化的代码差不多) 完全弃用
    mViewInfo[HoleCoordinateField] = holePoint;
    mViewInfo[HoleGroupNameField] = "";
    mViewInfo[HoleGroupColorField] = QColor(Qt::red);

    mViewInfo[HoleGroupCoordinatesField].setValue(QPointVector());
    mViewInfo[HoleAllGroupsField].setValue(QSet<QString>());
    mViewInfo[HoleAllCoordinatesField].setValue(QPoint2DVector());

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

void WellView::importViewInfo(const QHoleInfoVector& vec)
{ // 如果是区域模式: rect,viewpoints都有意义,但viewpoints可以通过rect得到,且更简便,不使用viewpoints
    // 如果是点模式,rect为空,使用viewpoints
    setSelectMode(ViewSelectMode::RectMode);
    mMousePos = {-1.0,-1.0};
    mValidMousePos = {-1.0,-1.0};
    mMouseRect = QRectF();
    mDrapRectF = QRectF();

    if (mSelectMode == RectMode) {
        for(auto holeInfo: vec) {

            mViewInfo[HoleCoordinateField] = holeInfo.coordinate;
            mViewInfo[HoleGroupNameField] = holeInfo.group;
            mViewInfo[HoleGroupColorField] = holeInfo.color;
            mViewInfo[HoleViewRectsField].setValue(holeInfo.viewrects);
            mViewInfo[HoleViewPointsField].setValue(holeInfo.viewpoints);
            mViewInfo[HoleViewSizeField] = holeInfo.viewsize;

            // 这3行代码只是单纯过checkField()函数,没有特定含义
            mViewInfo[HoleGroupCoordinatesField].setValue(QPointVector());
            mViewInfo[HoleAllGroupsField].setValue(QSet<QString>());
            mViewInfo[HoleAllCoordinatesField].setValue(QPoint2DVector());
            // uipoints,allgroups,allholes,groupholes都无需更新,特点都是可以自行更新
            // 其中uipoints是通过mViewRectDispersedPoints更新的
            // 在dispersedViewRects()已经完成mViewRects,mViewRectDispersedPoints,mMachinePoints的更新

            mSize = holeInfo.viewsize;
            Q_ASSERT(holeInfo.viewrects.count() == 1);
            mDrapRectF = holeInfo.viewrects[0];
            saveviewact->trigger(); // 去更新mViewRects从而绘图,同时还能映射到wellPattern
        }
    }
    else if (mSelectMode == PointMode) {

    }



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
    m[HoleAllCoordinatesField] = mViewInfo[HoleAllCoordinatesField]; // 孔板所有选择的孔坐标信息顺带组装
    //m[HoleCoordinateField] = mViewInfo[HoleCoordinateField]; // 坐标信息顺带组装
    //m[HoleAllGroupsField] = mViewInfo[HoleAllGroupsField]; // 孔板所有组名信息顺带组装
    // HoleGroupCoordinatesField 该组的所有孔坐标不组装
    emit applyAllEvent(m);// 5个信息足够

    auto allholes = mViewInfo[HoleAllCoordinatesField].value<QPoint2DVector>();//拿到所有分过组的孔坐标
    for(auto holes: allholes) {
        for(auto hole: holes) {
            auto pt_idx = holeID(hole);// 所有其他孔的临时数据区更新为当前孔的视野信息
            if (mSelectMode == RectMode) {
                mViewRects[pt_idx] = mViewRects[id];
                mTmpRects[pt_idx] = mViewRects[id];
                mViewRectDispersedPoints[pt_idx] = mViewRectDispersedPoints[id];
                mTmpRectDispersedPoints[pt_idx] = mViewRectDispersedPoints[id];
            } else if(mSelectMode == PointMode){
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
    m[HoleAllGroupsField] = mViewInfo[HoleAllGroupsField]; // 孔板所有组名信息顺带组装
    m[HoleAllCoordinatesField] = mViewInfo[HoleAllCoordinatesField]; // 孔板所有选择的孔坐标信息顺带组装
    m[HoleViewSizeField] = mViewInfo[HoleViewSizeField]; // 组装视野窗口尺寸
    // HoleGroupCoordinatesField 该组的所有孔坐标不组装
    if (mSelectMode == RectMode) {
        m[HoleViewRectsField].setValue(mViewRects[id]); // 视野窗口的区域信息
        m[HoleViewUiPointsField].setValue(mViewRectDispersedPoints[id]);
    }
    m[HoleViewPointsField].setValue(mViewMachinePoints);
    emit applyGroupEvent(m);

    auto holeCoordinates = mViewInfo[HoleGroupCoordinatesField].value<QPointVector>();//拿到本组其它孔的所有孔坐标
    for(auto pt: holeCoordinates) {
        auto pt_idx = holeID(pt);// 本组其他孔的临时数据区更新为当前孔的视野信息
        if (mSelectMode == RectMode) {
            mTmpRects[pt_idx] = mViewRects[id];
            mViewRects[pt_idx] = mViewRects[id];
            mViewRectDispersedPoints[pt_idx] = mViewRectDispersedPoints[id];
            mTmpRectDispersedPoints[pt_idx] = mViewRectDispersedPoints[id];
        } else if (mSelectMode == PointMode){
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
    m[HoleAllGroupsField] = mViewInfo[HoleAllGroupsField]; // 孔板所有组名信息顺带组装
    m[HoleAllCoordinatesField] = mViewInfo[HoleAllCoordinatesField]; // 孔板所有选择的孔坐标信息顺带组装
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
    } else if (mSelectMode == PointMode){
        if (!mDrapRectF.isEmpty()) {
            QPointFVector points;
            for(auto pt: mViewPoints[id]) {
                if (!mDrapRectF.contains(pt)) {
                    points.append(pt);
                }
            }
            mViewPoints[id] = points;
            mTmpPoints[id] = points;
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
        dispersedViewRects(); // 区域模式通过此函数内部把电机坐标算出来
        mMouseRect = QRectF();
    } else if (mSelectMode == PointMode){
        mViewPoints[id].append(mapFromPointF(mValidMousePos));
        mTmpPoints[id] = mViewPoints[id]; // 需要重叠一定比例
        // 点模式不是通过dispersedViewRects()计算,就等于选择的视野坐标
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
    auto points = getViewPoints(id);

    // 3. 重叠一定比例映射
    mViewMachinePoints = overlap(points,overlapRate);

    LOG<<"ui count = "<<mViewRectDispersedPoints[id].count()
    <<" view point count = "<<mViewMachinePoints.count();
}

QPointFVector WellView::getViewPoints(int id,bool norm) const
{ // 只用于区域模式
    Q_ASSERT(mSelectMode == RectMode);
    auto viewRects = mViewRects[id];
    auto diameter = getCircleRadius() * 2.0;
    QPointFVector points;

    auto ref = getExternalRectTopLeftPoint();
    auto view_w = qCeil(getInnerRectWidth());
    auto view_h = qCeil(getInnerRectHeight());

    for(auto viewRect: viewRects) {
        auto rect = mapToSize(viewRect,ref,diameter,diameter);
        auto x0 = qCeil(rect.topLeft().x()); // 像上取整
        auto y0 = qCeil(rect.topLeft().y());
        auto w = qCeil(rect.width());
        auto h = qCeil((rect.height()));
        //rect = QRectF(x0,y0,w,h); // 得到这个矩形在ui界面的实际大小

        // 现在要使用view_w,view_h(1个小视野)来遍历rect,得到所有遍历后的中心坐标
        for(int x = x0; x < x0+w ; x += view_w) {
            for(int y = y0; y < y0+h; y += view_h) {
                auto center_x = x+view_w/2.0;
                auto center_y = y+view_h/2.0;

                if (isValidPoint(QPointF(center_x,center_y))) { // 如果超过阈值认为是新的要保存的小矩形
                    points.append(QPointF(center_x-ref.x(),center_y-ref.y())); // 注意电机坐标要减去参考点
                }

//                auto threshold = view_w*view_w/4.0+view_h*view_h/4.0;//这个可以改
//                if (true) { // 如果是标记为保存的区域,2个小矩形的中心距离小于阈值就认为是靠近,只保留原来的
//                    bool hasSaved = false;
//                    for(int i = 0; i < points.count(); ++i) {
//                        auto pt = points[i];
//                        if ((pt.x()-center_x)*(pt.x()-center_x) + (pt.y()-center_y)*(pt.y()-center_y)<threshold) {
//                            // 如果 2个点的距离没有超过指定阈值,认为要保存的这个点已经被保存过了
//                            hasSaved = true;
//                            break;
//                        }
//                    }
//                    if (isValidPoint(QPointF(center_x,center_y)) && !hasSaved) { // 如果超过阈值认为是新的要保存的小矩形
//                        points.append(QPointF(center_x,center_y)); // 注意电机坐标要减去参考点
//                    }
//                } else { // 如果标记为要删除的点(现在改了写法,没有移除区域了,else不会执行不需要删除)
//                    QPointFVector holdPoints; // 保留的点
//                    for(int i = 0; i < points.count(); ++i) {
//                        auto pt = points[i];
//                        if (!((pt.x()-center_x)*(pt.x()-center_x) + (pt.y()-center_y)*(pt.y()-center_y)<threshold)) {
//                            // 如果 2个矩形中心的距离没有超过指定阈值,认为要删除这个中心点,取反也就是中心仍然要保留
//                            holdPoints.append(pt);
//                        }
//                    }
//                    points = holdPoints; // 覆盖更新
//                }
            }
        }
    }
    //LOG<<points;
    // 离散电机坐标归一化
    if (!norm) return points;

    QPointFVector normPoints;
    for(auto pt: points) {
        normPoints.append(QPointF(pt.x()/diameter,pt.y()/diameter));
    }
    return normPoints;
}

void WellView::paintEvent(QPaintEvent *event)
{
    View::paintEvent(event);
    QPainter painter(this);
    painter.drawRect(rect());
    auto pen = painter.pen();

    // 1. 变量准备
    auto radius = getCircleRadius();
    auto diameter = 2.0 * radius;
    auto topleft = getExternalRectTopLeftPoint();
    auto topright = getExternalRectTopRightPoint();
    auto bottomleft = getExternalRectBottomLeftPoint();
    auto bottomright = getExternalRectBottomRightPoint();
    auto rh = getInnerRectHeight();
    auto rw = getInnerRectWidth();
    auto groupcolor = mViewInfo[HoleGroupColorField].toString();
    auto id = holeID();

    // 2. 绘制选中的区域,区域模式绘制区域,点模式绘制点
    if (mSelectMode == RectMode) { // 区域模式
        // 以下2种绘制法都可以,和wellpattern绘制对应,wellpattern也是2种绘制法
        // 1. 直接绘制视野区域 对应wellpattern的(3.2)画法 最有效率
        if (!mViewRects[id].isEmpty()) {
            for(auto viewRect: mViewRects[id]) {
                painter.fillRect(mapToSize(viewRect,topleft,diameter,diameter),groupcolor);
                painter.drawText(mapToPointF(viewRect.topLeft())-QPointF(2,2),
                                 QString("(%1,%2,%3,%4)").arg(viewRect.x()).arg(viewRect.y())
                                 .arg(viewRect.width()).arg(viewRect.height()));
            }
        }

        // 2. 使用掩码矩阵点的缩放区域绘制,和wellpattern的mViewMaskSize一致就可 对应(3.4)画法
        // 没有效率不采用,导入实验配置时也是通过hole_viewrects来绘制区域而不是uipoints
//        for(auto pt: mViewRectDispersedPoints[id]) {
//            auto w = 1.0 / mDispersedMaskSize;
//            auto h = 1.0 / mDispersedMaskSize;
//            auto viewRect = QRectF(pt.x(),pt.y(),w,h);
//            //LOG<<mapToSize(viewRect,p11,diameter,diameter);
//            painter.fillRect(mapToSize(viewRect,p11,diameter,diameter),groupcolor);
//        }

        // 3. 绘制电机坐标中心和矩形区域,方便查看是否正确
        if (!mViewPoints[id].isEmpty()) {
            pen.setWidth(DefaultPainterPenWidth*2);
            pen.setColor(Qt::black);
            painter.setPen(pen);
            for(auto pt: mTmpPoints[id]) {
                auto pt_x = pt.x()*diameter+topleft.x();
                auto pt_y = pt.y()*diameter+topleft.y();
                painter.drawPoint(pt_x,pt_y);
                painter.drawText(pt_x-3,pt_y-3,QString("(%1,%2)").arg(pt.x()).arg(pt.y()));
            }
        }

        auto points = getViewPoints(id);
        if (!points.isEmpty()) {
            pen.setWidth(DefaultPainterPenWidth*2);
            pen.setColor(PurpleEA3FF7);
            painter.setPen(pen);
            for(auto pt: points) { // 注意: 要按比例放大加上参考点
                auto pt_x = pt.x()*diameter+topleft.x();
                auto pt_y = pt.y()*diameter+topleft.y();
                painter.drawPoint(pt_x,pt_y);
            }

            pen.setWidth(DefaultPainterPenWidth);
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);
            for(auto pt: points) { // 注意: 要按比例放大
                auto rec = QRectF(pt.x()*diameter+topleft.x()-rw/2.0,
                                  pt.y()*diameter+topleft.y()-rh/2.0,rw,rh);
                painter.drawRect(rec);
            }
        }

        // 4. 绘制圆,外接正方形和便于区分的灰色网格线
        painter.drawLine(topleft,bottomleft);
        painter.drawLine(topright,bottomright);
        painter.drawLine(topleft,topright);
        painter.drawLine(bottomleft,bottomright);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(DefaultPainterPenWidth/2);
        pen.setColor(Qt::gray);
        painter.setPen(pen);
        auto hor_offset = getInnerRectWidth();// 绘制垂直线,2个y坐标固定
        for (int i = 1; i <= mSize-1; ++i) {
            auto top = topleft + QPointF(i*hor_offset,0);
            auto bottom = bottomleft + QPointF(i*hor_offset,0);
            painter.drawLine(top,bottom);
        }
        auto ver_offset = getInnerRectHeight();// 绘制水平线,2个x坐标固定
        for (int i = 1; i <= mSize-1; ++i){
            auto left = topleft + QPointF(0,ver_offset*i);
            auto right = topright + QPointF(0,ver_offset*i);
            painter.drawLine(left,right);
        }

        pen.setWidth(DefaultPainterPenWidth);
        pen.setColor(Qt::blue);
        painter.setPen(pen);

        // 5.鼠标单击生成的矩形框
        painter.drawRect(mapToSize(mMouseRect,topleft,diameter,diameter));
        pen.setWidth(DefaultPainterPenWidth);
        pen.setColor(Qt::black); // 恢复,否则绘制其他的都变颜色了
        painter.setPen(pen);
    }
    else if (mSelectMode == PointMode){ // 点模式
        pen.setWidth(DefaultDrawPointWidth);
        pen.setColor(groupcolor);
        painter.setPen(pen);
        for(auto pt:mViewPoints[id]) {
            auto mpt = mapToPointF(pt);
            painter.drawPoint(mpt);
            painter.drawText(mpt.x()+3,mpt.y()-3,
                             QString("(%1,%2)").arg(pt.x()).arg(pt.y()));
        }
        pen.setColor(Qt::blue);
        painter.setPen(pen);
        if (mDrapRectF.isEmpty()) { // 绘制拖拽框不绘制点
            auto pt = mapFromPointF(mValidMousePos);
            painter.drawPoint(mValidMousePos);
            painter.drawText(mValidMousePos.x()+3,mValidMousePos.y()-3,
                             QString("(%1,%2)").arg(pt.x()).arg(pt.y()));
        }

        pen = painter.pen();
        pen.setWidth(DefaultPainterPenWidth);
        pen.setColor(Qt::black);
        painter.setPen(pen);
    } else { // wholeMode
        auto grayc = QColor(Qt::gray);
        grayc.setAlpha(DefaultColorAlpha);
        painter.fillRect(QRectF(getExternalRectTopLeftPoint(),QSize(diameter,diameter)),grayc);
    }
    // 画圆
    painter.drawEllipse(QPointF(width()/2.0,height()/2.0),radius,radius);
    painter.drawRect(getTriangleInnerRect()); // 三角内的矩形区域
    //painter.drawRect(getCircleInnerRect()); // 圆内接正方形
    painter.drawRect(getCircleExternalRect());// 圆外接正方形
    // 鼠标拖拽生成的矩形框
    pen.setWidth(DefaultPainterPenWidth);
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    if (!mDrapRectF.isEmpty()) {
        painter.drawRect(mapToSize(mDrapRectF,topleft,diameter,diameter));
    }
    pen.setColor(Qt::black); // 恢复,否则绘制其他的都变颜色了
    painter.setPen(pen);
    event->accept();
}

void WellView::mousePressEvent(QMouseEvent *event)
{
    View::mousePressEvent(event);
    if (mSelectMode == PointMode) {
        if (event->button() == Qt::LeftButton) {
            if (!mViewInfo[HoleGroupNameField].toString().isEmpty()) {
                if (getLeftTrianglePoints().containsPoint(mMousePos,Qt::WindingFill)){
                    isHighlight = true;
                    emit leftTriangleClicked();
                    emit triangleClicked(0);
                    LOG<<"left triangle is clicked";
                } else if (getRightTrianglePoints().containsPoint(mMousePos,Qt::WindingFill)) {
                    isHighlight = true;
                    emit rightTriangleClicked();
                    emit triangleClicked(2);
                    LOG<<"right triangle is clicked";
                } else if (getTopTrianglePoints().containsPoint(mMousePos,Qt::WindingFill)) {
                    isHighlight = true;
                    emit topTriangleClicked();
                    emit triangleClicked(1);
                    LOG<<"top triangle is clicked";
                } else if (getBottomTrianglePoints().containsPoint(mMousePos,Qt::WindingFill)) {
                    isHighlight = true;
                    emit bottomTriangleClicked();
                    emit triangleClicked(3);
                    LOG<<"bottom triangle is clicked";
                }
            }
        }
    }
}

void WellView::mouseReleaseEvent(QMouseEvent *event)
{
    View::mouseReleaseEvent(event);
    isHighlight = false;
    update();
}

void WellView::mouseMoveEvent(QMouseEvent *event)
{
    View::mouseMoveEvent(event);
}

void WellView::setSelectMode(WellView::ViewSelectMode mode)
{ // 切换区域模式不删除不更改wellview的任何信息
    mSelectMode = mode;

    //由于电机坐标区域模式下是离散出来的,点模式则是mViewPoints的直接赋值,故需要重新把点模式已有的电机坐标重新更新过去
    QVariantMap m; // 注意并没有改变wellview的任何信息,只更改了wellpattern的viewpoints信息
    auto allholes = mViewInfo[HoleAllCoordinatesField].value<QPoint2DVector>();
    for(auto holes: allholes) { //所有孔都要重新更新电机坐标的信息
        for (auto hole: holes) {
            auto pt_idx = holeID(hole);
            if (mSelectMode == RectMode) {
                m[HoleViewPointsField].setValue(overlap(getViewPoints(pt_idx), overlapRate));
            }
            else if (mSelectMode == PointMode) {
                m[HoleViewPointsField].setValue(overlap(mViewPoints[pt_idx],overlapRate));
            } // 只需要使用每个孔已有的信息根据不同模式重新把电机坐标算一下更新给wellpattern
            m[HoleCoordinateField] = hole; // 这个孔坐标装上是wellpattern去更新对应的孔
            emit selectModeChangedEvent(m);
        }
    }

    if (mode == WholeMode)
        setEnabled(false);
    else setEnabled(true);

    update();
}

WellView::WellView(QWidget *parent) : View(parent)
{
    connect(this,&WellView::triangleClicked,this,&WellView::adjustViewPoint);
}
