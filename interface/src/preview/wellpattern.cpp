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
    auto gtype = m[GroupTypeField].toString();
    auto gcolor = m[GroupColorField].toString();
    auto gname = m[GroupNameField].toString();
    auto medicine = m[GroupMedicineField].toString();
    auto dose = m[GroupDoseField].toString();
    auto unit = m[GroupDoseUnitField].toString();
    //LOG<<"well accept info from groupwin is "<<gtype<<gname<<gcolor<<medicine<<dose<<unit;

    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col){
            auto pt = mDrapPoints[row][col]; // 右击分组时当前拖拽区域内的点是一组
            if (pt){ // 是拖拽区域的
                mHoleInfo[row][col].isselected = true;//框选内对应的点都设为选中
                mHoleInfo[row][col].color = gcolor; // 颜色跟随分组窗口设置的颜色
                mHoleInfo[row][col].group = gname; // 名称是分组窗口设置的分组
                mHoleInfo[row][col].type = gtype; // 本孔分配的实验类型
                mHoleInfo[row][col].medicine = medicine; // 本孔分配的样品
                mHoleInfo[row][col].dose = dose; // 本孔分配的剂量
                mHoleInfo[row][col].doseunit = unit; // 本孔分配的剂量单位
                mHoleInfo[row][col].coordinate = QPoint(row,col); // 孔坐标
                mDrapPoints[row][col] = false; // 拖拽区域内的点也要更新为false,不然还会绘制这个区域
            }
        }
    }

    // 2. 框选的时候会遗漏鼠标当前选中的点
    if (mMousePos != QPoint(-1,-1)) {// 没启用鼠标事件或者点击外围,这是{-1,-1}会越界
        mHoleInfo[mMousePos.x()][mMousePos.y()].isselected = true; // 鼠标点击的这个
        mHoleInfo[mMousePos.x()][mMousePos.y()].color = gcolor;
        mHoleInfo[mMousePos.x()][mMousePos.y()].group = gname;
        mHoleInfo[mMousePos.x()][mMousePos.y()].coordinate = mMousePos;
        mHoleInfo[mMousePos.x()][mMousePos.y()].type = gtype; // 本孔分配的实验类型
        mHoleInfo[mMousePos.x()][mMousePos.y()].medicine = medicine; // 本孔分配的样品
        mHoleInfo[mMousePos.x()][mMousePos.y()].dose = dose; // 本孔分配的剂量
        mHoleInfo[mMousePos.x()][mMousePos.y()].doseunit = unit; // 本孔分配的剂量单位
        mDrapPoints[mMousePos.x()][mMousePos.y()] = false;
    }

    // 3. 更新已有的组信息(要在上边的group更新过以后再重新计算)
    auto gnames = allHoleGroupNames();//有可能分了a,b组,b改名a,只有a了,所以要总是重新根据group算
    //if (!gname.isEmpty()) gnames.insert(gname); // 新的组,无论是否添加过这个组
    LOG<<"current groups = "<<gnames;
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            mHoleInfo[row][col].allgroup = gnames; // 不分拖拽区域,所有孔都更新相同的组别信息,用等于可以防止以前组信息还保留
        }
    }
    mHoleInfo[mMousePos.x()][mMousePos.y()].allgroup = gnames;
    update();
}

QSet<QString> WellPattern::allHoleGroupNames() const
{ // 返回所有分过的组,不重复
    QSet<QString> set;
    QMap<QString,int> map;

    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col){
            auto gname = mHoleInfo[row][col].group;
            if (!gname.isEmpty()) {
                map[gname]++;
                set.insert(gname);
            }
        }
    }
    LOG<<"group's count = "<<map; // 统计每个组孔的数量

    return set;
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
    auto allgroup = m[AllGroupsField].value<QSet<QString>>();
    //LOG<<"well accept info from view is "<<groupColor<<groupName<<coordinate<<"【"<<viewpoints<<"】"<<viewsize<<allgroup;

    // 2. 根据视野窗口传来的组名 把coordinate对应的组(color+viewpoints,viewsize)都更新 (不需要更新group,allgroup,dose,medicine,unit,type)
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
        auto allgroups = mHoleInfo[mMousePos.x()][mMousePos.y()].allgroup;
        auto other_coord = allGroupHolePoints(mHoleInfo[mMousePos.x()][mMousePos.y()].group);
        QVariant v1,v2;
        v1.setValue(allgroups);
        m[AllGroupsField] = v1; // 已有的所有组(每次设置分组信息时会更新)
        v2.setValue(other_coord);
        m[GroupPointsField] = v2;
//        LOG<<"well send info to view is "<<m[GroupNameField].toString()<<m[GroupColorField].toString()
//        <<mMousePos<<"【"<<other_coord<<"】"<<allgroups;//ViewPattern::setStrategy接收
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
{ // 当前孔的所属组颜色和名称传递给分组窗口去更新ui信息(传递6个信息)
    // updateGroupByGroupInfo函数内用到这6个信息
    auto color = mHoleInfo[mMousePos.x()][mMousePos.y()].color;
    auto name = mHoleInfo[mMousePos.x()][mMousePos.y()].group;
    auto type = mHoleInfo[mMousePos.x()][mMousePos.y()].type;
    auto dose = mHoleInfo[mMousePos.x()][mMousePos.y()].dose;
    auto unit = mHoleInfo[mMousePos.x()][mMousePos.y()].doseunit;
    auto medicine = mHoleInfo[mMousePos.x()][mMousePos.y()].medicine;

    QVariantMap m;
    m[GroupColorField] = color;
    m[GroupNameField] = name;
    m[GroupTypeField] = type;
    m[GroupDoseUnitField] = unit;
    m[GroupDoseField] = dose;
    m[GroupMedicineField] = medicine;

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
            info.type = QString();
            info.medicine = QString();
            info.doseunit = QString();
            info.dose = QString();
            info.coordinate = QPoint(row,col);
            info.color = Qt::red;
            info.isselected = false;
            info.viewpoints = QPointVector();
            info.group = QString();
            info.allgroup = QSet<QString>();
            info.viewsize = QSize(0,0);
            var.append(info);
        }
        mHoleInfo.append(var);
    }
    update();
}