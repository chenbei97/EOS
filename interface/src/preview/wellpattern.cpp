/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-27 08:53:17
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-16 11:51:17
 * @FilePath: \EOS\interface\src\preview\wellpattern.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "wellpattern.h"

PreviewPatternInfo WellPattern::patternInfo() const
{
    PreviewPatternInfo info;

    auto allgroups = getAllWellGroupNames();

            foreach(auto group,allgroups) {

            QVariantMap groupInfo;
            auto holes = getHoleGroupCoordinates(group);

            foreach(auto hole,holes)
                {
                    QVariantMap holeInfo; // 这个孔存储的所有信息都在这里保存

                    holeInfo[HoleGroupCoordinatesField].setValue(holes); // 0. 每个孔都会存储本组所有的孔坐标信息

                    auto currentholeinfo = mHoleInfo[hole.x()][hole.y()];
                    Q_ASSERT(currentholeinfo.group == group); // group,color,isselected,allgroup,allcoordinate不用装
                    Q_ASSERT(currentholeinfo.coordinate == hole);
                    //currentholeinfo.color 颜色没用
                    Q_ASSERT(currentholeinfo.isselected == true);
                    Q_ASSERT(currentholeinfo.allgroup == allgroups);// 5条信息无需装

                    holeInfo[HoleCoordinateField] = hole; // 1. 坐标信息
                    holeInfo[HoleViewSizeField] = currentholeinfo.viewsize; // 2. 视野信息
                    holeInfo[HoleViewRectsField].setValue(currentholeinfo.viewrects);
                    holeInfo[HoleViewUiPointsField].setValue(currentholeinfo.uipoints);
                    holeInfo[HoleViewPointsField].setValue(currentholeinfo.viewpoints);

                    holeInfo[HoleExperTypeField] = currentholeinfo.type; // 3.备忘录信息
                    holeInfo[HoleMedicineField] = currentholeinfo.medicine;
                    holeInfo[HoleDoseField] = currentholeinfo.dose;
                    holeInfo[HoleDoseUnitField] = currentholeinfo.doseunit;

                    auto holename = QString("(%1,%2)").arg(hole.x()).arg(hole.y());
                    groupInfo[holename] = holeInfo;
                    //LOG<<groupInfo;
                }
            //LOG<<group<<"'s hole count = "<<groupInfo.keys().count();
            info[group] = groupInfo; // 存储了每个组的信息
        }
    //LOG<<"group's count = "<<info.keys().count();
    return info;
}

WellPattern::WellPattern(int rows, int cols, QWidget *parent) : Pattern(rows,cols,parent)
{
    mMouseEvent = true;
    setgroupact = new QAction(tr("Grouping"));
    openviewact = new QAction(tr("Select Holes"));
    removeholeact = new QAction(tr("Remove Hole"));
    addAction(setgroupact);
    addAction(openviewact);
    addAction(removeholeact);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    initDrapPoints();
    initDisablePoints();
    initHoleInfo();

    connect(setgroupact,&QAction::triggered,this,&WellPattern::onSetGroupAct);
    connect(openviewact,&QAction::triggered,this,&WellPattern::onOpenViewAct);
    connect(removeholeact,&QAction::triggered,this,&WellPattern::onRemoveHoleAct);
}

void WellPattern::setPatternSize(int rows, int cols)
{
    Pattern::setPatternSize(rows,cols);
    initDrapPoints();
    initDisablePoints();
    initHoleInfo();
}

void WellPattern::initDrapPoints()
{ // 清除拖拽区域,pressEvent会调用1次
    mDrapRect = QRectF();
    //mMousePos = {-1,-1};
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

void WellPattern::initDisablePoints()
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

void WellPattern::setDisablePoint(QCPoint point, bool enable)
{ // 2x3,x只能取0,1,y取0,1,2,不能取等号
    if (point.x()< 0 || point.x() >= mrows // 防止越界,外部也有义务不许越界
        || point.y()<0 || point.y() >= mcols)
        return;
    mDisablePoints[point.x()][point.y()] = enable;
    update();
}

void WellPattern::setDisablePoints(QCPointVector points, bool enable)
{// 对指定的有限坐标进行设置
    foreach(auto pt , points) {
        setDisablePoint(pt,enable);
    }
}

void WellPattern::setDisablePoints(bool enable)
{// 对所有的坐标进行设置
    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {
            mDisablePoints[r][c] = enable;
        }
    }
    update();
}

int WellPattern::drapPointCount() const
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

int WellPattern::numberOfViews() const
{ // 注意: 计算电机坐标而不是绘图坐标
    int count = 0;
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            if (mHoleInfo[row][col].isselected && !mDisablePoints[row][col])// 多层保护没坏处,虽然置灰点mHoleInfo对应的isselected一定是false
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
        !mDisablePoints[holePoint.x()][holePoint.y()]) // 多层保护没坏处,虽然置灰点mHoleInfo对应的isselected一定是false
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
                && !mDisablePoints[row][col]) // 多层保护没坏处,虽然置灰点mHoleInfo对应的isselected一定是false
                count += mHoleInfo[row][col].viewpoints.count();
        }
    }
    return count;
}

QSet<QString> WellPattern::getAllWellGroupNames() const
{ // 返回所有分过的组,不重复
    QSet<QString> set;
    QMap<QString,int> map;

    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col){
            auto gname = mHoleInfo[row][col].group;
            if (!gname.isEmpty() && !mDisablePoints[row][col]) {
                map[gname]++;
                set.insert(gname);
            }
        }
    }
    //LOG<<"group's count = "<<map; // 统计每个组孔的数量

    return set;
}

QPointVector WellPattern::getHoleGroupCoordinates(const QString &groupName) const
{ // 获取组内的所有孔坐标传递给视野窗口在应用到本组时可以更新其它孔的视野数据区信息
    if (groupName.isEmpty()) return QPointVector();

    QPointVector vec;
    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {
            auto holeinfo = mHoleInfo[r][c];
            if (holeinfo.group == groupName && !mDisablePoints[r][c]) {
                vec.append(holeinfo.coordinate);
            }
        }
    }

    return vec;
}

QPoint2DVector WellPattern::getAllWellHoleCoordinates() const
{
    QPoint2DVector points;

    auto allgroups = getAllWellGroupNames();

    foreach(auto group,allgroups) { // 把所有组的孔向量列表添加
        auto holes = getHoleGroupCoordinates(group);
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

            info.viewsize = 0;
            info.viewrects = ViewRectFVector();
            info.viewpoints = ViewPointVector();
            info.uipoints = ViewPointVector();

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

void WellPattern::clearAllHoleViewPoints()
{ // 切换物镜时调用,需要清理掉视野窗口映射的2个信息
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            mHoleInfo[row][col].viewrects = ViewRectFVector();
            mHoleInfo[row][col].viewpoints = ViewPointVector();
            mHoleInfo[row][col].uipoints = ViewPointVector();
            mHoleInfo[row][col].viewsize = 0;
        }
    }
    update();
}

void WellPattern::updateHoleInfo(QCPoint point,QCString group,QCPointFVector viewpoints,int viewsize)
{ // 更新指定孔的信息,用于导入实验配置时逐孔更新
    ViewPointVector points;
    for(auto pt: viewpoints)
        points.append(ViewPoint(pt.x(),pt.y()));

    Q_ASSERT(mHoleInfo[point.x()][point.y()].coordinate == point);

    mHoleInfo[point.x()][point.y()].group = group;
    mHoleInfo[point.x()][point.y()].viewpoints = points;

    mHoleInfo[point.x()][point.y()].viewsize = viewsize;
    mHoleInfo[point.x()][point.y()].isselected = true;

    mHoleInfo[point.x()][point.y()].allgroup = getAllWellGroupNames();
    mHoleInfo[point.x()][point.y()].allcoordinate = getAllWellHoleCoordinates();
    // 不能借助onOpenViewAct打开视野窗口更新,因为不传递viewpoints给视野窗口,而且更新仅仅更新1个孔的
    update();
}
