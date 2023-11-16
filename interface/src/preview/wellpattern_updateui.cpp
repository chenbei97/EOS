/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-01 15:46:10
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-01 15:46:36
 * @FilePath: \EOS\interface\src\preview\wellpattern_updateui.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "wellpattern.h"

// (1) 打开分组的对话框(以当前鼠标位置的孔信息去更新),更新分组对话框的UI信息
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
void WellPattern::updateHoleInfoByGroupInfo(QCVariantMap m)
{ // 关闭分组对话框后(只需要更新分组窗口提供的信息,拖拽区域信息,重新更新allgroup,allcoordinate字段,不需要更新其他信息)

    // 1. 更新孔的信息
    auto gtype = m[HoleExperTypeField].toString();
    auto gcolor = m[HoleGroupColorField].toString();
    auto gname = m[HoleGroupNameField].toString();
    auto medicine = m[HoleMedicineField].toString();
    auto dose = m[HoleDoseField].toString();
    auto unit = m[HoleDoseUnitField].toString();
    //LOG<<"well accept info from groupwin is "<<gtype<<gname<<gcolor<<medicine<<dose<<unit;

    static QBool2DVector lastDrapPoints;

    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col){
            auto pt = mDrapPoints[row][col]; // 右击分组时当前拖拽区域内的点是一组
            if (pt){ // 是拖拽区域的
                mHoleInfo[row][col].isselected = true;//框选内对应的点都设为选中
                mHoleInfo[row][col].group = gname; // 名称是分组窗口设置的分组
                mHoleInfo[row][col].color = gcolor; // 颜色跟随分组窗口设置的颜色
                mHoleInfo[row][col].coordinate = QPoint(row,col); // 孔坐标

//                mHoleInfo[row][col].viewpoints = QPointVector(); //每次分组就把视野坐标信息要清除掉
//                mHoleInfo[row][col].viewsize = QSize(-1,-1); // 可能仅仅是改个颜色或者某个信息不希望把之前的信息清除,因为viewsize又没变
//                // 否则重新分组后视野坐标点的绘制还在继续
//                emit clearViewWindowCache(QPoint(row,col));//重新分组后清除缓存信息

                mHoleInfo[row][col].type = gtype; // 本孔分配的实验类型
                mHoleInfo[row][col].medicine = medicine; // 本孔分配的样品
                mHoleInfo[row][col].dose = dose; // 本孔分配的剂量
                mHoleInfo[row][col].doseunit = unit; // 本孔分配的剂量单位
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
//        mHoleInfo[mMousePos.x()][mMousePos.y()].viewpoints = QPointVector();
//        mHoleInfo[mMousePos.x()][mMousePos.y()].viewsize = QSize(-1,-1);
//        emit clearViewWindowCache(mMousePos);
    }

    // 3. 更新已有的组信息(要在上边的group更新过以后再重新计算)
    auto gnames = getAllWellGroupNames();//有可能分了a,b组,b改名a,只有a了,所以要总是重新根据group算
    //LOG<<"current well all groups = "<<gnames;
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            mHoleInfo[row][col].allgroup = gnames; // 不分拖拽区域,所有孔都更新相同的组别信息,用等于可以防止以前组信息还保留
        }
    }
    mHoleInfo[mMousePos.x()][mMousePos.y()].allgroup = gnames;

    // 4. 更新已有的所有选中的孔信息
    auto holes = getAllWellHoleCoordinates();// 所有按组得到的二维孔向量
    //LOG<<"current well all coordinates = "<<holes;
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            mHoleInfo[row][col].allcoordinate = holes; // 不分拖拽区域,所有孔都更新相同的已有孔信息
        }
    }
    mHoleInfo[mMousePos.x()][mMousePos.y()].allcoordinate = holes;

    update();
}

// (3) 打开视野对话框,更新视野窗口的信息
void WellPattern::onOpenViewAct()
{ // 打开选择视野窗口的事件,需要传递当前孔的coordinate+group+color+grouppoints+allgroups+allcoordinates
    if (mMousePos != QPoint(-1,-1)){
        QVariantMap m;
        Q_ASSERT(mHoleInfo[mMousePos.x()][mMousePos.y()].coordinate == mMousePos);
        m[HoleCoordinateField] = mMousePos; // 告知视野当前孔坐标
        m[HoleGroupNameField] = mHoleInfo[mMousePos.x()][mMousePos.y()].group; // 所在组
        m[HoleGroupColorField] = mHoleInfo[mMousePos.x()][mMousePos.y()].color; // 组的颜色
        auto allgroups = mHoleInfo[mMousePos.x()][mMousePos.y()].allgroup;
        auto allcoordinates = mHoleInfo[mMousePos.x()][mMousePos.y()].allcoordinate;
        auto groupcoordinates = getHoleGroupCoordinates(mHoleInfo[mMousePos.x()][mMousePos.y()].group);
        m[WellAllGroupsField].setValue(allgroups); // 已有的所有组(每次设置分组信息时会更新)
        m[HoleGroupCoordinatesField].setValue(groupcoordinates);
        m[WellAllHolesField].setValue(allcoordinates);
        //LOG<<"well send info to view is "<<m[HoleGroupNameField].toString()<<m[HoleGroupColorField].toString();//ViewPattern::setStrategy接收
        emit openViewWindow(m);
    }
}

// (4) 被视野对话框的应用孔信息反过来更新孔信息
void WellPattern::updateHoleInfoByViewInfoApplyHole(QCVariantMap m)
{ // 删点或者保存点就应用到本孔
    // 1. 拿到应用到组视野窗口传递来的6个信息(ViewPattern::onApplyGroupAct) 关键信息4个
    auto groupName = m[HoleGroupNameField].toString();
    auto groupColor = m[HoleGroupColorField].toString();
    auto coordinate = m[HoleCoordinateField].toPoint();
    auto viewsize = m[HoleViewSizeField].toSize();
    auto viewpoints = m[HoleViewPointsField].value<QPointVector>();
    auto allgroup = m[WellAllGroupsField].value<QSet<QString>>();
    auto allcoordinates = m[WellAllHolesField].value<QPoint2DVector>();

    //LOG<<coordinate<<viewpoints;
    // 2. 把信息更新到本孔
    auto holeinfo = mHoleInfo[coordinate.x()][coordinate.y()];
    Q_ASSERT(holeinfo.coordinate == coordinate); // 孔坐标和当前传递的孔坐标相同
    Q_ASSERT(holeinfo.color == groupColor); // 孔颜色相同
    //Q_ASSERT(holeinfo.isselected == true); // 不肯定是被选中的孔,切换objective时会更新mrows,此时isselected可能false
    Q_ASSERT(holeinfo.allgroup == allgroup);// 这个是一定相等
    Q_ASSERT(holeinfo.allcoordinate == allcoordinates); // 同理
    holeinfo.isselected = true; // 要设置孔为选中,不然就不能绘制高亮了
    holeinfo.viewpoints = viewpoints; // 本组应用的视野数量和位置信息
    holeinfo.viewsize = viewsize; // 本组应用的视野尺寸
    mHoleInfo[coordinate.x()][coordinate.y()] = holeinfo;
    update();
}

// (5) 被视野对话框的应用组信息反过来更新孔信息
void WellPattern::updateHoleInfoByViewInfoApplyGroup(QCVariantMap m)
{// 应用到组-视野窗口的信息拿去更新数据(PreviewPhotoCanvas::onApplyGroupAct())
    // 数据包括视野窗口的组名+组颜色+视野尺寸+当前孔坐标+所有组名+所有视野坐标信息

    // 1. 拿到应用到组视野窗口传递来的6个信息(ViewPattern::onApplyGroupAct) 关键信息4个
    auto groupName = m[HoleGroupNameField].toString();
    auto groupColor = m[HoleGroupColorField].toString();
    auto coordinate = m[HoleCoordinateField].toPoint();
    auto viewsize = m[HoleViewSizeField].toSize();
    auto viewpoints = m[HoleViewPointsField].value<QPointVector>();
    auto allgroup = m[WellAllGroupsField].value<QSet<QString>>();
    auto allcoordinates = m[WellAllHolesField].value<QPoint2DVector>();

//    LOG<<"well accept info from view is "<<groupColor<<groupName<<coordinate<<
//    "【"<<viewpoints<<"】"<<viewsize<<allgroup<<allcoordinates;

    // 2. 根据视野窗口传来的组名 把coordinate对应的组(color+viewpoints,viewsize)都更新 (不需要更新group,allgroup,dose,medicine,unit,type)
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            auto holeinfo = mHoleInfo[row][col];
            if (holeinfo.group == groupName) { // 本组的
                //Q_ASSERT(holeinfo.coordinate == coordinate); // 组其它孔坐标和当前传递的孔坐标不同
                // Q_ASSERT(holeinfo.color == groupColor); // 本组不同孔颜色可能不同
                // Q_ASSERT(holeinfo.isselected == true); // 不肯定是被选中的孔,切换objective时会更新mrows,此时isselected可能false
                Q_ASSERT(holeinfo.allgroup == allgroup);// 这个是一定相等
                Q_ASSERT(holeinfo.allcoordinate == allcoordinates); // 同理
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

// (6) 被视野对话框的应用所有组信息反过来更新孔信息
void WellPattern::updateHoleInfoByViewInfoApplyAll(QCVariantMap m)
{ // 组名+组颜色+视野尺寸+视野坐标 4个关键信息足够 + 孔板所有孔坐标
    auto groupName = m[HoleGroupNameField].toString();
    auto groupColor = m[HoleGroupColorField].toString();
    auto viewsize = m[HoleViewSizeField].toSize();
    auto viewpoints = m[HoleViewPointsField].value<QPointVector>();

    auto allcoordinates = m[WellAllHolesField].value<QPoint2DVector>();
    auto allholes = getAllWellHoleCoordinates();
    //LOG<<allcoordinates;
    //LOG<<allholes;
    Q_ASSERT(allcoordinates == allholes);

    //LOG<<"all holes = " << allholes;
    foreach(auto holes, allcoordinates) {
        foreach(auto hole, holes) {
            auto holeinfo = mHoleInfo[hole.x()][hole.y()];
            holeinfo.isselected = true; // 要设置孔为选中,不然就不能绘制高亮了
            if (holeinfo.group == groupName) // 本组应用的组颜色,不同组的颜色不需要统一
                holeinfo.color = groupColor;
            holeinfo.viewpoints = viewpoints; // 应用的视野数量和位置信息
            holeinfo.viewsize = viewsize; // 本组应用的视野尺寸
            mHoleInfo[hole.x()][hole.y()] = holeinfo;
        }
    }
    update();
}

// (7) 删孔逻辑
void WellPattern::onRemoveHoleAct()
{ // 删孔就是清除当前鼠标的孔信息为默认即可
  // 双击该孔出现视野对话框或者打开分组对话框都是以这个孔信息为准的
  // 视野对话框因为有临时信息,需要清除,因为视野窗口尺寸不变临时信息没有被清除

  // 不清除allgroup,coordinate,allcoordinate字段也可以,每次打开视野窗口都会更新
    mHoleInfo[mMousePos.x()][mMousePos.y()].group = QString();
    mHoleInfo[mMousePos.x()][mMousePos.y()].color = Qt::red;
    mHoleInfo[mMousePos.x()][mMousePos.y()].coordinate = mMousePos;
    mHoleInfo[mMousePos.x()][mMousePos.y()].viewsize = QSize(0,0);
    mHoleInfo[mMousePos.x()][mMousePos.y()].viewpoints = QPointVector();
    mHoleInfo[mMousePos.x()][mMousePos.y()].isselected = false;
    mHoleInfo[mMousePos.x()][mMousePos.y()].allcoordinate = QPoint2DVector();
    mHoleInfo[mMousePos.x()][mMousePos.y()].allgroup = QSet<QString>();
    mHoleInfo[mMousePos.x()][mMousePos.y()].type = QString();
    mHoleInfo[mMousePos.x()][mMousePos.y()].medicine = QString();
    mHoleInfo[mMousePos.x()][mMousePos.y()].doseunit = QString();
    mHoleInfo[mMousePos.x()][mMousePos.y()].dose = QString();

    // 清除视野窗口的缓存信息
    emit clearViewWindowCache(mMousePos);
    update();
}