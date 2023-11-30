/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-25 09:48:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 09:49:29
 * @FilePath: \EOS\interface\src\preview\viewpattern.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#include "viewpattern.h"

inline namespace V2 { // 2024//11/27 需求变更需要重新设计

    void ViewPattern::clearViewWindowCache(const QPoint &holePoint)
    {// 删孔时清除该孔对应的视野信息
        auto id = holeID(holePoint);
        mViewRects[id].clear();
        mTmpRects[id].clear();
        update();
    }

    void ViewPattern::clearAllViewWindowCache(int viewSize,bool toggleObjective)
    {// 切换厂家或者物镜倍数时把所有的区域信息都要清空,对于mViewInfo只需要清理视野相关的2个信息
        mDrapRectF = QRectF();
        mViewRects.clear();
        mTmpRects.clear();

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

    void ViewPattern::updateViewWindowCache(QCPoint holePoint, QCPointVector viewPoints,int viewSize)
    {// 导入实验配置时去更新view的ui信息
        update();
    }

    void ViewPattern::onApplyAllAct()
    {
        checkField();
        if (mViewInfo[HoleGroupNameField].toString().isEmpty())
            return;

        QVariantMap m;
        m[HoleViewSizeField] = mViewInfo[HoleViewSizeField]; // 组装视野窗口尺寸
        m[HoleViewRectsField].setValue(mViewRects[holeID()]); // 视野窗口的区域信息
        m[HoleViewUiPointsField].setValue(mUiViewMaskNormPoints);
        m[HoleViewPointsField].setValue(mViewMaskNormPoints);
        m[HoleGroupColorField] = mViewInfo[HoleGroupColorField]; // 组装组颜色,可以把所有组颜色统一(可能没分过组默认颜色红色,无所谓)
        m[HoleGroupNameField] = mViewInfo[HoleGroupNameField];// 组装组名信息(应用到所有组时只有同组的组颜色需要覆盖)
        m[WellAllHolesField] = mViewInfo[WellAllHolesField]; // 孔板所有选择的孔坐标信息顺带组装
        //m[HoleCoordinateField] = mViewInfo[HoleCoordinateField]; // 坐标信息顺带组装
        //m[WellAllGroupsField] = mViewInfo[WellAllGroupsField]; // 孔板所有组名信息顺带组装
        // HoleGroupCoordinatesField 该组的所有孔坐标不组装
        emit applyAllEvent(m);// 5个信息足够

        auto allholes = mViewInfo[WellAllHolesField].value<QPoint2DVector>();//拿到所有分过组的孔坐标
        auto id = holeID();
        foreach(auto holes, allholes) {
            foreach (auto hole, holes) {
                auto pt_idx = hole.x()*PointToIDCoefficient+hole.y(); // 所有其他孔的临时数据区更新为当前孔的视野信息
                mTmpRects[pt_idx] = mViewRects[id];
                mViewRects[pt_idx] = mViewRects[id];
            }
        }
    }

    void ViewPattern::onApplyGroupAct()
    {
        checkField();
        if (mViewInfo[HoleGroupNameField].toString().isEmpty())
            return;

        QVariantMap m;
        m[HoleGroupNameField] = mViewInfo[HoleGroupNameField];// 组装组名称,方便pattern依据组名查找所有孔
        m[HoleGroupColorField] = mViewInfo[HoleGroupColorField]; // 组装组颜色,可以让pattern把同组内其他可能不相同的颜色全部统一
        m[HoleCoordinateField] = mViewInfo[HoleCoordinateField]; // 坐标信息顺带组装
        m[WellAllGroupsField] = mViewInfo[WellAllGroupsField]; // 孔板所有组名信息顺带组装
        m[WellAllHolesField] = mViewInfo[WellAllHolesField]; // 孔板所有选择的孔坐标信息顺带组装
        m[HoleViewSizeField] = mViewInfo[HoleViewSizeField]; // 组装视野窗口尺寸
        // HoleGroupCoordinatesField 该组的所有孔坐标不组装
        m[HoleViewRectsField].setValue(mViewRects[holeID()]); // 视野窗口的区域信息
        m[HoleViewUiPointsField].setValue(mUiViewMaskNormPoints);
        m[HoleViewPointsField].setValue(mViewMaskNormPoints);
        emit applyGroupEvent(m);

        auto holeCoordinates = mViewInfo[HoleGroupCoordinatesField].value<QPointVector>();//拿到本组其它孔的所有孔坐标
        auto id = holeID();
        foreach(auto pt, holeCoordinates) {
            auto pt_idx = pt.x()*PointToIDCoefficient+pt.y(); // 本组其他孔的临时数据区更新为当前孔的视野信息
            mTmpRects[pt_idx] = mViewRects[id];
            mViewRects[pt_idx] = mViewRects[id];
        }
    }

    void ViewPattern::onApplyHoleAct()
    {
        checkField();
        if (mViewInfo[HoleGroupNameField].toString().isEmpty())
            return;

        QVariantMap m;
        m[HoleGroupNameField] = mViewInfo[HoleGroupNameField];// 组装组名称,方便pattern依据组名查找所有孔
        m[HoleGroupColorField] = mViewInfo[HoleGroupColorField]; // 组装组颜色,可以让pattern把同组内其他可能不相同的颜色全部统一
        m[HoleCoordinateField] = mViewInfo[HoleCoordinateField]; // 坐标信息顺带组装
        m[WellAllGroupsField] = mViewInfo[WellAllGroupsField]; // 孔板所有组名信息顺带组装
        m[WellAllHolesField] = mViewInfo[WellAllHolesField]; // 孔板所有选择的孔坐标信息顺带组装
        m[HoleViewSizeField] = mViewInfo[HoleViewSizeField]; // 组装视野窗口尺寸
        // HoleGroupCoordinatesField 该组的所有孔坐标不组装
        m[HoleViewRectsField].setValue(mViewRects[holeID()]); // 视野窗口的区域信息
        m[HoleViewUiPointsField].setValue(mUiViewMaskNormPoints);
        m[HoleViewPointsField].setValue(mViewMaskNormPoints);
        emit applyHoleEvent(m);
    }

    void ViewPattern::onRemoveViewAct()
    {
        auto id = holeID();
        ViewRectF info;
        info.flag = false;// 表示是保存还是删除这个区域

        // 框选的时候保存框选的矩形,不保存单击生成的矩形
        if (!mDrapRectF.isEmpty()) {
            info.rect = mDrapRectF;
            mViewRects[id].append(info);
        } else {
            info.rect = mMouseRect;
            mViewRects[id].append(info);
        }
        viewRectsMapToViewMask();
        mMouseRect = QRectF();
        mTmpRects[id] = mViewRects[id];
        applyholeact->trigger();
        update();
    }

    void ViewPattern::onSaveViewAct()
    {
        auto id = holeID();
        ViewRectF info;
        info.flag = true;// 表示是保存还是删除这个区域

        // 框选的时候保存框选的矩形,不保存单击生成的矩形
        if (!mDrapRectF.isEmpty()) {
            info.rect = mDrapRectF;
            mViewRects[id].append(info);
        } else {
            info.rect = mMouseRect;
            mViewRects[id].append(info);
        }
        viewRectsMapToViewMask();
        mMouseRect = QRectF();
        mTmpRects[id] = mViewRects[id];
        applyholeact->trigger();
        update();
    }

    void ViewPattern::setViewInfo(const ViewInfo &info)
    {
        mViewInfo = info;
        checkField();

        mMousePos = {-1.0,-1.0};
        mMouseRect = QRectF();
        mDrapRectF = QRectF();

        auto id = holeID();
        mViewRects[id].clear();

        auto size =  mViewInfo[HoleViewSizeField].toInt();

        if(!mTmpRects[id].isEmpty() && size == mSize)
            mViewRects[id] = mTmpRects[id];
        else mTmpRects[id].clear();

        mSize = size;

        update();
    }

    ViewPattern::ViewPattern(QWidget *parent) : QWidget(parent)
    {
        initViewMask();
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

        connect(saveviewact,&QAction::triggered,this,&ViewPattern::onSaveViewAct);
        connect(removeviewact,&QAction::triggered,this,&ViewPattern::onRemoveViewAct);
        connect(applyholeact,&QAction::triggered,this,&ViewPattern::onApplyHoleAct);
        connect(applygroupact,&QAction::triggered,this,&ViewPattern::onApplyGroupAct);
        connect(applyallact,&QAction::triggered,this,&ViewPattern::onApplyAllAct);
    }

    int ViewPattern::holeID() const
    { // 每个孔双击打开视野窗口都是一对一的
        auto coordinate = mViewInfo[HoleCoordinateField].toPoint();
        auto id = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
        return id;
    }

    int ViewPattern::holeID(const QPoint& holePoint) const
    {
        return holePoint.x() * PointToIDCoefficient + holePoint.y();
    }

    bool ViewPattern::checkField() const
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

    void ViewPattern::initViewMask()
    {
        mUiViewMask.clear();
        mViewMask.clear();
        for(int r = 0; r < mUiViewMaskSize; ++r)
        {
            QBoolVector vec;
            for(int c = 0; c < mUiViewMaskSize; ++c) {
                vec.append(false);
            }
            mUiViewMask.append(vec);
        }
        for(int r = 0; r < mViewMaskSize; ++r)
        {
            QBoolVector vec;
            for(int c = 0; c < mViewMaskSize; ++c) {
                vec.append(false);
            }
            mViewMask.append(vec);
        }
    }

    QRectF ViewPattern::mapToSize(const QRectF &source,const QPointF&ref_point,int ref_w,int ref_h)
    { // source已经是0-1的等比例缩放了
        auto topleft = source.topLeft();
        auto size = source.size();

        auto x = topleft.x() * ref_w + ref_point.x();// 左上角顶点(0.2,0.3),长度(0.25,0.35)
        auto y = topleft.y() * ref_h + ref_point.y();// 放大到尺寸为(ref_w,ref_h),例如100
        auto w = size.width() * ref_w; // 那么结果就是(20,30,25,35),如果左上角参考点还需要移动就变成 (30,50,25,35)
        auto h = size.height() * ref_h;

        return QRectF(x,y,w,h);
    }

    QRectF ViewPattern::mapFromSize(const QRectF &source,const QPointF&ref_point,int ref_w,int ref_h)
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

    void ViewPattern::viewRectsMapToViewMask()
    { // 把mViewRects[idx]的信息映射到Mask,依据mSize
        auto viewRects = mViewRects[holeID()];

        mUiViewMaskPoints.clear();
        mUiViewMaskNormPoints.clear();
        mViewMaskNormPoints.clear();

        if (mViewMaskSize >= mUiViewMaskSize) {
            for(int r = 0; r < mViewMaskSize; ++r) {
                for(int c = 0; c < mViewMaskSize; ++c) {
                    for(auto viewRect: viewRects) {
                        auto flag = viewRect.flag; // 对于位置[r][c] 3层的for结束后,就知道这个位置是否被选中
                        // 1. 处理100x100的部分
                        if (r < mUiViewMaskSize && c < mUiViewMaskSize)
                        {
                            auto rect = mapToSize(viewRect.rect,QPointF(0.0,0.0),
                                                  mUiViewMaskSize,mUiViewMaskSize); // 等比例放大尺寸到mUiViewMaskSize
                            if (rect.intersects(QRectF(c,r,1.0,1.0))) {
                                mUiViewMask[r][c] = flag; //掩码对应位置根据flag标记是保存还是删除的区域
                            }
                        }
                        // 2. 所有坐标都处理
                        auto rect = mapToSize(viewRect.rect,QPointF(0.0,0.0),
                                              mViewMaskSize,mViewMaskSize); // 等比例放大尺寸到mViewMaskSize
                        if (rect.intersects(QRectF(c,r,1.0,1.0))) {
                            mViewMask[r][c] = flag; //掩码对应位置根据flag标记是保存还是删除的区域
                        }
                    }  // end 2层for

                    // 1. 处理100x100的部分
                    if (r < mUiViewMaskSize && c < mUiViewMaskSize) {// 要放在二层for这里
                        if (mUiViewMask[r][c]) { // 顺便把不为0的位置记录下来,其它地方就不再需要重复遍历了
                            mUiViewMaskPoints.append(QPointF(c+0.5,r+0.5));
                            ViewPoint point;
                            point.x = convertPrecision((c+0.5)/mUiViewMaskSize);
                            point.y = convertPrecision((r+0.5)/mUiViewMaskSize);
                            mUiViewMaskNormPoints.append(point);
                        }
                    }
                    // 2. 所有坐标都处理
                    if (mViewMask[r][c]) { // 顺便把不为0的位置记录下来,其它地方就不再需要重复遍历了
                        ViewPoint point;
                        point.x = convertPrecision((c+0.5)/mViewMaskSize);
                        point.y = convertPrecision((r+0.5)/mViewMaskSize);
                        mViewMaskNormPoints.append(point);
                    }
                }
            }
        } else { // mViewMaskSize比较小
            for(int r = 0; r < mUiViewMaskSize; ++r) {
                for(int c = 0; c < mUiViewMaskSize; ++c) {
                    for(auto viewRect: viewRects) {
                        auto flag = viewRect.flag; // 对于位置[r][c] 3层的for结束后,就知道这个位置是否被选中
                        // 1. 处理100x100的部分
                        if (r < mViewMaskSize && c < mViewMaskSize)
                        {
                            auto rect = mapToSize(viewRect.rect,QPointF(0.0,0.0),
                                                  mViewMaskSize,mViewMaskSize); // 等比例放大尺寸到mUiViewMaskSize
                            if (rect.intersects(QRectF(c,r,1.0,1.0))) {
                                mViewMask[r][c] = flag; //掩码对应位置根据flag标记是保存还是删除的区域
                            }
                        }
                        // 2. 所有坐标都处理
                        auto rect = mapToSize(viewRect.rect,QPointF(0.0,0.0),
                                              mUiViewMaskSize,mUiViewMaskSize); // 等比例放大尺寸到mViewMaskSize
                        if (rect.intersects(QRectF(c,r,1.0,1.0))) {
                            mUiViewMask[r][c] = flag; //掩码对应位置根据flag标记是保存还是删除的区域
                        }
                    }  // end 2层for

                    // 1. 处理100x100的部分
                    if (r < mViewMaskSize && c < mViewMaskSize) {// 要放在二层for这里
                        if (mViewMask[r][c]) { // 顺便把不为0的位置记录下来,其它地方就不再需要重复遍历了
                            ViewPoint point;
                            point.x = convertPrecision((c+0.5)/mViewMaskSize);
                            point.y = convertPrecision((r+0.5)/mViewMaskSize);
                            mViewMaskNormPoints.append(point);
                        }
                    }
                    // 2. 所有坐标都处理
                    if (mUiViewMask[r][c]) { // 顺便把不为0的位置记录下来,其它地方就不再需要重复遍历了
                        mUiViewMaskPoints.append(QPointF(c+0.5,r+0.5));
                        ViewPoint point;
                        point.x = convertPrecision((c+0.5)/mUiViewMaskSize);
                        point.y = convertPrecision((r+0.5)/mUiViewMaskSize);
                        mUiViewMaskNormPoints.append(point);
                    }
                }
            }
        }

        LOG<<"ui count = "<<mUiViewMaskNormPoints.count()<<" mask count = "<<mViewMaskNormPoints.count();
    }

    ViewInfo ViewPattern::viewInfo() const
    {
        return mViewInfo;
    }

    void ViewPattern::setDisablePoint(QCPoint point, bool enable)
    {


    }

    void ViewPattern::setDisablePoints(QCPointVector points, bool enable)
    {

    }

    void ViewPattern::setDisablePoints(bool enable)
    {

    }
}

namespace V1 {
    void ViewPattern::setStrategy(ViewPattern::DrawStrategy s, const QVariantMap& m)
    { // 设置策略+传递孔的信息+视野尺寸信息+应用到组使能
        // 1. 初始化通用信息
        strategy = s;
        mMousePoint = {-1,-1}; // 必须更新,否则上次的鼠标点还在会导致切换物镜或者brand出现越界
        mLastPos = {-1,-1};
        mDrapRect = QRectF(); // 上次的框选痕迹还在要清除
        mDrapPoints.clear();
        //initDrapPoints();//不能在这里调用,尺寸没更新

        if (strategy != InnerCircleRect) { // 如果不是InnerCircleRect策略,其他变量不用更新
            update();
            return;
        }

        // 2. 初始化InnerCircleRect策略需要的信息
        mCurrentViewInfo = m; // 来自pattern传递的信息(name,color,grouppoints,coordinate+allgroups)+objective/brand确定的viewsize信息
        auto size = m[HoleViewSizeField].toSize();//WellPattern::onOpenViewAct()组装的数据
        auto holename = m[HoleGroupNameField].toString();
        auto holecolor = m[HoleGroupColorField].toString();
        auto holecoordinate = m[HoleCoordinateField].toPoint();
        auto holegrouppoints = m[HoleGroupCoordinatesField].value<QPointVector>();
        auto wellallgroup = m[WellAllGroupsField].value<QSet<QString>>();
        auto wellallholes =m[WellAllHolesField].value<QPoint2DVector>();

//    LOG<<"view accept info from well is "<<holename<<holecolor<<holecoordinate
//    <<"【"<<holegrouppoints<<"】"<<wellallgroup<<size;

        // 3. 初始化视野尺寸,重新更新

        mrows = size.width();
        mcols = size.height();
        initDrapPoints();// 先更新尺寸
        initDisablePoints(); // 初始化置灰区域
        initSelectPoints();// 才能基于尺寸更新视野已被选择的信息(上次设置的临时信息)

        // 4.更新应用到本组的使能,未分过组或者分过组但是没选过视野
        int viewcount = viewPointCount();
        if (!holename.isEmpty() && viewcount) // 这个孔不属于任何组
            applygroupact->setEnabled(true);
        else applygroupact->setEnabled(false);


        viewcount? applyallact->setEnabled(true):applyallact->setEnabled(false); // 应用到所有组不受分组影响,只取决于当前视野选择的数量
        viewcount? removeviewact->setEnabled(true):removeviewact->setEnabled(false); // 删点不受分组影响,只取决于当前视野选择的数量

        update();
    }

    void ViewPattern::clearViewWindowCache(const QPoint &holepoint)
    { // 删孔时清除该孔对应的视野缓存信息
        // 1. 计算这个孔的id
        auto idx = holepoint.x()*PointToIDCoefficient+holepoint.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx

        // 2.此时点击视野坐标会越界所以还是需要重新分配空间
        QBool2DVector vec;
        for(int row = 0 ; row < mrows; ++ row) {
            QBoolVector var;
            for (int col = 0; col < mcols; ++col){
                var.append(false);
            }
            vec.append(var);
        }
        mViewSelectPoints[idx] = vec;
        mTmpViewSelectPoints[idx] = vec;
        LOG<<"is clear "<<holepoint<<" 's cache";
        update();
    }

    void ViewPattern::clearAllViewWindowCache(int viewsize)
    { // 切换厂家或者物镜倍数时把所有的信息都要清空,不仅是当前孔,所有的孔信息都要清除
        mMousePoint = {-1,-1}; // 必须重置,否则可能之前遗留的鼠标坐标超过现在的size范围导致绘图越界
        mDrapRect = QRectF();
        mViewSelectPoints.clear();
        mTmpViewSelectPoints.clear();

        auto currentviewsize = mCurrentViewInfo[HoleViewSizeField].toSize();
        Q_ASSERT(currentviewsize.width() == currentviewsize.height());
        if (currentviewsize.width() != viewsize) {// 视野尺寸相同就不需要清除
            mCurrentViewInfo[HoleViewSizeField] = QSize(viewsize,viewsize); // 更新viewsize
            mCurrentViewInfo[HoleViewPointsField].setValue(QPointVector()); // 清除原有的视野区域
        } // 其它不要清除,否则WellPattern::updateHoleInfoByViewInfoApplyAll这里的交互和QAssert可能设定失败

        mrows = viewsize;
        mcols = viewsize;
        initDrapPoints();
        initDisablePoints();
        initSelectPoints();
        update();
    }

    void ViewPattern::updateViewWindowCache(QCPoint holepoint, QCPointVector viewpoints,int viewsize)
    { // 导入实验配置时去更新view的ui信息
        mCurrentViewInfo[HoleCoordinateField] = holepoint;
        mrows = viewsize;
        mcols = viewsize;
        initDrapPoints();
        initDisablePoints();
        initSelectPoints();

        auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();
        auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
                foreach(auto viewpoint,viewpoints) {
                mViewSelectPoints[idx][viewpoint.x()][viewpoint.y()] = true;
            }
        mTmpViewSelectPoints[idx] = mViewSelectPoints[idx];
        update();
    }

    void ViewPattern::onSaveViewAct()
    { // 保存选择的视野坐标到当前孔id对应的视野数据区并保存到临时信息用于initSelectPoints重新初始化

        // 1. 计算当前孔的唯一id
        auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();
        auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
        bool haveSelect = false; // 是否至少选了1个视野,viewPointCount()计算也可以,这里少做次循环

        // 2. 防止框选时鼠标点击的地方漏掉,鼠标点击的坐标也要认为是框选上,一层保障,非禁用坐标
        if (mMousePoint != QPoint(-1,-1) && !mDisablePoints[mMousePoint.x()][mMousePoint.y()]) {
            mViewSelectPoints[idx][mMousePoint.x()][mMousePoint.y()] = true;
            haveSelect = true;
            mDrapPoints[mMousePoint.x()][mMousePoint.y()] = false;
        }

        // 3. 把框选区域的视野保存,然后清除框选区域
        for(int r = 0; r < mrows; ++r) {
            for(int c = 0; c < mcols; ++c) {
                if (mDrapPoints[r][c] && !mDisablePoints[r][c]) { // 多层保护 灰色坐标不可选中
                    mDrapPoints[r][c] = false;
                    mViewSelectPoints[idx][r][c] = true;
                    haveSelect = true;
                }
            }
        }

        // 4. 根据新的保存视野数量去更新使能,但是可能没分过组,2个共同控制
        if (!mCurrentViewInfo[HoleGroupNameField].toString().isEmpty() && haveSelect)
            applygroupact->setEnabled(true);// 如果没分组还是不允许应用到本组
        else applygroupact->setEnabled(false);

        haveSelect? removeviewact->setEnabled(true):removeviewact->setEnabled(false); // 应用所有组不受分组影响,没有点就不能应用
        haveSelect? removeviewact->setEnabled(true):removeviewact->setEnabled(false); // 删点不受分组影响,没有点就不能删点

        // 5. 每次保存视野的数量位置信息都保存到临时信息,用于更新下次的重新保存的初始化(initSelectPoints内更新)
        mTmpViewSelectPoints[idx] = mViewSelectPoints[idx]; // 临时保存这次设置

        applyholeact->trigger();

        update();
    }

    void ViewPattern::onRemoveViewAct()
    {
        // 1.判断是否允许删点
        if (mMousePoint==QPoint(-1,-1) || !viewPointCount() || mDisablePoints[mMousePoint.x()][mMousePoint.y()])
            return; // 一层保障

        // 2. 计算当前孔的唯一id拿到当前孔对应视野信息
        auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();
        auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
        auto currentviewinfo = mViewSelectPoints[idx];

        // 3. 更新mViewSelectPoints视野信息
        currentviewinfo[mMousePoint.x()][mMousePoint.y()] = false;
        for(int r = 0; r < mrows; ++r) { // 鼠标+框选的都可以删除
            for(int c = 0; c < mcols; ++c) {
                if (mDrapPoints[r][c] && !mDisablePoints[r][c]) { // 多层保护,灰色坐标不可更新
                    currentviewinfo[r][c] = false;
                    mDrapPoints[r][c] = false;
                }
            }
        }
        mViewSelectPoints[idx] = currentviewinfo;

        // 4. 更新mTmpViewSelectPoints临时信息
        mTmpViewSelectPoints[idx] = currentviewinfo;

        // 5. 如果视野数量变成0 禁止应用到组/所有组和删点
        if (!viewPointCount()) {
            applyallact->setEnabled(false);
            applygroupact->setEnabled(false);
            removeviewact->setEnabled(false);
        }

        applyholeact->trigger();
        update();
    }

    void ViewPattern::onApplyHoleAct()
    {
        // 1. 当前孔没有分过组或者没有保存过点不允许触发应用到孔事件
        if (mCurrentViewInfo[HoleGroupNameField].toString().isEmpty() || !viewPointCount())
            return; // 多加一层保护总没坏处

        // 2. 组装组名+组颜色+视野尺寸+当前孔坐标+已有的所有组+本孔选择的所有视野坐标 6个信息 4个关键信息
        QVariantMap m;
        m[HoleGroupNameField] = mCurrentViewInfo[HoleGroupNameField];// 组装组名称,方便pattern依据组名查找所有孔
        m[HoleGroupColorField] = mCurrentViewInfo[HoleGroupColorField]; // 组装组颜色,可以让pattern把同组内其他可能不相同的颜色全部统一
        m[HoleViewSizeField] = mCurrentViewInfo[HoleViewSizeField]; // 组装视野窗口尺寸
        m[HoleCoordinateField] = mCurrentViewInfo[HoleCoordinateField]; // 坐标信息顺带组装
        m[WellAllGroupsField] = mCurrentViewInfo[WellAllGroupsField]; // 孔板所有组名信息顺带组装
        m[WellAllHolesField] = mCurrentViewInfo[WellAllHolesField]; // 孔板所有选择的孔坐标信息顺带组装

        // 3. 组装当前孔选择的所有视野坐标信息
        auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();// 当前孔坐标
        auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
        QPointVector viewpoints;
        for(int row = 0 ; row < mrows; ++ row) {
            for (int col = 0; col < mcols; ++col) {
                if (mViewSelectPoints[idx][row][col] && !mDisablePoints[row][col]) { // 为true的是被选中的且不能是灰色区域的
                    viewpoints.append(QPoint(row,col)); // 当前孔选择的全部视野坐标
                }
            }
        }
        m[HoleViewPointsField].setValue(viewpoints);
        emit applyHoleEvent(m);

        // 4. 本组或其他组的其他孔不需要更新缓存信息
    }

    void ViewPattern::onApplyGroupAct()
    { // 传递视野窗口的组名+组颜色+视野尺寸+当前孔坐标+所有视野坐标信息+更新同组其它孔的视野信息和临时信息

        // 1. 当前孔没有分过组或者没有保存过点不允许触发应用到组事件
        if (mCurrentViewInfo[HoleGroupNameField].toString().isEmpty() || !viewPointCount())
            return; // 多加一层保护总没坏处

        // 2. 组装组名+组颜色+视野尺寸+当前孔坐标+已有的所有组+本孔选择的所有视野坐标 6个信息 4个关键信息
        QVariantMap m;
        m[HoleGroupNameField] = mCurrentViewInfo[HoleGroupNameField];// 组装组名称,方便pattern依据组名查找所有孔
        m[HoleGroupColorField] = mCurrentViewInfo[HoleGroupColorField]; // 组装组颜色,可以让pattern把同组内其他可能不相同的颜色全部统一
        m[HoleViewSizeField] = mCurrentViewInfo[HoleViewSizeField]; // 组装视野窗口尺寸
        m[HoleCoordinateField] = mCurrentViewInfo[HoleCoordinateField]; // 坐标信息顺带组装
        m[WellAllGroupsField] = mCurrentViewInfo[WellAllGroupsField]; // 孔板所有组名信息顺带组装
        m[WellAllHolesField] = mCurrentViewInfo[WellAllHolesField]; // 孔板所有选择的孔坐标信息顺带组装

        // 3. 组装当前孔选择的所有视野坐标信息
        auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();// 当前孔坐标
        auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
        QPointVector viewpoints;
        for(int row = 0 ; row < mrows; ++ row) {
            for (int col = 0; col < mcols; ++col) {
                if (mViewSelectPoints[idx][row][col] && !mDisablePoints[row][col]) { // 为true选中的且不是灰色区域的
                    viewpoints.append(QPoint(row,col)); // 当前孔选择的全部视野坐标
                }
            }
        }

        m[HoleViewPointsField].setValue(viewpoints);
        //LOG<<"view send viewpoins info to well is 【"<<viewpoints<<"】";//WellPattern::updateGroupByViewInfo接收
        emit applyGroupEvent(m);

        // 4.更新同组其它孔的视野信息和临时信息
        QBool2DVector vec;
        for(int row = 0 ; row < mrows; ++ row) { // 分配空间
            QBoolVector var;
            for (int col = 0; col < mcols; ++col){
                var.append(false);
            }
            vec.append(var);
        }
        for(int r = 0; r < mrows; ++r) {
            for(int c = 0; c < mcols; ++c) {
                if (mViewSelectPoints[idx][r][c] && !mDisablePoints[r][c]) {// 使用当前孔的视野信息更新
                    vec[r][c] = true;
                }
            }
        }
        auto groupPoints = mCurrentViewInfo[HoleGroupCoordinatesField].value<QPointVector>();//拿到本组其它孔的所有孔坐标
                foreach(auto pt, groupPoints) {
                auto pt_idx = pt.x()*PointToIDCoefficient+pt.y(); // 本组其他孔的临时数据区更新为当前孔的视野信息
                mTmpViewSelectPoints[pt_idx] = vec;
                mViewSelectPoints[pt_idx] = vec;
            }
    }

    void ViewPattern::onApplyAllAct()
    { // 应用到所有组
        // 1. 当前孔保存过点不允许触发应用到所有组事件
        if (!viewPointCount())
            return; // 多加一层保护总没坏处

        // 2. 组装组颜色+视野尺寸+视野坐标 3个关键信息足够 + 孔板所有孔坐标
        QVariantMap m;
        m[HoleViewSizeField] = mCurrentViewInfo[HoleViewSizeField]; // 组装视野窗口尺寸
        m[HoleGroupColorField] = mCurrentViewInfo[HoleGroupColorField]; // 组装组颜色,可以把所有组颜色统一(可能没分过组默认颜色红色,无所谓)
        m[HoleGroupNameField] = mCurrentViewInfo[HoleGroupNameField];// 组名信息顺带组装(应用到所有组时只有同组的组颜色需要覆盖)
        //m[HoleCoordinateField] = mCurrentViewInfo[HoleCoordinateField]; // 坐标信息顺带组装
        //m[WellAllGroupsField] = mCurrentViewInfo[WellAllGroupsField]; // 所有组名信息顺带组装
        m[WellAllHolesField] = mCurrentViewInfo[WellAllHolesField]; // 孔板所有选择的孔坐标信息顺带组装

        // 3. 组装当前孔选择的所有视野坐标信息
        auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();// 当前孔坐标
        auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
        QPointVector viewpoints;
        for(int row = 0 ; row < mrows; ++ row) {
            for (int col = 0; col < mcols; ++col) {
                if (mViewSelectPoints[idx][row][col] && !mDisablePoints[row][col]) { // 被选中的且不是灰色区域的视野
                    viewpoints.append(QPoint(row,col)); // 当前孔选择的全部视野坐标
                }
            }
        }
        m[HoleViewPointsField].setValue(viewpoints);
        emit applyAllEvent(m);// 3个信息足够

        // 4.更新其它所有孔的视野信息和临时信息,不区分组
        QBool2DVector vec;
        for(int row = 0 ; row < mrows; ++ row) { // 分配空间
            QBoolVector var;
            for (int col = 0; col < mcols; ++col){
                var.append(false);
                if(mViewSelectPoints[idx][row][col] && !mDisablePoints[row][col]) {
                    var[col] = true; // 优化一下变成1个循环
                }
            }
            vec.append(var);
        }
//    for(int r = 0; r < mrows; ++r) {
//        for(int c = 0; c < mcols; ++c) {
//            if (mViewSelectPoints[idx][r][c] && !mDisablePoints[r][c]) {// 使用当前孔的视野信息更新
//                vec[r][c] = true;
//            }
//        }
//    }
        auto allholes = mCurrentViewInfo[WellAllHolesField].value<QPoint2DVector>();//拿到所有分过组的孔坐标
                foreach(auto holes, allholes) {
                        foreach (auto hole, holes) {
                        auto pt_idx = hole.x()*PointToIDCoefficient+hole.y(); // 所有其他孔的临时数据区更新为当前孔的视野信息
                        mTmpViewSelectPoints[pt_idx] = vec;
                        mViewSelectPoints[pt_idx] = vec;
                    }
            }
    }

    void ViewPattern::initSelectPoints()
    { // setStrategy内调用,构造函数无需调用,更新当前孔的视野数量(分配空间,如果有上次设置过的视野数量信息就更新)

        // 1. 计算当前孔id
        auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();
        auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx

        // 2. 分配空间并赋值给当前id的视野数据区
        QBool2DVector vec;
        for(int row = 0 ; row < mrows; ++ row) {
            QBoolVector var;
            for (int col = 0; col < mcols; ++col){
                var.append(false);
            }
            vec.append(var);
        }
        mViewSelectPoints[idx] = vec; // 这里去分配空间

        // 3. 上次设置的视野数量信息去初始化当前的显示效果不变 (onSaveViewAct保存的临时信息)
        // 确实有上次保存的视野数量信息+其行列数是正确的(brand/objective可能会更改)+如果不符合就清除掉上次的视野数量信息(已经无效)
        if (!mTmpViewSelectPoints[idx].isEmpty() && mTmpViewSelectPoints[idx].count() == mrows) {
            if (!mTmpViewSelectPoints[idx][0].isEmpty() &&mTmpViewSelectPoints[idx][0].count() == mcols) {
                // 行列数相当,那么上次临时保存的值来更新
                mViewSelectPoints[idx] = mTmpViewSelectPoints[idx];
            }
        } else mTmpViewSelectPoints[idx].clear(); // 说明view形状变了无需重现上次的设置
        update();
    }

    int ViewPattern::viewPointCount() const
    { // 计算当前孔已经选择的视野数(控制应用到本组的使能,未分过组或者分过组但是没选过视野)

        auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();
        auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
        auto viewinfo = mViewSelectPoints[idx]; // 当前孔的选择过的视野信息

        if (viewinfo.isEmpty()) return 0; // 有可能这个孔没分配过空间,一层保险
        // 不过setStrategy内initSelectPoints是在调用本函数之前执行过,所以已经分配过空间

        int count = 0;
        for(int row = 0 ; row < mrows; ++ row) {
            for (int col = 0; col < mcols; ++col) {
                if (viewinfo[row][col] && !mDisablePoints[row][col]) { // 视野被选择过的数量,多层保护不包含灰色区域的
                    count++;
                }
            }
        }
        return count;
    }

    void ViewPattern::initDrapPoints()
    { // 拖拽结束后清除这些点
        mDrapRect = QRectF();
        mDrapPoints.clear();
        for(int row = 0 ; row < mrows; ++ row) {
            QBoolVector var;
            for (int col = 0; col < mcols; ++col){
                var.append(false);
            }
            mDrapPoints.append(var);
        }
        update();
    }

    void ViewPattern::initDisablePoints()
    {
        mDisablePoints.clear();
        for(int row = 0 ; row < mrows; ++ row) {
            QBoolVector var;
            for (int col = 0; col < mcols; ++col){
                var.append(false);
            }
            mDisablePoints.append(var);
        }
        update();
    }

    void ViewPattern::setDisablePoint(QCPoint point, bool enable)
    { // 2x3,x只能取0,1,y取0,1,2,不能取等号
        if (point.x()< 0 || point.x() >= mrows // 防止越界,外部也有义务不许越界
            || point.y()<0 || point.y() >= mcols)
            return;
        mDisablePoints[point.x()][point.y()] = enable;
        update();
    }

    void ViewPattern::setDisablePoints(QCPointVector points, bool enable)
    { // 对指定的有限坐标设置
                foreach(auto pt , points) {
                setDisablePoint(pt,enable);
            }
    }

    void ViewPattern::setDisablePoints(bool enable)
    { // 对所有的坐标进行设置
        for(int r = 0; r < mrows; ++r) {
            for(int c = 0; c < mcols; ++c) {
                mDisablePoints[r][c] = enable;
            }
        }
        update();
    }

    int ViewPattern::drapPointCount() const
    { // 计算拖拽区域包含的点个数
        int count = 0;
        for(int r = 0; r < mrows; ++ r) {
            for(int c = 0; c < mcols; ++c) {
                if (mDrapPoints[r][c] && !mDisablePoints[r][c])
                    count++;
            }
        }
        return count;
    }

    ViewInfo ViewPattern::currentViewInfo() const
    {
        return mCurrentViewInfo;
    }

    ViewPattern::ViewPattern(QWidget *parent) : QWidget(parent)
    {
        strategy = NoStrategy;
        mrows = 0;
        mcols = 0;
        mMousePoint = QPoint(-1,-1);
        mLastPos = {-1,-1};
        mMouseClickColor.setAlpha(DefaultColorAlpha);
        mGrayColor.setAlpha(DefaultColorAlpha);
        //initDrapPoints();// 无需调用,构造出来默认不会有拖拽框
        //initDisablePoints(); // 不需要在这初始化,因为mrows,mcols都是0
        //initSelectPoints();//无需调用,因为当前孔未知
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

        saveviewact->setEnabled(true);
        removeviewact->setEnabled(false);
        applygroupact->setEnabled(false);
        applyallact->setEnabled(false);// 应用到所有组可以不需要当前孔是否分了组,但是没有点不能应用

        connect(saveviewact,&QAction::triggered,this,&ViewPattern::onSaveViewAct);
        connect(removeviewact,&QAction::triggered,this,&ViewPattern::onRemoveViewAct);
        connect(applyholeact,&QAction::triggered,this,&ViewPattern::onApplyHoleAct);
        connect(applygroupact,&QAction::triggered,this,&ViewPattern::onApplyGroupAct);
        connect(applyallact,&QAction::triggered,this,&ViewPattern::onApplyAllAct);
    }
}// end namespace v1