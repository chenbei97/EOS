/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-11-01 15:46:10
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-11-17 16:29:25
 * @FilePath: \EOS\interface\src\preview\wellpattern_updateui.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "wellpattern.h"

// (1) 打开分组的对话框,以原有的信息去更新分组对话框的UI信息
void WellPattern::onSetGroupAct()
{
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
{
    // 1. 更新分组对话框提供的信息
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
            if (pt && !mDisablePoints[row][col]){ // 是拖拽区域的(拖拽区域已经不会包含置灰的不可选的孔,mouseMoveEvent已经对其限制),不过多一层保护没坏处
                mHoleInfo[row][col].isselected = true;//框选内对应的点都设为选中
                mHoleInfo[row][col].group = gname; // 名称是分组窗口设置的分组
                mHoleInfo[row][col].color = gcolor; // 颜色跟随分组窗口设置的颜色
                mHoleInfo[row][col].coordinate = QPoint(row,col); // 孔坐标
                mHoleInfo[row][col].type = gtype; // 本孔分配的实验类型
                mHoleInfo[row][col].medicine = medicine; // 本孔分配的样品
                mHoleInfo[row][col].dose = dose; // 本孔分配的剂量
                mHoleInfo[row][col].doseunit = unit; // 本孔分配的剂量单位
                mDrapPoints[row][col] = false; // 拖拽区域内的点也要更新为false,不然还会绘制这个区域

                // 视野的2个信息无需更新,只更新分组对话框提供的信息
                //mHoleInfo[row][col].viewpoints = QPointVector(); //每次分组就把视野坐标信息要清除掉
                //mHoleInfo[row][col].viewsize = QSize(-1,-1); // 可能仅仅是改个颜色或者某个信息不希望把之前的信息清除,因为viewsize又没变
                //emit clearViewWindowCache(QPoint(row,col));//重新分组后清除缓存信息
            }
        }
    }

    // 2. 框选的时候会遗漏鼠标当前选中的点
    if (mMousePos != QPoint(-1,-1) && !mDisablePoints[mMousePos.x()][mMousePos.y()]) {// 没启用鼠标事件或者点击外围,这是{-1,-1}会越界
        // 注意2个判断先后顺序,如果是-1,-1就返回了,编译器不会执行后边的判断也就不会越界
        mHoleInfo[mMousePos.x()][mMousePos.y()].isselected = true; // 鼠标点击的这个
        mHoleInfo[mMousePos.x()][mMousePos.y()].color = gcolor;
        mHoleInfo[mMousePos.x()][mMousePos.y()].group = gname;
        mHoleInfo[mMousePos.x()][mMousePos.y()].coordinate = mMousePos;
        mHoleInfo[mMousePos.x()][mMousePos.y()].type = gtype; // 本孔分配的实验类型
        mHoleInfo[mMousePos.x()][mMousePos.y()].medicine = medicine; // 本孔分配的样品
        mHoleInfo[mMousePos.x()][mMousePos.y()].dose = dose; // 本孔分配的剂量
        mHoleInfo[mMousePos.x()][mMousePos.y()].doseunit = unit; // 本孔分配的剂量单位
        mDrapPoints[mMousePos.x()][mMousePos.y()] = false;
        //mHoleInfo[mMousePos.x()][mMousePos.y()].viewpoints = QPointVector();
        //mHoleInfo[mMousePos.x()][mMousePos.y()].viewsize = QSize(-1,-1);
        //emit clearViewWindowCache(mMousePos);
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
{ // wellpattern组装6个信息,viewpattern自行组装2个信息,共8个
    if (mMousePos != QPoint(-1,-1)){
        QVariantMap m;
        Q_ASSERT(mHoleInfo[mMousePos.x()][mMousePos.y()].coordinate == mMousePos);
        m[HoleCoordinateField] = mMousePos; // 告知视野当前孔坐标
        m[HoleGroupNameField] = mHoleInfo[mMousePos.x()][mMousePos.y()].group; // 所在组
        m[HoleGroupColorField] = mHoleInfo[mMousePos.x()][mMousePos.y()].color; // 组的颜色
        // 所有组只在打开分组对话框更新,但可能没打开分组对话框此时allgroup就没有被赋值会出现错误 在204行的设定可能会错误
        mHoleInfo[mMousePos.x()][mMousePos.y()].allgroup = getAllWellGroupNames();
        m[WellAllGroupsField].setValue(mHoleInfo[mMousePos.x()][mMousePos.y()].allgroup); // 已有的所有组(每次设置分组信息时会更新)
        m[HoleGroupCoordinatesField].setValue(getHoleGroupCoordinates(mHoleInfo[mMousePos.x()][mMousePos.y()].group));
        mHoleInfo[mMousePos.x()][mMousePos.y()].allcoordinate = getAllWellHoleCoordinates();
        m[WellAllHolesField].setValue(mHoleInfo[mMousePos.x()][mMousePos.y()].allcoordinate);
        //LOG<<"well send info to view is "<<m[HoleGroupNameField].toString()<<m[HoleGroupColorField].toString();//ViewPattern::setStrategy接收
        emit openViewWindow(m);
    }
}

// (4) 保存或删除视野的应用到孔事件
void WellPattern::updateHoleInfoByViewInfoApplyHole(QCVariantMap m)
{
    // 1. 关键信息: 视野区域-绘图 视野坐标-电机坐标
    auto groupName = m[HoleGroupNameField].toString();
    auto groupColor = m[HoleGroupColorField].toString();
    auto coordinate = m[HoleCoordinateField].toPoint();
    auto allgroup = m[WellAllGroupsField].value<QSet<QString>>();
    auto allcoordinates = m[WellAllHolesField].value<QPoint2DVector>();
    // HoleGroupCoordinatesField 该组的所有孔坐标不解析
    auto viewsize = m[HoleViewSizeField].toInt();
    auto viewrects = m[HoleViewRectsField].value<QRectFVector>(); // 关键信息
    auto viewpoints = m[HoleViewPointsField].value<ViewPointVector>();// 关键信息
    auto uipoints = m[HoleViewUiPointsField].value<ViewPointVector>(); // 关键信息

    // 2. 把信息更新到本孔
    auto holeinfo = mHoleInfo[coordinate.x()][coordinate.y()];
    Q_ASSERT(holeinfo.coordinate == coordinate); // 孔坐标和当前传递的孔坐标相同
    Q_ASSERT(holeinfo.color == groupColor); // 孔颜色相同
    //Q_ASSERT(holeinfo.isselected == true); // 不肯定是被选中的孔,切换objective时会更新holeinfo,此时isselected可能false
    Q_ASSERT(holeinfo.allgroup == allgroup);// 这个是一定相等
    Q_ASSERT(holeinfo.allcoordinate == allcoordinates); // 同理
    holeinfo.isselected = true; // 要设置孔为选中,不然就不能绘制高亮了
    holeinfo.viewrects = viewrects; // 本组应用的视野数量和位置信息
    holeinfo.viewsize = viewsize; // 本组应用的视野尺寸
    holeinfo.viewpoints = viewpoints; // 本组应用的电机坐标
    holeinfo.uipoints = uipoints; // 绘图使用的坐标
    mHoleInfo[coordinate.x()][coordinate.y()] = holeinfo;
    update();
}

// (5) 应用到组事件
void WellPattern::updateHoleInfoByViewInfoApplyGroup(QCVariantMap m)
{
    // 1. 关键信息: 视野区域
    auto groupName = m[HoleGroupNameField].toString();
    auto groupColor = m[HoleGroupColorField].toString();
    auto coordinate = m[HoleCoordinateField].toPoint();
    auto allgroup = m[WellAllGroupsField].value<QSet<QString>>();
    auto allcoordinates = m[WellAllHolesField].value<QPoint2DVector>();
    auto viewsize = m[HoleViewSizeField].toInt();
    auto viewrects = m[HoleViewRectsField].value<QRectFVector>(); // 关键信息
    auto viewpoints = m[HoleViewPointsField].value<ViewPointVector>(); // 关键信息
    auto uipoints = m[HoleViewUiPointsField].value<ViewPointVector>(); // 关键信息

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
                holeinfo.viewrects = viewrects; // 本组应用的视野数量和位置信息
                holeinfo.viewpoints = viewpoints;
                holeinfo.uipoints = uipoints;
                holeinfo.viewsize = viewsize; // 本组应用的视野尺寸
                mHoleInfo[row][col] = holeinfo;
            }
        }
    }
    update();
}

// (6) 应用到所有事件
void WellPattern::updateHoleInfoByViewInfoApplyAll(QCVariantMap m)
{
    auto groupName = m[HoleGroupNameField].toString();
    auto groupColor = m[HoleGroupColorField].toString();
    auto allcoordinates = m[WellAllHolesField].value<QPoint2DVector>();
    auto viewsize = m[HoleViewSizeField].toInt();
    auto viewrects = m[HoleViewRectsField].value<QRectFVector>(); // 关键信息
    auto viewpoints = m[HoleViewPointsField].value<ViewPointVector>(); // 关键信息
    auto uipoints = m[HoleViewUiPointsField].value<ViewPointVector>(); // 关键信息

    //切物镜时
    //LOG<<allcoordinates;
    //LOG<<getAllWellHoleCoordinates();
    Q_ASSERT(allcoordinates == getAllWellHoleCoordinates());

    foreach(auto holes, allcoordinates) {
        foreach(auto hole, holes) {
            auto holeinfo = mHoleInfo[hole.x()][hole.y()];
            holeinfo.isselected = true; // 要设置孔为选中,不然就不能绘制高亮了
            if (holeinfo.group == groupName) // 本组应用的组颜色,不同组的颜色不需要统一
                holeinfo.color = groupColor;
            holeinfo.viewrects = viewrects;
            holeinfo.viewpoints = viewpoints;
            holeinfo.uipoints = uipoints;
            holeinfo.viewsize = viewsize;
            mHoleInfo[hole.x()][hole.y()] = holeinfo;
        }
    }
    update();
}

// (7) 删孔逻辑-需要清除viewPattern对应的临时信息
void WellPattern::onRemoveHoleAct()
{
    mHoleInfo[mMousePos.x()][mMousePos.y()].group = QString();
    mHoleInfo[mMousePos.x()][mMousePos.y()].color = Qt::red;
    mHoleInfo[mMousePos.x()][mMousePos.y()].coordinate = mMousePos;
    mHoleInfo[mMousePos.x()][mMousePos.y()].viewsize = 0;
    mHoleInfo[mMousePos.x()][mMousePos.y()].viewrects = QRectFVector();
    mHoleInfo[mMousePos.x()][mMousePos.y()].viewpoints = ViewPointVector();
    mHoleInfo[mMousePos.x()][mMousePos.y()].uipoints = ViewPointVector();
    mHoleInfo[mMousePos.x()][mMousePos.y()].isselected = false;
    mHoleInfo[mMousePos.x()][mMousePos.y()].allcoordinate = QPoint2DVector();
    mHoleInfo[mMousePos.x()][mMousePos.y()].allgroup = QSet<QString>();

    mHoleInfo[mMousePos.x()][mMousePos.y()].type = QString();
    mHoleInfo[mMousePos.x()][mMousePos.y()].medicine = QString();
    mHoleInfo[mMousePos.x()][mMousePos.y()].doseunit = QString();
    mHoleInfo[mMousePos.x()][mMousePos.y()].dose = QString();

    // 清除视野窗口的缓存信息
    emit clearViewWindowCache(mMousePos);
    openviewact->trigger(); // 重新刷新一下
    update();
}
