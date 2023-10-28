/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-25 09:48:48
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-25 09:49:29
 * @FilePath: \EOS\interface\src\preview\photocanvas.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "previewphotocanvas.h"

void PreviewPhotoCanvas::setStrategy(PreviewPhotoCanvas::DrawStrategy s, const QVariantMap& m)
{ // 设置策略+传递孔的信息+视野尺寸信息+应用到组使能
    // 1. 初始化通用信息
    strategy = s;
    mMousePoint = {-1,-1}; // 必须更新,否则上次的鼠标点还在会导致切换物镜或者brand出现越界
    mLastPos = {-1,-1};
    mDrapRect = QRectF(); // 上次的框选痕迹还在要清除
    initDrapPoints();

    if (strategy != InnerCircleRect) { // 如果不是InnerCircleRect策略,其他变量不用更新
        update();
        return;
    }

    // 2. 初始化InnerCircleRect策略需要的信息
    mCurrentHoleInfo = m; // 来自pattern传递的信息(groupname,groupcolor,grouppoints,coordinate)+objective/brand确定的viewsize信息
    //auto groupcolor = m[GroupColorField].toString();
    //auto coordinate = m[HolePointField].toPoint();
    //auto grouppoints = m[GroupPointsField].value<QPointVector>();

    // 3. 初始化视野尺寸,重新更新
    auto size = m[ViewSizeField].toSize();
    mrows = size.width();
    mcols = size.height();
    initSelectPoints();// 先更新尺寸,才能基于尺寸更新视野已被选择的信息(上次设置的临时信息)

    // 4.更新应用到本组的使能,未分过组或者分过组但是没选过视野
    auto groupname = m[GroupNameField].toString();
    if (!groupname.isEmpty() && viewPointCount()) // 这个孔不属于任何组
        applygroupact->setEnabled(true);
    else applygroupact->setEnabled(false);

    update();
}

void PreviewPhotoCanvas::onSaveViewAct()
{ // 保存选择的视野到当前孔id对应的视野数据区并保存到临时信息用于initSelectPoints重新初始化

    // 1. 计算当前孔的唯一id
    auto coordinate = mCurrentHoleInfo[CoordinateField].toPoint();
    auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
    bool haveSelect = false; // 是否至少选了1个视野,viewPointCount()计算也可以,这里少做次循环

    // 2. 防止框选时鼠标点击的地方漏掉,鼠标点击的坐标也要认为是框选上,一层保障
    if (mMousePoint != QPoint(-1,-1)) {
        mHoleSelectPoints[idx][mMousePoint.x()][mMousePoint.y()] = true;
        haveSelect = true;
        mDrapPoints[mMousePoint.x()][mMousePoint.y()] = false;
    }

    // 3. 把框选区域的视野保存,然后清除框选区域
    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {
            if (mDrapPoints[r][c]) {
                mDrapPoints[r][c] = false;
                mHoleSelectPoints[idx][r][c] = true;
                haveSelect = true;
            }
        }
    }

    // 4. 根据新的保存视野数量去更新使能,但是可能没分过组,2个共同控制
    if (!mCurrentHoleInfo[GroupNameField].toString().isEmpty() && haveSelect)
        applygroupact->setEnabled(true);// 如果没分组还是不允许应用到本组
    else applygroupact->setEnabled(false);

    // 5. 每次保存视野的数量位置信息都保存到临时信息,用于更新下次的重新保存的初始化(initSelectPoints内更新)
    mTmpHoleSelectPoints[idx] = mHoleSelectPoints[idx]; // 临时保存这次设置
    update();
}

void PreviewPhotoCanvas::onApplyGroupAct()
{ // 传递视野窗口的组名+组颜色+视野尺寸+当前孔坐标+所有视野坐标信息+更新同组其它孔的视野信息和临时信息

    // 1. 当前孔没有分过组或者没有保存过点不允许触发应用到组事件
    if (mCurrentHoleInfo[GroupNameField].toString().isEmpty() || !viewPointCount())
        return; // ,多加一层保护总没坏处

     // 2. 组装组名+组颜色+视野尺寸+当前孔坐标
    QVariantMap m;
    m[GroupNameField] = mCurrentHoleInfo[GroupNameField];// 组装组名称,方便pattern依据组名查找所有孔
    m[GroupColorField] = mCurrentHoleInfo[GroupColorField]; // 组装组颜色,可以让pattern把同组内其他可能不相同的颜色全部统一
    m[ViewSizeField] = mCurrentHoleInfo[ViewSizeField]; // 这是为了pattern画点使用
    m[CoordinateField] = mCurrentHoleInfo[CoordinateField]; // 坐标信息顺带组装

    // 3. 组装当前孔选择的所有视野坐标信息
    auto coordinate = mCurrentHoleInfo[CoordinateField].toPoint();
    auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
    QPointVector points;
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            if (mHoleSelectPoints[idx][row][col]) {
                points.append(QPoint(row,col)); // 当前孔选择的全部视野坐标
            }
        }
    }
    QVariant v;
    v.setValue(points);
    m[PointsField] = v;
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
            if (mHoleSelectPoints[idx][r][c]) {// 使用当前孔的视野信息更新
                vec[r][c] = true;
            }
        }
    }
    auto groupPoints = mCurrentHoleInfo[GroupPointsField].value<QPointVector>();//拿到本组其它孔的所有坐标
    foreach(auto pt, groupPoints) {
        auto pt_idx = pt.x()*PointToIDCoefficient+pt.y(); // 本组其他孔的临时数据区更新为当前孔的视野信息
        mTmpHoleSelectPoints[pt_idx] = vec;
        mHoleSelectPoints[pt_idx] = vec;
    }
}

void PreviewPhotoCanvas::onApplyAllAct()
{ // 应用到所有组

}

void PreviewPhotoCanvas::initSelectPoints()
{ // setStrategy内调用,构造函数无需调用,更新当前孔的视野数量(分配空间,如果有上次设置过的视野数量信息就更新)

    // 1. 计算当前孔id
    auto coordinate = mCurrentHoleInfo[CoordinateField].toPoint();
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
    mHoleSelectPoints[idx] = vec; // 这里去分配空间

    // 3. 上次设置的视野数量信息去初始化当前的显示效果不变 (onSaveViewAct保存的临时信息)
    // 确实有上次保存的视野数量信息+其行列数是正确的(brand/objective可能会更改)+如果不符合就清除掉上次的视野数量信息(已经无效)
    if (!mTmpHoleSelectPoints[idx].isEmpty() && mTmpHoleSelectPoints[idx].count() == mrows) {
        if (!mTmpHoleSelectPoints[idx][0].isEmpty() &&mTmpHoleSelectPoints[idx][0].count() == mcols) {
            // 行列数相当,那么上次临时保存的值来更新
            mHoleSelectPoints[idx] = mTmpHoleSelectPoints[idx];
        }
    } else mTmpHoleSelectPoints[idx].clear(); // 说明view形状变了无需重现上次的设置

    update();
}

int PreviewPhotoCanvas::viewPointCount() const
{ // 计算当前孔已经选择的视野数(控制应用到本组的使能,未分过组或者分过组但是没选过视野)

    auto coordinate = mCurrentHoleInfo[CoordinateField].toPoint();
    auto idx = coordinate.x()*PointToIDCoefficient+coordinate.y();// 保证索引唯一不重叠2k+y,每个孔对应唯一的idx
    auto holeinfo = mHoleSelectPoints[idx]; // 当前孔的选择过的视野信息

    if (holeinfo.isEmpty()) return 0; // 有可能这个孔没分配过空间,一层保险
    // 不过setStrategy内initSelectPoints是在调用本函数之前执行过,所以已经分配过空间

    int count = 0;
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            if (holeinfo[row][col]) { // 视野被选择过的数量
                count++;
            }
        }
    }
    return count;
}

void PreviewPhotoCanvas::initDrapPoints()
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

int PreviewPhotoCanvas::drapPointCount() const
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

QVariantMap PreviewPhotoCanvas::currentHoleInfo() const
{ // 目的是为了切换brand,objective时外部拿到当前的视野孔信息,然后更新其中尺寸字段即可
    // 然后再次调用setStrategy->initSelectPoints()->更新视野信息/临时信息(不匹配时临时信息就被清除了)
    return mCurrentHoleInfo;
}

PreviewPhotoCanvas::PreviewPhotoCanvas(QWidget *parent) : QWidget(parent)
{
    strategy = NoStrategy;
    mrows = 0;
    mcols = 0;
    mMousePoint = QPoint(-1,-1);
    mLastPos = {-1,-1};
    mExternalCircleRectSize = DefaultPreviewPhotoCanvasViewRectSize;
    mMouseClickColor.setAlpha(PatternColorAlpha);
    //initDrapPoints();// 无需调用,构造出来默认不会有拖拽框
    //initSelectPoints();//无需调用,因为当前孔未知


    saveviewact = new QAction(tr("选点"));
    applygroupact = new QAction(tr("应用到本组"));
    applyallact = new QAction(tr("应用到所有"));
    applygroupact->setEnabled(false);
    addAction(saveviewact);
    addAction(applygroupact);
    addAction(applyallact);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    connect(saveviewact,&QAction::triggered,this,&PreviewPhotoCanvas::onSaveViewAct);
    connect(applygroupact,&QAction::triggered,this,&PreviewPhotoCanvas::onApplyGroupAct);
    connect(applyallact,&QAction::triggered,this,&PreviewPhotoCanvas::onApplyAllAct);
}
