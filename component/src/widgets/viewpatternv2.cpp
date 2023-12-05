/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-05 09:52:07
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-05 09:55:19
 * @FilePath: \EOS\component\src\widgets\viewpatternv2.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "viewpatternv2.h"

namespace obsoleteClass {
    void ViewPatternV2::clearViewWindowCache(const QPoint &holePoint)
    {// 删孔时清除该孔对应的视野信息
        auto id = holeID(holePoint);
        mDrapRectF = QRectF();
        mViewRects[id].clear();
        mTmpRects[id].clear();
        mUiViewMaskNormPoints[id].clear();
        mTmpUiViewMaskNormPoints[id].clear();
        mViewMaskNormPoints.clear();
        initUiViewMask();

        // 颜色+当前孔坐标+当前视野尺寸 无需清理（以下代码其实不清理也行,wellpattern删孔逻辑内重新触发了openviewwindow刷新了）
        mViewInfo[HoleGroupNameField] = "";
        mViewInfo[WellAllHolesField].setValue(QPoint2DVector());
        mViewInfo[HoleGroupCoordinatesField].setValue(QPointVector());
        mViewInfo[WellAllGroupsField].setValue(QSet<QString>());
        mViewInfo[HoleViewRectsField].setValue(ViewRectFVector());
        mViewInfo[HoleViewUiPointsField].setValue(ViewPointVector());
        mViewInfo[HoleViewPointsField].setValue(ViewPointVector());
        checkField();

        applyallact->setEnabled(false);
        applygroupact->setEnabled(false);
        removeviewact->setEnabled(false);
        saveviewact->setEnabled(false);

        update();
    }

    void ViewPatternV2::clearAllViewWindowCache(int viewSize,bool toggleObjective)
    {// 切换厂家或者物镜倍数时把所有的区域信息都要清空,对于mViewInfo只需要清理视野相关的2个信息
        mDrapRectF = QRectF();
        mViewRects.clear();
        mTmpRects.clear();
        mUiViewMaskNormPoints.clear();
        mTmpUiViewMaskNormPoints.clear();
        mViewMaskNormPoints.clear();
        initUiViewMask(); // 这个掩码矩阵也要清理

        if (viewSize != mViewInfo[HoleViewSizeField].toInt()) {
            mViewInfo[HoleViewSizeField] = viewSize;
            mViewInfo[HoleViewRectsField].setValue(ViewRectFVector()); // 清空视野信息
            mViewInfo[HoleViewUiPointsField].setValue(ViewPointVector());
            mViewInfo[HoleViewPointsField].setValue(ViewPointVector());
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

    void ViewPatternV2::updateViewWindowCache(QCPoint holePoint, QCPointFVector viewPoints,int viewSize)
    {// 导入实验配置时去更新view的ui信息(和setViewInfo初始化的代码差不多)
        mViewInfo[HoleCoordinateField] = holePoint;
        mViewInfo[HoleGroupNameField] = "";
        mViewInfo[HoleGroupColorField] = QColor(Qt::red);

        mViewInfo[HoleGroupCoordinatesField].setValue(QPointVector());
        mViewInfo[WellAllGroupsField].setValue(QSet<QString>());
        mViewInfo[WellAllHolesField].setValue(QPoint2DVector());

        mViewInfo[HoleViewSizeField] = viewSize;
        mViewInfo[HoleViewRectsField].setValue(ViewRectFVector());
        mViewInfo[HoleViewUiPointsField].setValue(ViewPointVector());
        mViewInfo[HoleViewPointsField].setValue(ViewPointVector());

        mMousePos = {-1.0,-1.0};
        mMouseRect = QRectF();
        mDrapRectF = QRectF();
        mSize = viewSize;

        auto id = holeID();

        mUiViewMaskNormPoints[id].clear();
        mTmpUiViewMaskNormPoints[id].clear();
        mViewRects[id].clear();
        mTmpRects[id].clear();

        for(auto pt: viewPoints) { // 电机坐标要转成UI区域,然后转成UI坐标
            mViewRects[id].append(ViewRectF(QRectF(pt.x(),pt.y(),
                                                   1.0/mSize,1.0/mSize),true));// 这个坐标当初导出时根据每个小视野的尺寸为单位导出,小视野的比例就是1/mSize
        }
        mTmpRects[id] = mViewRects[id];

        for(int r = 0; r < mUiViewMaskSize; ++r) {
            for (int c = 0; c < mUiViewMaskSize; ++c) {
                for(auto viewRect: mViewRects[id]) {
                    auto rect = mapToSize(viewRect.rect,QPointF(0.0,0.0),
                                          mUiViewMaskSize,mUiViewMaskSize); // 等比例放大尺寸到mUiViewMaskSize
                    if (rect.intersects(QRectF(c,r,1.0,1.0))) {
                        ViewPoint point;
                        point.x = convertPrecision((r+0.5)/mUiViewMaskSize);
                        point.y = convertPrecision((c+0.5)/mUiViewMaskSize);
                        mUiViewMaskNormPoints[id].append(point);
                    }
                }  // end 2层for
            }
        }

        mTmpUiViewMaskNormPoints[id] = mUiViewMaskNormPoints[id];
        applyholeact->trigger();
        update();
    }

    void ViewPatternV2::onApplyAllAct()
    {
        checkField();
        if (mViewInfo[HoleGroupNameField].toString().isEmpty())
            return;

        auto id = holeID();
        QVariantMap m;
        m[HoleViewSizeField] = mViewInfo[HoleViewSizeField]; // 组装视野窗口尺寸
        m[HoleViewRectsField].setValue(mViewRects[id]); // 视野窗口的区域信息
        m[HoleViewUiPointsField].setValue(mUiViewMaskNormPoints[id]);
        m[HoleViewPointsField].setValue(mViewMaskNormPoints);
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
                        auto pt_idx = hole.x()*PointToIDCoefficient+hole.y(); // 所有其他孔的临时数据区更新为当前孔的视野信息
                        mTmpRects[pt_idx] = mViewRects[id];
                        mViewRects[pt_idx] = mViewRects[id];
                        mUiViewMaskNormPoints[pt_idx] = mUiViewMaskNormPoints[id];
                        mTmpUiViewMaskNormPoints[pt_idx] = mUiViewMaskNormPoints[id];
                    }
            }
        update();
    }

    void ViewPatternV2::onApplyGroupAct()
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
        m[HoleViewRectsField].setValue(mViewRects[id]); // 视野窗口的区域信息
        m[HoleViewUiPointsField].setValue(mUiViewMaskNormPoints[id]);
        m[HoleViewPointsField].setValue(mViewMaskNormPoints);
        emit applyGroupEvent(m);

        auto holeCoordinates = mViewInfo[HoleGroupCoordinatesField].value<QPointVector>();//拿到本组其它孔的所有孔坐标
                foreach(auto pt, holeCoordinates) {
                auto pt_idx = pt.x()*PointToIDCoefficient+pt.y(); // 本组其他孔的临时数据区更新为当前孔的视野信息
                mTmpRects[pt_idx] = mViewRects[id];
                mViewRects[pt_idx] = mViewRects[id];
                mUiViewMaskNormPoints[pt_idx] = mUiViewMaskNormPoints[id];
                mTmpUiViewMaskNormPoints[pt_idx] = mUiViewMaskNormPoints[id];
            }
        update();
    }

    void ViewPatternV2::onApplyHoleAct()
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
        m[HoleViewRectsField].setValue(mViewRects[id]); // 视野窗口的区域信息
        m[HoleViewUiPointsField].setValue(mUiViewMaskNormPoints[id]);
        m[HoleViewPointsField].setValue(mViewMaskNormPoints);
        emit applyHoleEvent(m);
    }

    void ViewPatternV2::onRemoveViewAct()
    {
        auto id = holeID();
        if (mSelectMode == RectMode) {
            ViewRectF info;
            info.flag = false;// 表示是保存还是删除这个区域

#ifdef notuse_remove_whole
            // 框选的时候保存框选的矩形,不保存单击生成的矩形
    if (!mDrapRectF.isEmpty()) {
        info.rect = mDrapRectF;
        mViewRects[id].append(info);
    } else {
        info.rect = mMouseRect;
        mViewRects[id].append(info);
    }
#else
            // 不是原来的方式:删除添加flag为false,保存添加flag为true
            // 现在改为整体删除,那么只需要检测已有的矩形是否和框选的矩形有交集,直接删除之前保存的就可以
            int i = 0;
            if (!mDrapRectF.isEmpty()) {
                for(i = 0; i < mViewRects[id].count(); ++i) {
                    if (mViewRects[id][i].rect.intersects(mDrapRectF))
                        break;
                }
            } else {
                for(i = 0; i < mViewRects[id].count(); ++i) {
                    if (mViewRects[id][i].rect.intersects(mMouseRect))
                        break;
                }
            }

            // 已经没有了不能继续删,否则越界; 没有交集也不能删除,此时i变成mViewRects[id].count()
            if (!mViewRects[id].isEmpty() && i < mViewRects[id].count())
                mViewRects[id].remove(i);
#endif

            viewRectsMapToViewMask();
            mMouseRect = QRectF();
            mTmpRects[id] = mViewRects[id];
            applyholeact->trigger();
        } else {
//            mUiSelectedPoints[id].append(mapMousePoints());
//            mTmpUiSelectedPoints[id] = mUiSelectedPoints[id];
        }

        update();
    }

#define use_save_cover 0
    void ViewPatternV2::onSaveViewAct()
    {
        auto id = holeID();
        if(mSelectMode == RectMode) {
            ViewRectF info;
            info.flag = true;// 表示是保存还是删除这个区域

            // 框选的时候保存框选的矩形,不保存单击生成的矩形
            if (!mDrapRectF.isEmpty()) {
                info.rect = mDrapRectF;
#ifdef use_save_cover  // 变为覆盖处理,也就是说flag其实不再需要了
                if (mViewRects[id].isEmpty())
                    mViewRects[id].append(info);
                else mViewRects[id][0] = info; // 但是懒得改以前的代码设计,兼容一下
#else
                mViewRects[id].append(info);
#endif
            } else {
                info.rect = mMouseRect;
#ifdef use_save_cover
                if (mViewRects[id].isEmpty())
                    mViewRects[id].append(info);
                else mViewRects[id][0] = info; // 变为覆盖处理
#else
                mViewRects[id].append(info);
#endif
            }
            mTmpRects[id] = mViewRects[id];

            viewRectsMapToViewMask();
            mMouseRect = QRectF();

            applyholeact->trigger();
        } else {
            mUiSelectedPoints[id].append(mapFromPointF(mMousePos));
            mTmpUiSelectedPoints[id] = mUiSelectedPoints[id];
        }
        LOG<<mUiSelectedPoints[id];
        update();
    }

    ViewPatternV2::ViewPatternV2(QWidget *parent) : View(parent)
    {
            initUiViewMask();
    }

    int ViewPatternV2::holeID() const
    { // 每个孔双击打开视野窗口都是一对一的
        auto coordinate = mViewInfo[HoleCoordinateField].toPoint();
        auto id = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
        return id;
    }

    int ViewPatternV2::holeID(const QPoint& holePoint) const
    {
        return holePoint.x() * PointToIDCoefficient + holePoint.y();
    }

    void ViewPatternV2::initUiViewMask()
    {
        mUiViewMask.clear();
        for(int r = 0; r < mUiViewMaskSize; ++r)
        {
            QBoolVector vec;
            for(int c = 0; c < mUiViewMaskSize; ++c) {
                vec.append(false);
            }
            mUiViewMask.append(vec);
        }
    }

    void ViewPatternV2::viewRectsMapToViewMask()
    { // 把mViewRects[idx]的信息映射到Mask,依据mSize
        auto id = holeID();
        auto viewRects = mViewRects[id];

        mUiViewMaskNormPoints[id].clear();
        mTmpUiViewMaskNormPoints[id].clear();
        mViewMaskNormPoints.clear();
        //initUiViewMask();

        for(int r = 0; r < mUiViewMaskSize; ++r) {
            for(int c = 0; c < mUiViewMaskSize; ++c) {
                mUiViewMask[r][c] = false; // 先清理掉否则重复添加
                for(auto viewRect: viewRects) {
                    auto flag = viewRect.flag; // 对于位置[r][c] 3层的for结束后,就知道这个位置是否被选中
                    auto rect = mapToSize(viewRect.rect,QPointF(0.0,0.0),
                                          mUiViewMaskSize,mUiViewMaskSize); // 等比例放大尺寸到mUiViewMaskSize
                    if (rect.intersects(QRectF(c,r,1.0,1.0))) {
                        //mUiViewMask[id][r][c] = flag; //掩码对应位置根据flag标记是保存还是删除的区域
                        mUiViewMask[r][c] = flag; //掩码对应位置根据flag标记是保存还是删除的区域
                    }
                }  // end 2层for

                if (mUiViewMask[r][c]) { // 顺便把不为0的位置记录下来,其它地方就不再需要重复遍历了
                    ViewPoint point;
                    point.x = convertPrecision((c+0.5)/mUiViewMaskSize);
                    point.y = convertPrecision((r+0.5)/mUiViewMaskSize);
                    mUiViewMaskNormPoints[id].append(point);
                }
            }
        }
        mTmpUiViewMaskNormPoints[id] = mUiViewMaskNormPoints[id];

        QPointFVector points;
        auto view_w = qCeil(getInnerRectWidth());
        auto view_h = qCeil(getInnerRectHeight());
        for(auto viewRect: viewRects) {
            auto flag = viewRect.flag;
            auto rect = mapToSize(viewRect.rect,QPointF(0.0,0.0),
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
                    if (flag) { // 如果是标记为保存的区域,2个小矩形的中心距离小于阈值就认为是靠近,只保留原来的
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
                    } else { // 如果标记为要删除的点
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

        ViewPoint point;
        for(auto pt: points) {
            point.x = convertPrecision(pt.x()/(getCircleRadius()*2.0));
            point.y = convertPrecision(pt.y()/(getCircleRadius()*2.0));
            mViewMaskNormPoints.append(point);
        }

        LOG<<"ui count = "<<mUiViewMaskNormPoints[id].count()
           <<" mask count = "<<mViewMaskNormPoints.count()<<points.count();
    }

    void ViewPatternV2::paintEvent(QPaintEvent *event)
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

        if (mSelectMode == RectMode) { // 区域模式
            // 2. 绘制选中的区域
            // 以下2种绘制法都可以,和wellpattern绘制对应,wellpattern也是2种绘制法
            // 1. 直接绘制视野区域 对应wellpattern的(3.2)画法
//            if (!mViewRects[id].isEmpty()) {
//                for(auto viewRect: mViewRects[id]) {
//                    if (viewRect.flag)
//                        painter.fillRect(mapToSize(viewRect.rect,p11,diameter,diameter),groupcolor);
//                    else painter.fillRect(mapToSize(viewRect.rect,p11,diameter,diameter),DefaultNativeColor);
//                }
//            }

            // 2. 使用掩码矩阵点的缩放区域绘制,和wellpattern的mViewMaskSize一致就可 对应(3.4)画法
            //LOG<<mUiViewMaskNormPoints[holeID()].count()<<diameter;
            //LOG<<"id = "<<id<<mUiViewMaskNormPoints[id].count();
            for(auto pt: mUiViewMaskNormPoints[id]) {
                auto w = 1.0 / mUiViewMaskSize;
                auto h = 1.0 / mUiViewMaskSize;
                auto viewRect = QRectF(pt.x.toDouble(),pt.y.toDouble(),w,h);
                //LOG<<mapToSize(viewRect,p11,diameter,diameter);
                painter.fillRect(mapToSize(viewRect,p11,diameter,diameter),groupcolor);
            }

            // 3. 绘制圆,外接正方形和便于区分的灰色网格线

            painter.drawLine(p11,p21);
            painter.drawLine(p12,p22);
            painter.drawLine(p11,p12);
            painter.drawLine(p21,p22);
            pen = painter.pen();
            pen.setWidth(1);
            pen.setColor(Qt::gray);
            painter.setPen(pen);
            auto hor_offset = getInnerRectWidth();
            for (int i = 1; i <= mSize-1; ++i) {
                auto top = p11 + QPointF(i*hor_offset,0);
                auto bottom = p21 + QPointF(i*hor_offset,0);
                painter.drawLine(top,bottom);
            }
            auto ver_offset = getInnerRectHeight();
            for (int i = 1; i <= mSize-1; ++i){
                auto top = p11 + QPointF(0,ver_offset*i);
                auto bottom = p12 + QPointF(0,ver_offset*i);
                painter.drawLine(top,bottom);
            }

            pen = painter.pen();
            pen.setWidth(DefaultPainterPenWidth);
            pen.setColor(Qt::blue);
            painter.setPen(pen);

            // 4. 鼠标单击生成的矩形框
            painter.drawRect(mapToSize(mMouseRect,p11,diameter,diameter));
            // 5. 鼠标拖拽生成的矩形框
            if (!mDrapRectF.isEmpty()) {
                painter.drawRect(mapToSize(mDrapRectF,p11,diameter,diameter));
            }

            pen.setColor(Qt::black); // 恢复,否则绘制其他的都变颜色了
            painter.setPen(pen);
        } else { // 点模式
            pen = painter.pen();
            pen.setWidth(DefaultDrawPointWidth);
            painter.setPen(pen);
            //LOG<<mUiSelectedPoints[id];
            for(auto pt:mUiSelectedPoints[id]) {
                painter.drawPoint(mapToPointF(pt));
            }
            painter.drawPoint(mMousePos);
            pen = painter.pen();
            pen.setWidth(DefaultPainterPenWidth);
            painter.setPen(pen);
        }
        // 画圆
        painter.drawEllipse(QPointF(width()/2.0,height()/2.0),radius,radius);

        event->accept();
    }

    void ViewPatternV2::mousePressEvent(QMouseEvent *event)
    {
        View::mousePressEvent(event);

//                auto rectinfo = mViewRects[holeID()];
//                if (rectinfo.isEmpty()) {
//                    removeviewact->setEnabled(false);
//                } else {
//                    for(auto viewRect: rectinfo) {
//                        if (viewRect.flag) { // 保存的区域
//                            if (viewRect.rect.contains(mDrapRectF)) { // 已经包含了
//                                saveviewact->setEnabled(false);
//                                removeviewact->setEnabled(true);
//                            } else if (!viewRect.rect.intersects(mDrapRectF)){ // 无交集
//                                saveviewact->setEnabled(true); // 不是已经保存的可以保存
//                                removeviewact->setEnabled(true);
//                            } else { // 有交集
//                                saveviewact->setEnabled(true);
//                                removeviewact->setEnabled(true);
//                            }
//                        } else {
//                            if (viewRect.rect.contains(mDrapRectF)) { // 已经是完全删除的
//                                saveviewact->setEnabled(true); //可以保存但是不可以删
//                                removeviewact->setEnabled(false);
//                            } else if (!viewRect.rect.intersects(mDrapRectF)){ // 不是删除的
//                                saveviewact->setEnabled(false);
//                                removeviewact->setEnabled(true);// 不是已经删除的去删除
//                            } else {
//                                saveviewact->setEnabled(true);
//                                removeviewact->setEnabled(true);
//                            }
//                        }
//                    }
//                }
    }

    void ViewPatternV2::mouseReleaseEvent(QMouseEvent *event)
    {
        View::mouseReleaseEvent(event);
    }

    void ViewPatternV2::mouseMoveEvent(QMouseEvent *event)
    {
        View::mouseMoveEvent(event);
    }
}