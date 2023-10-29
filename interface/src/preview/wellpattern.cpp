/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-27 08:53:17
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-27 08:54:02
 * @FilePath: \EOS\interface\src\preview\wellpattern.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "wellpattern.h"

void WellPattern::updateGroupByGroupInfo(QCVariantMap m)
{ // 右击分组将分组窗口的组信息(color+name)+拖拽区域信息去更新孔信息(不需要更新points,viewsize)

    // 1. 更新孔的信息
    auto gcolor = m[GroupColorField].toString();
    auto gname = m[GroupNameField].toString();
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col){
            auto pt = mDrapPoints[row][col]; // 右击分组时当前拖拽区域内的点是一组
            if (!gname.isEmpty() && !mHoleInfo[row][col].allgroup.contains(gname))
                mHoleInfo[row][col].allgroup.append(gname); // 不分拖拽区域,所有孔都更新相同的组别信息
            if (pt){ // 是拖拽区域的
                mHoleInfo[row][col].isselected = true;//框选内对应的点都设为选中
                mHoleInfo[row][col].color = gcolor; // 颜色跟随分组窗口设置的颜色
                mHoleInfo[row][col].group = gname; // 名称是分组窗口设置的分组
                mHoleInfo[row][col].coordinate = QPoint(row,col); // 孔坐标
                mDrapPoints[row][col] = false; // 拖拽区域内的点也要更新为false,不然还会绘制这个区域
            }
        }
    }
    LOG<<mHoleInfo[0][0].allgroup;
    // 2. 框选的时候会遗漏鼠标当前选中的点
    if (mMousePos != QPoint(-1,-1)) {// 没启用鼠标事件或者点击外围,这是{-1,-1}会越界
        mHoleInfo[mMousePos.x()][mMousePos.y()].isselected = true; // 鼠标点击的这个
        mHoleInfo[mMousePos.x()][mMousePos.y()].color = gcolor;
        mHoleInfo[mMousePos.x()][mMousePos.y()].group = gname;
        if (!gname.isEmpty() && !mHoleInfo[mMousePos.x()][mMousePos.y()].allgroup.contains(gname))
            mHoleInfo[mMousePos.x()][mMousePos.y()].allgroup.append(gname);
        mHoleInfo[mMousePos.x()][mMousePos.y()].coordinate = mMousePos;
        mDrapPoints[mMousePos.x()][mMousePos.y()] = false;
    }
    update();
}

void WellPattern::updateGroupByViewInfo(QCVariantMap m)
{// 应用到组-视野窗口的信息拿去更新数据(PreviewPhotoCanvas::onApplyGroupAct())
  // 数据包括视野窗口的组名+组颜色+视野尺寸+当前孔坐标+所有组名+所有视野坐标信息

    // 1. 拿到应用到组视野窗口传递来的5个信息
    auto groupName = m[GroupNameField].toString();
    auto groupColor = m[GroupColorField].toString();
    auto coordinate = m[CoordinateField].toPoint();
    auto viewsize = m[ViewSizeField].toSize();
    auto viewpoints = m[ViewPointsField].value<QPointVector>();
    auto allgroup = m[AllGroupsField].toStringList();
    //LOG<<groupColor<<groupName<<coordinate<<viewpoints<<viewsize<<allgroup;

    // 2. 根据视野窗口传来的组名 把coordinate对应的组(color+viewpoints,viewsize)都更新 (不需要更新isselected,group,allgroup)
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            auto holeinfo = mHoleInfo[row][col];
            if (holeinfo.group == groupName) { // 本组的
                //Q_ASSERT(holeinfo.coordinate == coordinate); // 组其它孔坐标和当前传递的孔坐标不同
               // Q_ASSERT(holeinfo.color == groupColor); // 本组不同孔颜色可能不同
                // Q_ASSERT(holeinfo.isselected == true); // 不肯定是被选中的孔,切换objective时会更新mrows,此时isselected可能false
                Q_ASSERT(holeinfo.allgroup == allgroup);
                holeinfo.isselected = true; // 要设置孔为选中,不然就不能绘制高亮了
                holeinfo.color = groupColor; // 本组应用的组颜色(有可能同组不同孔的颜色不同,帮助统一化)
                holeinfo.viewpoints = viewpoints; // 本组应用的视野数量和位置信息
                holeinfo.viewsize = viewsize; // 本组应用的视野尺寸
                mHoleInfo[row][col] = holeinfo;
            }
        }
    }
    update();
}

void WellPattern::onOpenViewAct()
{ // 打开选择视野窗口的事件,需要传递当前孔的coordinate+group+color+grouppoints+allgroups
    if (mMousePos != QPoint(-1,-1)){
        QVariantMap m;
        Q_ASSERT(mHoleInfo[mMousePos.x()][mMousePos.y()].coordinate == mMousePos);
        m[CoordinateField] = mMousePos; // 告知视野当前孔坐标
        m[GroupNameField] = mHoleInfo[mMousePos.x()][mMousePos.y()].group; // 所在组
        m[GroupColorField] = mHoleInfo[mMousePos.x()][mMousePos.y()].color; // 组的颜色
        m[AllGroupsField] = mHoleInfo[mMousePos.x()][mMousePos.y()].allgroup; // 已有的所有组(每次设置分组信息时会更新)
        QVariant v;
        v.setValue(allGroupHolePoints(mHoleInfo[mMousePos.x()][mMousePos.y()].group));
        m[GroupPointsField] = v;
        emit openViewWindow(m);
    }
}

QPointVector WellPattern::allGroupHolePoints(const QString &groupName) const
{ // 获取组内的所有孔坐标传递给视野窗口在应用到本组时可以更新其它孔的视野数据区信息
    if (groupName.isEmpty()) return QPointVector();

    QPointVector vec;
    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {
            auto holeinfo = mHoleInfo[r][c];
            if (holeinfo.group == groupName) {
                vec.append(holeinfo.coordinate);
            }
        }
    }

    return vec;
}

void WellPattern::onSetGroupAct()
{ // 当前孔的所属组颜色和名称传递给分组窗口去更新ui信息
    auto color = mHoleInfo[mMousePos.x()][mMousePos.y()].color;
    auto name = mHoleInfo[mMousePos.x()][mMousePos.y()].group;
    QVariantMap m;
    m[GroupColorField] = color;
    m[GroupNameField] = name;
    emit openSetGroupWindow(m);
}

WellPattern::WellPattern(int rows, int cols, QWidget *parent) : Pattern(rows,cols,parent)
{
    mMouseEvent = true;
    setgroupact = new QAction(tr("分组"));
    openviewact = new QAction(tr("选点"));
    addAction(setgroupact);
    addAction(openviewact);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    initDrapPoints();
    initHoleInfo();

    connect(setgroupact,&QAction::triggered,this,&WellPattern::onSetGroupAct);
    connect(openviewact,&QAction::triggered,this,&WellPattern::onOpenViewAct);
}

void WellPattern::setPatternSize(int rows, int cols)
{
    Pattern::setPatternSize(rows,cols);
    initDrapPoints();
    initHoleInfo();
}

void WellPattern::initDrapPoints()
{ // 清除拖拽区域
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

int WellPattern::drapPointCount() const
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

void WellPattern::initHoleInfo()
{ // 初始化孔信息
    mHoleInfo.clear();
    for(int row = 0 ; row < mrows; ++ row) {
        QHoleInfoVector var;
        for (int col = 0; col < mcols; ++col){
            HoleInfo info;
            info.coordinate = QPoint(row,col);
            info.color = Qt::red;
            info.isselected = false;
            info.viewpoints = QPointVector();
            info.group = QString();
            info.allgroup = QStringList();
            info.viewsize = QSize(0,0);
            var.append(info);
        }
        mHoleInfo.append(var);
    }
    update();
}

void WellPattern::clearHolePattern()
{
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col){
            HoleInfo info = mHoleInfo[row][col];
            info.coordinate = QPoint(row,col);
            info.color = Qt::red;
            info.isselected = false;
            info.viewpoints = QPointVector();
            //info.group = QString(); // 无需改动组名,已有的组名和视野窗口尺寸
            //info.allgroup = QStringList(); // 如果不去除分组信息的话,可能会出现组在但是无高亮,从而没设置这些组的信息导致实验信息保存有问题
            //info.viewsize = QSize(0,0);
            mHoleInfo[row][col] = info;
        }
    }
    update();
}