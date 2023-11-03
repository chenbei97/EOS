/***
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-25 09:48:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 09:49:29
 * @FilePath: \EOS\interface\src\preview\viewpattern.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved.
 */

#include "viewpattern.h"

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

void ViewPattern::onSaveViewAct()
{ // 保存选择的视野到当前孔id对应的视野数据区并保存到临时信息用于initSelectPoints重新初始化

    // 1. 计算当前孔的唯一id
    auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();
    auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
    bool haveSelect = false; // 是否至少选了1个视野,viewPointCount()计算也可以,这里少做次循环

    // 2. 防止框选时鼠标点击的地方漏掉,鼠标点击的坐标也要认为是框选上,一层保障
    if (mMousePoint != QPoint(-1,-1)) {
        mViewSelectPoints[idx][mMousePoint.x()][mMousePoint.y()] = true;
        haveSelect = true;
        mDrapPoints[mMousePoint.x()][mMousePoint.y()] = false;
    }

    // 3. 把框选区域的视野保存,然后清除框选区域
    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {
            if (mDrapPoints[r][c]) {
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
    update();
}

void ViewPattern::onRemoveViewAct()
{
    // 1.判断是否允许删点
    if (mMousePoint==QPoint(-1,-1) || !viewPointCount())
        return; // 一层保障

    // 2. 计算当前孔的唯一id拿到当前孔对应视野信息
    auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();
    auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
    auto currentviewinfo = mViewSelectPoints[idx];

    // 3. 更新mViewSelectPoints视野信息
    currentviewinfo[mMousePoint.x()][mMousePoint.y()] = false;
    for(int r = 0; r < mrows; ++r) { // 鼠标+框选的都可以删除
        for(int c = 0; c < mcols; ++c) {
            if (mDrapPoints[r][c]) {
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

    update();
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
            if (mViewSelectPoints[idx][row][col]) {
                viewpoints.append(QPoint(row,col)); // 当前孔选择的全部视野坐标
            }
        }
    }
    QVariant v;
    v.setValue(viewpoints);
    m[HoleViewPointsField] = v;
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
            if (mViewSelectPoints[idx][r][c]) {// 使用当前孔的视野信息更新
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
    //m[HoleGroupNameField] = mCurrentViewInfo[HoleGroupNameField];// 组名信息顺带组装(应用到所有组时组名信息不重要了)
    //m[HoleCoordinateField] = mCurrentViewInfo[HoleCoordinateField]; // 坐标信息顺带组装
    //m[WellAllGroupsField] = mCurrentViewInfo[WellAllGroupsField]; // 所有组名信息顺带组装
    m[WellAllHolesField] = mCurrentViewInfo[WellAllHolesField]; // 孔板所有选择的孔坐标信息顺带组装

    // 3. 组装当前孔选择的所有视野坐标信息
    auto coordinate = mCurrentViewInfo[HoleCoordinateField].toPoint();// 当前孔坐标
    auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
    QPointVector viewpoints;
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            if (mViewSelectPoints[idx][row][col]) {
                viewpoints.append(QPoint(row,col)); // 当前孔选择的全部视野坐标
            }
        }
    }
    QVariant v;
    v.setValue(viewpoints);
    m[HoleViewPointsField] = v;
    emit applyAllEvent(m);// 3个信息足够

    // 4.更新其它所有孔的视野信息和临时信息,不区分组
    QBool2DVector vec;
    for(int row = 0 ; row < mrows; ++ row) { // 分配空间
        QBoolVector var;
        for (int col = 0; col < mcols; ++col){
            var.append(false);
            if(mViewSelectPoints[idx][row][col]) {
                var[col] = true; // 优化一下变成1个循环
            }
        }
        vec.append(var);
    }
//    for(int r = 0; r < mrows; ++r) {
//        for(int c = 0; c < mcols; ++c) {
//            if (mViewSelectPoints[idx][r][c]) {// 使用当前孔的视野信息更新
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
            if (viewinfo[row][col]) { // 视野被选择过的数量
                count++;
            }
        }
    }
    return count;
}

void ViewPattern::initDrapPoints()
{ // 拖拽结束后清除这些点
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

int ViewPattern::drapPointCount() const
{ // 计算拖拽区域包含的点个数
    int count = 0;
    for(int r = 0; r < mrows; ++ r) {
        for(int c = 0; c < mcols; ++c) {
            if (mDrapPoints[r][c])
                count++;
        }
    }
    return count;
}

ViewInfo ViewPattern::currentViewInfo() const
{ // 目的是为了切换brand,objective时外部拿到当前的视野孔信息,然后更新其中尺寸字段即可
    // 然后再次调用setStrategy->initSelectPoints()->更新视野信息/临时信息(不匹配时临时信息就被清除了)
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
    //initDrapPoints();// 无需调用,构造出来默认不会有拖拽框
    //initSelectPoints();//无需调用,因为当前孔未知
    saveviewact = new QAction(tr("选点"));
    removeviewact = new QAction(tr("删点"));
    applygroupact = new QAction(tr("应用到本组"));
    applyallact = new QAction(tr("应用到所有组"));
    addAction(saveviewact);
    addAction(removeviewact);
    addAction(applygroupact);
    addAction(applyallact);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    saveviewact->setEnabled(true);
    removeviewact->setEnabled(false);
    applygroupact->setEnabled(false);
    applyallact->setEnabled(false);// 应用到所有组可以不需要当前孔是否分了组,但是没有点不能应用

    connect(saveviewact,&QAction::triggered,this,&ViewPattern::onSaveViewAct);
    connect(removeviewact,&QAction::triggered,this,&ViewPattern::onRemoveViewAct);
    connect(applygroupact,&QAction::triggered,this,&ViewPattern::onApplyGroupAct);
    connect(applyallact,&QAction::triggered,this,&ViewPattern::onApplyAllAct);
}
