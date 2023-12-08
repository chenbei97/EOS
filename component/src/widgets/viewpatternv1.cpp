/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-05 08:58:55
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-05 09:15:58
 * @FilePath: \EOS\component\src\widgets\viewpatternv1.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "ViewPatternv1.h"

namespace obsoleteClass {
    void ViewPatternV1::setStrategy(ViewPatternV1::DrawStrategy s, const QVariantMap& m)
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
        auto wellallgroup = m[HoleAllGroupsField].value<QSet<QString>>();
        auto wellallholes =m[HoleAllCoordinatesField].value<QPoint2DVector>();

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

    void ViewPatternV1::clearViewWindowCache(const QPoint &holepoint)
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

    void ViewPatternV1::clearAllViewWindowCache(int viewsize)
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

    void ViewPatternV1::updateViewWindowCache(QCPoint holepoint, QCPointVector viewpoints,int viewsize)
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

    void ViewPatternV1::onSaveViewAct()
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

    void ViewPatternV1::onRemoveViewAct()
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

    void ViewPatternV1::onApplyHoleAct()
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
        m[HoleAllGroupsField] = mCurrentViewInfo[HoleAllGroupsField]; // 孔板所有组名信息顺带组装
        m[HoleAllCoordinatesField] = mCurrentViewInfo[HoleAllCoordinatesField]; // 孔板所有选择的孔坐标信息顺带组装

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

    void ViewPatternV1::onApplyGroupAct()
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
        m[HoleAllGroupsField] = mCurrentViewInfo[HoleAllGroupsField]; // 孔板所有组名信息顺带组装
        m[HoleAllCoordinatesField] = mCurrentViewInfo[HoleAllCoordinatesField]; // 孔板所有选择的孔坐标信息顺带组装

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

    void ViewPatternV1::onApplyAllAct()
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
        //m[HoleAllGroupsField] = mCurrentViewInfo[HoleAllGroupsField]; // 所有组名信息顺带组装
        m[HoleAllCoordinatesField] = mCurrentViewInfo[HoleAllCoordinatesField]; // 孔板所有选择的孔坐标信息顺带组装

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
        auto allholes = mCurrentViewInfo[HoleAllCoordinatesField].value<QPoint2DVector>();//拿到所有分过组的孔坐标
                foreach(auto holes, allholes) {
                        foreach (auto hole, holes) {
                        auto pt_idx = hole.x()*PointToIDCoefficient+hole.y(); // 所有其他孔的临时数据区更新为当前孔的视野信息
                        mTmpViewSelectPoints[pt_idx] = vec;
                        mViewSelectPoints[pt_idx] = vec;
                    }
            }
    }

    void ViewPatternV1::initSelectPoints()
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

    int ViewPatternV1::viewPointCount() const
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

    void ViewPatternV1::initDrapPoints()
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

    void ViewPatternV1::initDisablePoints()
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

    void ViewPatternV1::setDisablePoint(QCPoint point, bool enable)
    { // 2x3,x只能取0,1,y取0,1,2,不能取等号
        if (point.x()< 0 || point.x() >= mrows // 防止越界,外部也有义务不许越界
            || point.y()<0 || point.y() >= mcols)
            return;
        mDisablePoints[point.x()][point.y()] = enable;
        update();
    }

    void ViewPatternV1::setDisablePoints(QCPointVector points, bool enable)
    { // 对指定的有限坐标设置
                foreach(auto pt , points) {
                setDisablePoint(pt,enable);
            }
    }

    void ViewPatternV1::setDisablePoints(bool enable)
    { // 对所有的坐标进行设置
        for(int r = 0; r < mrows; ++r) {
            for(int c = 0; c < mcols; ++c) {
                mDisablePoints[r][c] = enable;
            }
        }
        update();
    }

    int ViewPatternV1::drapPointCount() const
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

    ViewInfo ViewPatternV1::currentViewInfo() const
    {
        return mCurrentViewInfo;
    }

    ViewPatternV1::ViewPatternV1(QWidget *parent) : QWidget(parent)
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

        connect(saveviewact,&QAction::triggered,this,&ViewPatternV1::onSaveViewAct);
        connect(removeviewact,&QAction::triggered,this,&ViewPatternV1::onRemoveViewAct);
        connect(applyholeact,&QAction::triggered,this,&ViewPatternV1::onApplyHoleAct);
        connect(applygroupact,&QAction::triggered,this,&ViewPatternV1::onApplyGroupAct);
        connect(applyallact,&QAction::triggered,this,&ViewPatternV1::onApplyAllAct);
    }

    void ViewPatternV1::drawSelectRect(QPainter &painter)
    { // 根据当前孔的组颜色去绘制视野已经被选择的小矩形区域

        // 计算当前孔的唯一id
        auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();
        auto groupcolor = mCurrentViewInfo[HoleGroupColorField].toString();
        auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx


        if (mViewSelectPoints[idx].isEmpty()) return; // 删孔后清除缓存信息这里是空直接返回否则遍历越界
        auto  rects = getInnerRects();
        // 绘制框选的所有视野
        for(int row = 0 ; row < mrows; ++ row) {
            for (int col = 0; col < mcols; ++col) {
                auto rect = rects[row][col];
                if (mViewSelectPoints[idx][row][col] && !mDisablePoints[row][col]) { // 多层保护,不可选的视野不允许绘制
                    painter.fillRect(rect,groupcolor);
                }
//            else { // 不加也行,就是删除点时候要恢复成白色,默认也是白色
//                painter.fillRect(rect,Qt::white);
//            }
            }
        }
    }

    void ViewPatternV1::drawDrapRect(QPainter &painter)
    {// 绘制推拽矩形框包含的小矩形区域
        auto  rects = getInnerRects();

        // 绘制框选的所有视野
        for(int row = 0 ; row < mrows; ++ row) {
            for (int col = 0; col < mcols; ++col) {
                if (mDrapPoints[row][col] && !mDisablePoints[row][col]) { // 多层保护,不可选的视野不允许绘制
                    auto rect = rects[row][col];
                    painter.fillRect(rect,mMouseClickColor);
                }
            }
        }
    }

    void ViewPatternV1::drawInnerLine(QPainter &painter)
    {// 画窗口的线区分出小矩形区域
        // 绘制外边框
        auto p11 = getInnerRectTopLeftPoint();
        auto p12 = getInnerRectTopRightPoint();
        auto p21 = getInnerRectBottomLeftPoint();
        auto p22 = getInnerRectBottomRightPoint();

        painter.drawLine(p11,p21);
        painter.drawLine(p11,p12);
        painter.drawLine(p12,p22);
        painter.drawLine(p21,p22);

        // 绘制内部线
//    auto tops = getInnerVerticalLineTopPoints();
//    auto bottoms = getInnerVerticalLineBottomPoints();
//    auto lefts = getInnerHorizonalLineLeftPoints();
//    auto rights = getInnerHorizonalLineRightPoints();

//    Q_ASSERT(tops.count() == bottoms.count());
//    Q_ASSERT(lefts.count() == rights.count());
//    for(int i = 0; i < tops.count(); i++) {
//        painter.drawLine(tops.at(i),bottoms.at(i));
//    }
//    for(int i = 0; i < lefts.count(); i++) {
//        painter.drawLine(lefts.at(i),rights.at(i));
//    }
        // 优化一下改为直接画线,少做几次循环操作
        auto hor_offset = getInnerRectWidth();
        for (int i = 1; i <= mrows-1; ++i) {
            auto top = p11 + QPointF(i*hor_offset,0);
            auto bottom = p21 + QPointF(i*hor_offset,0);
            painter.drawLine(top,bottom);
        }
        auto ver_offset = getInnerRectHeight(); // 点之间y坐标相差的是小矩形高度
        for (int i = 1; i <= mcols-1; ++i){
            auto top = p11 + QPointF(0,ver_offset*i);
            auto bottom = p12 + QPointF(0,ver_offset*i);
            painter.drawLine(top,bottom);
        }

        // 高亮鼠标区域
        auto rects = getInnerRects();
        //LOG<<"rects.size = "<<rects.size()<<" mouse = "<<mMousePoint;
        // rects.size =  4  mouse =  QPoint(7,6)
        // 原来10x10,鼠标可能点的(7,6),现在切换尺寸4x4,导致绘制鼠标的高亮越界! 所以初始化时必须重置鼠标
        if (mMousePoint != QPoint(-1,-1)) // 尚未点击
            painter.fillRect(rects[mMousePoint.x()][mMousePoint.y()],mMouseClickColor);
    }

    void ViewPatternV1::drawCircle(QPainter &painter)
    {// 画窗口的内接圆
        auto radius = getCircleRadius();
        painter.drawEllipse(QPointF(width()/2.0,height()/2.0),radius,radius);
    }

    void ViewPatternV1::drawDisbaleRect(QPainter &painter)
    {
        auto  rects = getInnerRects();
        // 绘制置灰的所有视野
        for(int row = 0 ; row < mrows; ++ row) {
            for (int col = 0; col < mcols; ++col) {
                if (mDisablePoints[row][col]) {
                    auto rect = rects[row][col];
                    painter.fillRect(rect,mGrayColor);
                    painter.drawLine(rect.topLeft(),rect.bottomRight());
                    painter.drawLine(rect.topRight(),rect.bottomLeft());
                }
            }
        }
    }

    void ViewPatternV1::paintEvent(QPaintEvent *event)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        auto pen = painter.pen();
        pen.setWidth(DefaultPainterPenWidth);
        painter.setPen(pen);

        if (strategy == NoStrategy)
        {
            event->accept();
            return;
        }


        drawDrapRect(painter); // 1.绘制拖拽区域最前
        drawSelectRect(painter); // 2. 被选中的高亮其次,要覆盖拖拽区域
        drawDisbaleRect(painter); // 3. 置灰区域覆盖被选中的区域再其次
        drawCircle(painter); // 4. 画线画圆防止被覆盖,最后
        drawInnerLine(painter);
        // 绘制框,最后画防止被覆盖
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

    void ViewPatternV1::mousePressEvent(QMouseEvent *event)
    { // 左键点击清除框选,计算鼠标点击的小矩形区域坐标
        if (event->button() == Qt::LeftButton) {
            initDrapPoints(); // 框选后，如果左键点一下应该取消框选
            mDrapRect.setWidth(0);
            mDrapRect.setHeight(0);
            mDrapRect = QRectF();
            update();
        } // 右键是菜单

        mLastPos = event->pos();
        mMousePoint = {-1,-1};
        auto rects = getInnerRects();
        for(int r = 0; r < mrows; ++r) {
            for(int c = 0; c < mcols; ++c) {
                if (rects[r][c].contains(mLastPos) && !mDisablePoints[r][c]) {
                    mMousePoint = {r,c}; // 多层保护,不可选的视野视为-1,-1,这样去设置使能 ①,②做法均可
                }
            }
        }
//    if (mMousePoint != QPoint(-1,-1)) {
//        if (mDisablePoints[mMousePoint.x()][mMousePoint.y()])
//            mMousePoint = {-1,-1}; // 多层保护,不可选的视野视为-1,-1,这样去设置使能,①,②做法均可
//    }

        // 右键时如果这个视野没被选择过不能删除点
        auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();//当前视野窗口隶属的孔坐标
        auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
        auto viewpoints = mViewSelectPoints[idx];// 拿到本孔对应的视野坐标
        int viewcount = viewPointCount(); // 本孔选择的视野数量

        auto groupname = mCurrentViewInfo[HoleGroupNameField].toString();
        if (!groupname.isEmpty() && viewcount)
            applygroupact->setEnabled(true);
        else applygroupact->setEnabled(false);//如果视野数量=0或者本孔未分组不能应用到本组

        // 视野数量=0不能应用到所有组
        viewcount?applyallact->setEnabled(true):applyallact->setEnabled(false);
        // 视野等于=0不能删点,这里改为当前鼠标右击的视野坐标是否已被选择过也可以
        bool isselect = false;
        if (mMousePoint != QPoint(-1,-1)) // 防止下方索引越界,如果是可选的视野坐标再去判断是否被选中
            isselect = viewpoints[mMousePoint.x()][mMousePoint.y()];
        isselect?removeviewact->setEnabled(true):removeviewact->setEnabled(false);

        update();
        emit mouseClicked(mMousePoint);
        event->accept();
    }

    void ViewPatternV1::mouseMoveEvent(QMouseEvent *event)
    { // 绘制拖拽框
        if (event->buttons() & Qt::LeftButton){
            initDrapPoints(); // 清除拖拽区域
            auto end = event->pos(); // 鼠标停下的点
            mDrapRect = QRectF(mLastPos,end); // 鼠标形成的矩形框
            auto rects = getInnerRects();
            for(int row = 0; row < mrows; ++row)
                for(int col = 0; col < mcols; ++col) {
                    if(mDrapRect.intersects(rects[row][col]) // 小矩形区域在这个推拽区域内有交集
                       && !mDisablePoints[row][col]){ //根源在mDrapPoints直接限制好,paintEvent不加
                        mDrapPoints[row][col] = true;//!mDisablePoints[row][col]判断也可以
                    }
                }
        }
        update();
        event->accept();
    }

    void ViewPatternV1::mouseReleaseEvent(QMouseEvent *event)
    { // 拖拽区域点个数为0才是预览事件(这里不能对applyall,applygroup,remove的使能操作)

        // 1. 点到边缘不能保存点
        if (mMousePoint == QPoint(-1,-1) ){ // 右击不触发
            saveviewact->setEnabled(false);
            removeviewact->setEnabled(false);
            applyholeact->setEnabled(false);
            applygroupact->setEnabled(false);
            applyallact->setEnabled(false);
            return; // 可能会点到边缘位置,不能选点,由于绘图没有死区,这不设置也行
        }

        // 2. 点到不能选的孔也不能保存点
        if (mDisablePoints[mMousePoint.x()][mMousePoint.y()]) {
            saveviewact->setEnabled(false);
            removeviewact->setEnabled(false);
            applyholeact->setEnabled(false);
            applygroupact->setEnabled(false);
            applyallact->setEnabled(false);
            return;
        }

        saveviewact->setEnabled(true);
        removeviewact->setEnabled(true);
        applyholeact->setEnabled(true);
        applygroupact->setEnabled(true);
        applyallact->setEnabled(true);

        if (drapPointCount()<1 ) // 没有拖拽区域单击才是预览事件
        {
            LOG<<"preview event is emit";
            emit previewEvent(mMousePoint);
        }

        event->accept();
    }

    double ViewPatternV1::getCircleRadius() const
    { // 视野圆半径
        return width()>=height()?height()/2.0:width()/2.0;
    }

    double ViewPatternV1::getInnerRectWidth() const
    {// 小矩形区域的宽度
        return 2 * getCircleRadius() / mrows *1.0;
    }

    double ViewPatternV1::getInnerRectHeight() const
    {// 小矩形区域的高度
        return 2 * getCircleRadius() / mcols *1.0;
    }

    QPointF ViewPatternV1::getInnerRectTopLeftPoint() const
    {// 外接正方形左上角顶点
        return QPointF(width()/2.0-getCircleRadius(),0.0);
    }

    QPointF ViewPatternV1::getInnerRectTopRightPoint() const
    {// 外接正方形右上角顶点
        return QPointF(width()/2.0+getCircleRadius(),0.0);
    }

    QPointF ViewPatternV1::getInnerRectBottomLeftPoint() const
    {// 外接正方形左下角顶点
        return QPointF(width()/2.0-getCircleRadius(),height());
    }

    QPointF ViewPatternV1::getInnerRectBottomRightPoint() const
    {// 外接正方形右下角顶点
        return QPointF(width()/2.0+getCircleRadius(),height());
    }

    QPointFVector ViewPatternV1::getInnerVerticalLineTopPoints() const
    {// 正方形顶侧的等分点
        QPointFVector vec;
        auto offset = getInnerRectWidth();
        auto topleft= getInnerRectTopLeftPoint();
        for (int i = 1; i <= mrows-1; ++i)
            vec.append(topleft+ QPoint(i*offset,0));
//    LOG<<"topleft = "<<topleft<<" offset = "<<offset<<" mrows = "<<mrows
//    <<" radius = "<<getExternalCircleRadius();
        return vec;
    }

    QPointFVector ViewPatternV1::getInnerVerticalLineBottomPoints() const
    {// 正方形底侧的等分点
        QPointFVector vec;
        auto offset = getInnerRectWidth();
        auto bottomleft= getInnerRectBottomLeftPoint();
        for (int i = 1; i <= mrows-1; ++i)
            vec.append(bottomleft+ QPoint(i*offset,0));
        return vec;
    }

    QPointFVector ViewPatternV1::getInnerHorizonalLineLeftPoints() const
    {// 正方形左侧的等分点
        QPointFVector vec;
        auto offset = getInnerRectHeight(); // 点之间y坐标相差的是小矩形高度
        auto topleft = getInnerRectTopLeftPoint();
        for (int i = 1; i <= mcols-1; ++i) // i = 1开始是不包含topleft
            vec.append(topleft+ QPoint(0,i*offset)); // x不变,y增加
        return vec;
    }

    QPointFVector ViewPatternV1::getInnerHorizonalLineRightPoints() const
    {// 正方形右侧的等分点
        QPointFVector vec;
        auto offset = getInnerRectHeight(); // 点之间y坐标相差的是小矩形高度
        auto topright = getInnerRectTopRightPoint();
        for (int i = 1; i <= mcols-1; ++i) // i = 1开始是不包含topright
            vec.append(topright+ QPoint(0,i*offset)); // x不变,y增加
        return vec;
    }

    QRectF2DVector ViewPatternV1::getInnerRects() const
    {
        QRectF2DVector m;
        auto hoffset = getInnerRectWidth();
        auto voffset = getInnerRectHeight();
        auto start = getInnerRectTopLeftPoint();
        for(int i = 0 ; i < mrows; ++i) {
            QRectFVector vec;
            for(int j = 0; j < mcols; ++j) { // j*offset加在x坐标也就是水平方向
                auto topleft = start + QPointF(j*hoffset,i*voffset); // x, x+d，j依次取0,1; y先不变在后
                auto bottomright = topleft + QPointF(hoffset,voffset);
                vec.append(QRectF(topleft,bottomright));
            }
            m.append(vec);
        }

        return m;
    }

    void ViewPatternV1::updatePatternUi()
    { // objective更新后视野窗口更新了,但是孔图案的ui绘制点还在,要刷新一下(本函数弃用,Preview::updateViewPatternUi内调用setStrategy和initHoeInfo更新了ui)

        // 1. 拿到当前孔的id
        auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();
        auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx

        // 2. 清除选择的视野信息
        for(int r = 0; r < mrows; ++r) {
            for(int c = 0; c < mcols; ++c) {//调用updateApplyGroup之前已经setStrategy更新了mrows,mcols并分配空间不会越界
                mViewSelectPoints[idx][r][c] = false;
            }
        }
        mTmpViewSelectPoints[idx].clear(); // 以前保存的临时信息清空,其实被setStrategy清空过了,视野尺寸不配的时候,加层保护

        // 3. 重新组装数据,除了viewpoint是空的其它不变(pattern的组颜色+名称+坐标这些是不用动的)
        QVariantMap m;
        m[HoleGroupNameField] = mCurrentViewInfo[HoleGroupNameField];// 组装组名称,方便pattern依据组名查找所有孔
        m[HoleGroupColorField] = mCurrentViewInfo[HoleGroupColorField]; // 组装组颜色,可以让pattern把同组内其他可能不相同的颜色全部统一
        m[HoleViewSizeField] = mCurrentViewInfo[HoleViewSizeField]; // 这是为了pattern画点使用
        m[HoleCoordinateField] = mCurrentViewInfo[HoleCoordinateField]; // 坐标信息顺带组装
        m[HoleAllGroupsField] = mCurrentViewInfo[HoleAllGroupsField]; // 所有组名信息顺带组装
        m[HoleViewRectsField].setValue(QPointVector());//去更新mViewInfo[row][col].viewpoints就是空的
        emit applyGroupEvent(m);

        // 4. 还应该把其它组的所有孔信息也清除,不仅仅是本组的孔(触发应用到所有组的孔)
                foreach(auto groupname, m[HoleAllGroupsField].value<QSet<QString>>()) {
                m[HoleGroupNameField] = groupname;
                emit applyGroupEvent(m);
            }
        // 这些工作已经在clearViewInfo做了

        // 5.同时更新同组其它孔的视野信息和临时信息也都重新分配空间并清空
        auto groupPoints = mCurrentViewInfo[HoleGroupCoordinatesField].value<QPointVector>();//拿到本组其它孔的所有坐标
                foreach(auto pt, groupPoints) {
                auto pt_idx = pt.x()*PointToIDCoefficient+pt.y(); // 本组其他孔的临时数据区更新为当前孔的视野信息
                mTmpViewSelectPoints[pt_idx] = mViewSelectPoints[idx]; //mViewSelectPoints[idx]已经分配过空间全是false
                mViewSelectPoints[pt_idx] = mViewSelectPoints[idx];
            }

        //applygroupact->trigger();// 如果没选择过视野不能触发,所以只能多写上述代码
    }
}