/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-01 15:46:10
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-01 15:46:36
 * @FilePath: \EOS\interface\src\preview\wellpattern_updateui.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "wellpattern.h"

// (1) 打开分组的对话框,更新分组对话框的UI信息
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
    m[HoleGroupColorField] = color;
    m[HoleGroupNameField] = name;
    m[HoleExperTypeField] = type;
    m[HoleDoseUnitField] = unit;
    m[HoleDoseField] = dose;
    m[HoleMedicineField] = medicine;

    emit openSetGroupWindow(m);
}

// (2) 被分组对话框的信息反过来更新孔信息
void WellPattern::updateGroupByGroupInfo(QCVariantMap m)
{ // 右击分组将分组窗口的组信息(color+name)+拖拽区域信息去更新孔信息(不需要更新points,viewsize)

    // 1. 更新孔的信息
    auto gtype = m[HoleExperTypeField].toString();
    auto gcolor = m[HoleGroupColorField].toString();
    auto gname = m[HoleGroupNameField].toString();
    auto medicine = m[HoleMedicineField].toString();
    auto dose = m[HoleDoseField].toString();
    auto unit = m[HoleDoseUnitField].toString();
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
    auto gnames = getAllWellGroupNames();//有可能分了a,b组,b改名a,只有a了,所以要总是重新根据group算
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

// (3) 打开视野对话框,更新视野窗口的信息
void WellPattern::onOpenViewAct()
{ // 打开选择视野窗口的事件,需要传递当前孔的coordinate+group+color+grouppoints+allgroups
    if (mMousePos != QPoint(-1,-1)){
        QVariantMap m;
        Q_ASSERT(mHoleInfo[mMousePos.x()][mMousePos.y()].coordinate == mMousePos);
        m[HoleCoordinateField] = mMousePos; // 告知视野当前孔坐标
        m[HoleGroupNameField] = mHoleInfo[mMousePos.x()][mMousePos.y()].group; // 所在组
        m[HoleGroupColorField] = mHoleInfo[mMousePos.x()][mMousePos.y()].color; // 组的颜色
        auto allgroups = mHoleInfo[mMousePos.x()][mMousePos.y()].allgroup;
        auto groupcoordinates = getHoleGroupCoordinates(mHoleInfo[mMousePos.x()][mMousePos.y()].group);
        QVariant v1,v2;
        v1.setValue(allgroups);
        m[WellAllGroupsField] = v1; // 已有的所有组(每次设置分组信息时会更新)
        v2.setValue(groupcoordinates);
        m[HoleGroupCoordinatesField] = v2;
//        LOG<<"well send info to view is "<<m[GroupNameField].toString()<<m[GroupColorField].toString()
//        <<mMousePos<<"【"<<other_coord<<"】"<<allgroups;//ViewPattern::setStrategy接收
        emit openViewWindow(m);
    }
}

// (4) 被视野对话框的信息反过来更新孔信息
void WellPattern::updateGroupByViewInfo(QCVariantMap m)
{// 应用到组-视野窗口的信息拿去更新数据(PreviewPhotoCanvas::onApplyGroupAct())
    // 数据包括视野窗口的组名+组颜色+视野尺寸+当前孔坐标+所有组名+所有视野坐标信息

    // 1. 拿到应用到组视野窗口传递来的6个信息(ViewPattern::onApplyGroupAct) 关键信息4个
    auto groupName = m[HoleGroupNameField].toString();
    auto groupColor = m[HoleGroupColorField].toString();
    auto coordinate = m[HoleCoordinateField].toPoint();
    auto viewsize = m[HoleViewSizeField].toSize();
    auto viewpoints = m[HoleViewPointsField].value<QPointVector>();
    auto allgroup = m[WellAllGroupsField].value<QSet<QString>>();

    LOG<<"well accept info from view is "<<groupColor<<groupName<<coordinate<<
    "【"<<viewpoints<<"】"<<viewsize<<allgroup;

    // 2. 根据视野窗口传来的组名 把coordinate对应的组(color+viewpoints,viewsize)都更新 (不需要更新group,allgroup,dose,medicine,unit,type)
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            auto holeinfo = mHoleInfo[row][col];
            if (holeinfo.group == groupName) { // 本组的
                //Q_ASSERT(holeinfo.coordinate == coordinate); // 组其它孔坐标和当前传递的孔坐标不同
                // Q_ASSERT(holeinfo.color == groupColor); // 本组不同孔颜色可能不同
                // Q_ASSERT(holeinfo.isselected == true); // 不肯定是被选中的孔,切换objective时会更新mrows,此时isselected可能false
                Q_ASSERT(holeinfo.allgroup == allgroup);// 这个是一定相等
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
