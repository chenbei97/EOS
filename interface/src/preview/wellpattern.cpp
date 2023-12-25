/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-27 08:53:17
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-08 17:25:35
 * @FilePath: \EOS\interface\src\preview\wellpattern.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "wellpattern.h"

WellPatternInfo WellPattern::patternInfo() const
{
    WellPatternInfo allinfo;

    auto allgroups = getAllGroups();

    WellPatternInfo info;
    for(auto groupName: allgroups) {
        WellGroupInfo groupInfoMap;
        auto groupHoles = getGroupHoles(groupName);

        for(auto hole: groupHoles)
        {
            WellHoleInfo holeInfoMap; // 这个孔存储的所有信息都在这里保存

            auto holeInfo = mHoleInfo[hole.x()][hole.y()];

            Q_ASSERT(holeInfo.group == groupName);
            Q_ASSERT(holeInfo.coordinate == hole);
            Q_ASSERT(holeInfo.isselected == true); // isselected不装,已有的孔坐标就是true
            Q_ASSERT(holeInfo.allgroup == allgroups);

            holeInfoMap[HoleCoordinateField] = holeInfo.coordinate; // 1. 孔信息
            holeInfoMap[HoleGroupColorField] = holeInfo.color;
            holeInfoMap[HoleGroupNameField] = holeInfo.group;
            holeInfoMap[HoleGroupCoordinatesField].setValue(groupHoles);
            holeInfoMap[HoleAllGroupsField].setValue(holeInfo.allgroup);
            holeInfoMap[HoleAllCoordinatesField].setValue(holeInfo.allcoordinate);

            // 2. 视野信息
#ifdef viewRowColUnEqual
            holeInfoMap[HoleViewSizeField].setValue(holeInfo.dimension);
#else
            holeInfoMap[HoleViewSizeField] = holeInfo.viewsize;
#endif
            holeInfoMap[HoleViewRectsField].setValue(holeInfo.viewrects);
            holeInfoMap[HoleViewUiPointsField].setValue(holeInfo.uipoints);
            holeInfoMap[HoleViewPointsField].setValue(holeInfo.viewpoints);

            holeInfoMap[HoleExperTypeField] = holeInfo.type; // 3.备忘录信息
            holeInfoMap[HoleMedicineField] = holeInfo.medicine;
            holeInfoMap[HoleDoseField] = holeInfo.dose;
            holeInfoMap[HoleDoseUnitField] = holeInfo.doseunit;

            auto groupInfoKey = QString(PairArgsField).arg(hole.x()).arg(hole.y());
            groupInfoMap[groupInfoKey] = holeInfoMap;
        }
        //LOG<<groupName<<"'s hole count = "<<groupInfoMap.keys().count();
        info[groupName] = groupInfoMap; // 存储了每个组的信息
    }
    //LOG<<"group's count = "<<info.keys().count();
    allinfo[HoleSizeField] = patternSize();
    allinfo[GroupField] = info;
    return allinfo;
}

WellPattern::WellPattern(int rows, int cols, QWidget *parent) : Pattern(rows,cols,parent)
{
    mMouseEvent = true;
    opengroupact = new QAction(tr(GroupHoleActTitle));
    openviewact = new QAction(tr(SelectHoleActTitle));
    removeholeact = new QAction(tr(RemoveHoleActTitle));
    enterviewact = new QAction(tr(EnterViewActTitle));
    addAction(opengroupact);
    addAction(openviewact);
    addAction(removeholeact);
    addAction(enterviewact);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    initDrapHoles();
    initDisableHoles();
    initHoleInfo();

    connect(opengroupact,&QAction::triggered,this,&WellPattern::onSetGroupAct);
    connect(openviewact,&QAction::triggered,this,&WellPattern::onOpenViewAct);
    connect(removeholeact,&QAction::triggered,this,&WellPattern::onRemoveHoleAct);
}

void WellPattern::setPatternSize(int rows, int cols)
{
    Pattern::setPatternSize(rows,cols);
    initDrapHoles();
    initDisableHoles();
    initHoleInfo();
}

void WellPattern::setViewMode(ViewMode mode)
{
    mSelectMode = mode;
    update();
    //clearViewInfo(); // 不清除任何信息
}

void WellPattern::initDrapHoles()
{ // 清除拖拽区域,pressEvent会调用1次
    mDrapRect = QRectF();
    //mMousePos = {-1,-1};
    mDrapHoles.clear();
    for(int row = 0 ; row < mrows; ++ row) {
        QBoolVector var;
        for (int col = 0; col < mcols; ++col){
            var.append(false);
        }
        mDrapHoles.append(var);
    }
    update();
}

void WellPattern::initDisableHoles()
{
    mDisableHoles.clear();
    for(int row = 0 ; row < mrows; ++ row) {
        QBoolVector var;
        for (int col = 0; col < mcols; ++col){
            var.append(false);
        }
        mDisableHoles.append(var);
    }
    update();
}

void WellPattern::setDisableHole(QCPoint point, bool enable)
{ // 2x3,x只能取0,1,y取0,1,2,不能取等号
    if (point.x()< 0 || point.x() >= mrows // 防止越界,外部也有义务不许越界
        || point.y()<0 || point.y() >= mcols)
        return;
    mDisableHoles[point.x()][point.y()] = enable;
    update();
}

void WellPattern::setDisableHoles(QCPointVector points, bool enable)
{// 对指定的有限坐标进行设置
    for(auto pt : points) {
        setDisableHole(pt,enable);
    }
}

void WellPattern::setDisableHoles(bool enable)
{// 对所有的坐标进行设置
    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {
            mDisableHoles[r][c] = enable;
        }
    }
    update();
}

int WellPattern::drapHoleCount() const
{ // 计算拖拽区域包含的点个数
    int count = 0;
    for(int r = 0; r < mrows; ++ r) {
        for(int c = 0; c < mcols; ++c) {
            if (mDrapHoles[r][c] && !mDisableHoles[r][c])
                count++;
        }
    }
    return count;
}

int WellPattern::numberOfViews() const
{ // 注意: 计算电机坐标而不是绘图坐标
    int count = 0;
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            if (mHoleInfo[row][col].isselected && !mDisableHoles[row][col])// 多层保护没坏处,虽然置灰点mHoleInfo对应的isselected一定是false
            {
                count += mHoleInfo[row][col].viewpoints.count();
            }
        }
    }
    return count;
}

int WellPattern::numberOfViews(const QPoint &holePoint)
{ // 外部调用holePoint有义务不越界
    int count = 0;
    if (mHoleInfo[holePoint.x()][holePoint.y()].isselected &&
        !mDisableHoles[holePoint.x()][holePoint.y()]) // 多层保护没坏处,虽然置灰点mHoleInfo对应的isselected一定是false
        count = mHoleInfo[holePoint.x()][holePoint.y()].viewpoints.count();
    return count;
}

int WellPattern::numberOfViews(const QString &group)
{
    int count = 0;
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            if (mHoleInfo[row][col].isselected
                && mHoleInfo[row][col].group == group
                && !mDisableHoles[row][col]) // 多层保护没坏处,虽然置灰点mHoleInfo对应的isselected一定是false
                count += mHoleInfo[row][col].viewpoints.count();
        }
    }
    return count;
}

QSet<QString> WellPattern::getAllGroups() const {
    // 返回所有分过的组,不重复
    QSet<QString> set;
    QMap<QString,int> map;

    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col){
            auto gname = mHoleInfo[row][col].group;
            if (!gname.isEmpty() && !mDisableHoles[row][col]) {
                map[gname]++;
                set.insert(gname);
            }
        }
    }
    //LOG<<"group's count = "<<map; // 统计每个组孔的数量

    return set;
}

QPointVector WellPattern::getGroupHoles(const QString &groupName) const
{ // 获取组内的所有孔坐标传递给视野窗口在应用到本组时可以更新其它孔的视野数据区信息
    if (groupName.isEmpty()) return QPointVector();

    QPointVector vec;
    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {
            auto holeinfo = mHoleInfo[r][c];
            if (holeinfo.group == groupName && !mDisableHoles[r][c]) {
                vec.append(holeinfo.coordinate);
            }
        }
    }

    return vec;
}

QPoint2DVector WellPattern::getAllHoles() const
{ // 获取所有选过的孔坐标
    QPoint2DVector points;

    auto allgroups = getAllGroups();

    foreach(auto group,allgroups) { // 把所有组的孔向量列表添加
        auto holes = getGroupHoles(group);
        points.append(holes);
    }

    return points;
}

void WellPattern::initHoleInfo()
{ // 初始化孔信息
    mHoleInfo.clear();
    for(int row = 0 ; row < mrows; ++ row) {
        QHoleInfoVector var;
        for (int col = 0; col < mcols; ++col){
            HoleInfo info;
            info.group = QString();
            info.coordinate = QPoint(row,col);
            info.color = Qt::red;
#ifdef viewRowColUnEqual
            info.dimension = Dimension2D();
#else
            info.viewsize = 0;
#endif
            info.viewrects = QRectFVector();
            info.viewpoints = QPointFVector();
            info.uipoints = QPointFVector();

            info.isselected = false;
            info.allgroup = QSet<QString>();
            info.allcoordinate = QPoint2DVector();

            info.type = QString();
            info.medicine = QString();
            info.doseunit = QString();
            info.dose = QString();
            var.append(info);
        }
        mHoleInfo.append(var);
    }
    update();
}

void WellPattern::clearViewInfo()
{ // 切换物镜时调用,需要清理掉视野窗口映射的4个信息
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            mHoleInfo[row][col].viewrects = QRectFVector();
            mHoleInfo[row][col].viewpoints = QPointFVector();
            mHoleInfo[row][col].uipoints = QPointFVector();
#ifdef viewRowColUnEqual
            mHoleInfo[row][col].dimension = Dimension2D();
#else
            mHoleInfo[row][col].viewsize = 0;
#endif
        }
    }
    update();
}

//void WellPattern::importHoleInfoV1(QCPoint point,QCString group,QCPointFVector viewpoints,int viewsize)
//{ // 完全弃用,后续版本会删除此函数
//    // 更新指定孔的信息,用于导入实验配置时逐孔更新,mrows,mcols无需更新的原因是导入实验配置先更新的tool信息触发了togglebrand已经更新过
//    // 1. viewpoints存储的时候是电机坐标,并非UI坐标,需要转换
//    QPointFVector points;
//    QRectFVector rects; // 基于电机坐标的等比例区域
//    for(auto pt: viewpoints)
//    {
//        points.append(QPointF(pt.x(),pt.y())); // 当初电机坐标是按照1.0/viewsize去存的,现在的比例不变
//        auto rect = QRectF(pt.x(),pt.y(),1.0/viewsize,1.0/viewsize);
//        rects.append(rect);
//    }
//
//    // 2. 转换ui坐标需要 UI区域和掩码矩阵
//    QPointFVector uipoints; // 要转换的UI坐标
//    for(int r = 0; r < mDispersedMaskSize; ++r) {
//        for (int c = 0; c < mDispersedMaskSize; ++c) {
//            for(auto viewRect: rects) {
//                auto topleft = viewRect.topLeft();
//                auto size = viewRect.size(); // 每个等比例UI区域要放大到固定的UI掩码矩阵尺寸
//
//                auto x = topleft.x() * mDispersedMaskSize ;
//                auto y = topleft.y() * mDispersedMaskSize ;
//                auto w = size.width() * mDispersedMaskSize;
//                auto h = size.height() * mDispersedMaskSize;
//
//                auto rect = QRectF(x,y,w,h); // 等比例放大尺寸到mUiViewMaskSize
//                if (rect.intersects(QRectF(c,r,1.0,1.0))) { // 掩码区域包含的点都视为UI点
//                    uipoints.append(QPointF((r+0.5)/mDispersedMaskSize,(c+0.5)/mDispersedMaskSize));
//                }
//            }  // end 3层for
//        }
//    }
//
//
//    Q_ASSERT(mHoleInfo[point.x()][point.y()].coordinate == point);
//
//    // 3.颜色不更新,默认就红色,不同组的颜色会一样,因为当初没有存不同的颜色
//    mHoleInfo[point.x()][point.y()].group = group;
//    mHoleInfo[point.x()][point.y()].viewpoints = points;
//    mHoleInfo[point.x()][point.y()].uipoints = uipoints;
//
//    mHoleInfo[point.x()][point.y()].viewsize = viewsize;
//    mHoleInfo[point.x()][point.y()].isselected = true;
//
//    mHoleInfo[point.x()][point.y()].allgroup = getAllGroups();
//    mHoleInfo[point.x()][point.y()].allcoordinate = getAllHoles();
//    // 不能借助onOpenViewAct打开视野窗口更新,因为不传递viewpoints给视野窗口,而且更新仅仅更新1个孔的
//    update();
//}

void WellPattern::importHoleInfo(const QHoleInfoVector& vec,ViewMode mode)
{
    setViewMode(mode);
    for(auto holeInfo: vec) {
        auto x = holeInfo.coordinate.x();
        auto y = holeInfo.coordinate.y();
        mHoleInfo[x][y] = holeInfo; // importHoleInfo之前已经调用setPatternSize不会越界
    }
    auto allgroups = getAllGroups();
    auto allholes = getAllHoles();
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            mHoleInfo[row][col].allcoordinate = allholes;
            mHoleInfo[row][col].allgroup = allgroups;
        }
    }
    update();
    //openviewact->trigger(); // 把组信息等传给viewpattern
}