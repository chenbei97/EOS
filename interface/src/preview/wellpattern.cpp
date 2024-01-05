/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-27 08:53:17
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-08 17:25:35
 * @FilePath: \EOS\interface\src\preview\wellpattern.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "wellpattern.h"

// (1) 打开分组的对话框,以原有的信息去更新分组对话框的UI信息
void WellPattern::onSetGroupAct()
{
    auto color = mHoleInfo[mMousePos.x()][mMousePos.y()].color;
    auto name = mHoleInfo[mMousePos.x()][mMousePos.y()].group;
    auto dose = mHoleInfo[mMousePos.x()][mMousePos.y()].dose;
    auto unit = mHoleInfo[mMousePos.x()][mMousePos.y()].doseunit;
    auto medicine = mHoleInfo[mMousePos.x()][mMousePos.y()].medicine;

    QVariantMap m;
    m[HoleGroupColorField] = color;
    m[HoleGroupNameField] = name;
    m[HoleDoseUnitField] = unit;
    m[HoleDoseField] = dose;
    m[HoleMedicineField] = medicine;
    m[HoleAllGroupsField].setValue(getAllGroups());

    emit openWellGroupWindow(m);
}

// (2) 被分组对话框的信息反过来更新孔信息
void WellPattern::updateHoleInfo(QCVariantMap m)
{
    // 1. 更新分组对话框提供的信息
    auto gcolor = m[HoleGroupColorField].toString();
    auto gname = m[HoleGroupNameField].toString();
    auto medicine = m[HoleMedicineField].toString();
    auto dose = m[HoleDoseField].toString();
    auto unit = m[HoleDoseUnitField].toString();
    //LOG<<"well accept info from groupwin is "<<gtype<<gname<<gcolor<<medicine<<dose<<unit;

    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col){
            auto pt = mDrapHoles[row][col]; // 右击分组时当前拖拽区域内的点是一组
            if (pt && !mDisableHoles[row][col]){ // 是拖拽区域的(拖拽区域已经不会包含置灰的不可选的孔,mouseMoveEvent已经对其限制),不过多一层保护没坏处
                mHoleInfo[row][col].isselected = true;//框选内对应的点都设为选中
                mHoleInfo[row][col].group = gname; // 名称是分组窗口设置的分组
                mHoleInfo[row][col].color = gcolor; // 颜色跟随分组窗口设置的颜色
                mHoleInfo[row][col].coordinate = QPoint(row,col); // 孔坐标
                mHoleInfo[row][col].medicine = medicine; // 本孔分配的样品
                mHoleInfo[row][col].dose = dose; // 本孔分配的剂量
                mHoleInfo[row][col].doseunit = unit; // 本孔分配的剂量单位
                mDrapHoles[row][col] = false; // 拖拽区域内的点也要更新为false,不然还会绘制这个区域

                // 视野的信息无需更新,只更新分组对话框提供的信息
            }
        }
    }

    // 2. 框选的时候会遗漏鼠标当前选中的点
    if (mMousePos != QPoint(-1,-1) && !mDisableHoles[mMousePos.x()][mMousePos.y()]) {// 没启用鼠标事件或者点击外围,这是{-1,-1}会越界
        // 注意2个判断先后顺序,如果是-1,-1就返回了,编译器不会执行后边的判断也就不会越界
        mHoleInfo[mMousePos.x()][mMousePos.y()].isselected = true; // 鼠标点击的这个
        mHoleInfo[mMousePos.x()][mMousePos.y()].color = gcolor;
        mHoleInfo[mMousePos.x()][mMousePos.y()].group = gname;
        mHoleInfo[mMousePos.x()][mMousePos.y()].coordinate = mMousePos;
        mHoleInfo[mMousePos.x()][mMousePos.y()].medicine = medicine; // 本孔分配的样品
        mHoleInfo[mMousePos.x()][mMousePos.y()].dose = dose; // 本孔分配的剂量
        mHoleInfo[mMousePos.x()][mMousePos.y()].doseunit = unit; // 本孔分配的剂量单位
        mDrapHoles[mMousePos.x()][mMousePos.y()] = false;
    }

    // 3. 更新已有的组信息(要在上边的group更新过以后再重新计算)
    auto gnames = getAllGroups();//有可能分了a,b组,b改名a,只有a了,所以要总是重新根据group算
    //LOG<<"current well all groups = "<<gnames;
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            mHoleInfo[row][col].allgroup = gnames; // 不分拖拽区域,所有孔都更新相同的组别信息,用等于可以防止以前组信息还保留
        }
    }
    mHoleInfo[mMousePos.x()][mMousePos.y()].allgroup = gnames;

    // 4. 更新已有的所有选中的孔信息
    auto holes = getAllGroupHoles();// 所有按组得到的二维孔向量
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
        mHoleInfo[mMousePos.x()][mMousePos.y()].allgroup = getAllGroups();
        m[HoleAllGroupsField].setValue(mHoleInfo[mMousePos.x()][mMousePos.y()].allgroup); // 已有的所有组(每次设置分组信息时会更新)
        m[HoleGroupCoordinatesField].setValue(getGroupHoles(mHoleInfo[mMousePos.x()][mMousePos.y()].group));
        mHoleInfo[mMousePos.x()][mMousePos.y()].allcoordinate = getAllGroupHoles();
        m[HoleAllCoordinatesField].setValue(mHoleInfo[mMousePos.x()][mMousePos.y()].allcoordinate);
        //LOG<<"well send info to view is "<<m[HoleGroupNameField].toString()<<m[HoleGroupColorField].toString();//ViewPattern::setStrategy接收
        emit openWellViewWindow(m);
    }
}

// (4) 保存或删除视野的应用到孔事件
void WellPattern::applyHoleEvent(QCVariantMap m)
{
    // 1. 关键信息: 视野区域-绘图 视野坐标-电机坐标
    auto groupName = m[HoleGroupNameField].toString();
    auto groupColor = m[HoleGroupColorField].toString();
    auto coordinate = m[HoleCoordinateField].toPoint();
    auto allgroup = m[HoleAllGroupsField].value<QSet<QString>>();
    auto allcoordinates = m[HoleAllCoordinatesField].value<QPoint2DVector>();
    // HoleGroupCoordinatesField 该组的所有孔坐标不解析

#ifdef viewRowColUnEqual
    auto dimension = m[HoleViewSizeField].value<Dimension2D>();
#else
    auto viewsize = m[HoleViewSizeField].toInt();
#endif
    auto viewpoints = m[HoleViewPointsField].value<QPointFVector>();// 关键信息

    //LOG<<"coord: "<<coordinate<<"viewpoint count = "<<viewpoints.count()<<" viewrect count = "<<viewrects.count();
    // 2. 把信息更新到本孔
    auto holeinfo = mHoleInfo[coordinate.x()][coordinate.y()];
    Q_ASSERT(holeinfo.coordinate == coordinate); // 孔坐标和当前传递的孔坐标相同
    Q_ASSERT(holeinfo.color == groupColor); // 孔颜色相同
    //Q_ASSERT(holeinfo.isselected == true); // 不肯定是被选中的孔,切换objective时会更新holeinfo,此时isselected可能false
    //Q_ASSERT(holeinfo.allgroup == allgroup);// 这个是一定相等(导入实验配置这不一定相等因为不导入)
    //Q_ASSERT(holeinfo.allcoordinate == allcoordinates); // 同理
    holeinfo.allgroup = getAllGroups();
    holeinfo.allcoordinate = getAllGroupHoles();
    holeinfo.isselected = true; // 要设置孔为选中,不然就不能绘制高亮了
#ifdef viewRowColUnEqual // 本组应用的视野尺寸
    holeinfo.dimension = dimension;
#else
    holeinfo.viewsize = viewsize;
#endif

    holeinfo.viewpoints = viewpoints; // 本组应用的电机坐标

    if (mSelectMode == ViewMode::RectMode) { // 区域模式下才会有这2个信息,点模式下不要获取,这样得到的是空向量把已有的覆盖了
        auto viewrects = m[HoleViewRectsField].value<QRectFVector>(); // 关键信息
        auto uipoints = m[HoleViewUiPointsField].value<QPointFVector>(); // 关键信息
        holeinfo.viewrects = viewrects; // 本组应用的视野数量和位置信息
        holeinfo.uipoints = uipoints; // 绘图使用的坐标
    }

    mHoleInfo[coordinate.x()][coordinate.y()] = holeinfo;
    update();
}

// (5) 应用到组事件
void WellPattern::applyGroupEvent(QCVariantMap m)
{
    // 1. 关键信息: 视野区域
    auto groupName = m[HoleGroupNameField].toString();
    auto groupColor = m[HoleGroupColorField].toString();
    auto coordinate = m[HoleCoordinateField].toPoint();
    auto allgroup = m[HoleAllGroupsField].value<QSet<QString>>();
    auto allcoordinates = m[HoleAllCoordinatesField].value<QPoint2DVector>();

#ifdef viewRowColUnEqual
    auto dimension = m[HoleViewSizeField].value<Dimension2D>();
#else
    auto viewsize = m[HoleViewSizeField].toInt();
#endif
    auto viewpoints = m[HoleViewPointsField].value<QPointFVector>(); // 关键信息

    QRectFVector viewrects;
    QPointFVector uipoints;
    if (mSelectMode == ViewMode::RectMode) {
        viewrects = m[HoleViewRectsField].value<QRectFVector>(); // 关键信息
        uipoints = m[HoleViewUiPointsField].value<QPointFVector>(); // 关键信息
    }

    // 2. 根据视野窗口传来的组名 把coordinate对应的组(color+viewpoints,viewsize)都更新 (不需要更新group,allgroup,dose,medicine,unit,type)
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            auto holeinfo = mHoleInfo[row][col];
            if (holeinfo.group == groupName) { // 本组的
                //Q_ASSERT(holeinfo.coordinate == coordinate); // 组其它孔坐标和当前传递的孔坐标不同
                // Q_ASSERT(holeinfo.color == groupColor); // 本组不同孔颜色可能不同
                // Q_ASSERT(holeinfo.isselected == true); // 不肯定是被选中的孔,切换objective时会更新mrows,此时isselected可能false
                //Q_ASSERT(holeinfo.allgroup == allgroup);// 这个是一定相等,并不是,偶然会不相等暂时没找到原因
                Q_ASSERT(holeinfo.allcoordinate == allcoordinates); // 同理
                LOG<<holeinfo.allcoordinate<<allcoordinates;
                holeinfo.allcoordinate = allcoordinates;
                holeinfo.isselected = true; // 要设置孔为选中,不然就不能绘制高亮了
                holeinfo.color = groupColor; // 本组应用的组颜色(有可能同组不同孔的颜色不同,帮助统一化)
                if (mSelectMode == ViewMode::RectMode) {
                    holeinfo.viewrects = viewrects; // 本组应用的视野数量和位置信息
                    holeinfo.uipoints = uipoints;
                }
                holeinfo.viewpoints = viewpoints;
#ifdef viewRowColUnEqual // 本组应用的视野尺寸
                holeinfo.dimension = dimension;
#else
                holeinfo.viewsize = viewsize;
#endif
                mHoleInfo[row][col] = holeinfo;
            }
        }
    }
    update();
}

// (6) 应用到所有事件
void WellPattern::applyAllEvent(QCVariantMap m)
{
    auto groupName = m[HoleGroupNameField].toString();
    auto groupColor = m[HoleGroupColorField].toString();
    auto allcoordinates = m[HoleAllCoordinatesField].value<QPoint2DVector>();
#ifdef viewRowColUnEqual
    auto dimension = m[HoleViewSizeField].value<Dimension2D>();
#else
    auto viewsize = m[HoleViewSizeField].toInt();
#endif
    auto viewpoints = m[HoleViewPointsField].value<QPointFVector>(); // 关键信息
    QRectFVector viewrects;
    QPointFVector uipoints;
    if (mSelectMode == ViewMode::RectMode) {
        viewrects = m[HoleViewRectsField].value<QRectFVector>(); // 关键信息
        uipoints = m[HoleViewUiPointsField].value<QPointFVector>(); // 关键信息
    }

    //切物镜时
    //LOG<<allcoordinates;
    //LOG<<getAllWellHoleCoordinates();
    Q_ASSERT(allcoordinates == getAllGroupHoles());

    for(auto holes: allcoordinates) {
        for(auto hole: holes) {
            auto holeinfo = mHoleInfo[hole.x()][hole.y()];
            holeinfo.isselected = true; // 要设置孔为选中,不然就不能绘制高亮了
            if (holeinfo.group == groupName) // 本组应用的组颜色,不同组的颜色不需要统一
                holeinfo.color = groupColor;
            if (mSelectMode == ViewMode::RectMode) {
                holeinfo.viewrects = viewrects;
                holeinfo.uipoints = uipoints;
            }
            holeinfo.viewpoints = viewpoints;
#ifdef viewRowColUnEqual // 本组应用的视野尺寸
            holeinfo.dimension = dimension;
#else
            holeinfo.viewsize = viewsize;
#endif
            mHoleInfo[hole.x()][hole.y()] = holeinfo;
        }
    }
    update();
}

// (7) 删孔逻辑-需要清除viewPattern对应的临时信息
void WellPattern::onRemoveHoleAct()
{
    if (mDrapRect.isEmpty()) {
        mHoleInfo[mMousePos.x()][mMousePos.y()].group = QString();
        mHoleInfo[mMousePos.x()][mMousePos.y()].color = WhiteColor;
        mHoleInfo[mMousePos.x()][mMousePos.y()].coordinate = mMousePos;
#ifdef viewRowColUnEqual
        mHoleInfo[mMousePos.x()][mMousePos.y()].dimension = Dimension2D();
#else
        mHoleInfo[mMousePos.x()][mMousePos.y()].viewsize = 0;
#endif
        mHoleInfo[mMousePos.x()][mMousePos.y()].viewrects = QRectFVector();
        mHoleInfo[mMousePos.x()][mMousePos.y()].viewpoints = QPointFVector();
        mHoleInfo[mMousePos.x()][mMousePos.y()].uipoints = QPointFVector();
        mHoleInfo[mMousePos.x()][mMousePos.y()].isselected = false;
        mHoleInfo[mMousePos.x()][mMousePos.y()].allcoordinate = QPoint2DVector();
        mHoleInfo[mMousePos.x()][mMousePos.y()].allgroup = QSet<QString>();

        mHoleInfo[mMousePos.x()][mMousePos.y()].medicine = QString();
        mHoleInfo[mMousePos.x()][mMousePos.y()].doseunit = QString();
        mHoleInfo[mMousePos.x()][mMousePos.y()].dose = QString();

        // 清除视野窗口的缓存信息
        openviewact->trigger(); // 重新刷新一下,先刷新再移除
        emit removeHole(mMousePos);
    } else { // 支持框删除
        for(int r = 0; r < mrows; ++r) {
            for(int c = 0; c < mcols; ++c) {
                if (mDrapHoles[r][c]) {
                    mHoleInfo[r][c].group = QString();
                    mHoleInfo[r][c].color = WhiteColor;
                    mHoleInfo[r][c].coordinate = mMousePos;
#ifdef viewRowColUnEqual
                    mHoleInfo[r][c].dimension = Dimension2D();
#else
                    mHoleInfo[r][c].viewsize = 0;
#endif
                    mHoleInfo[r][c].viewrects = QRectFVector();
                    mHoleInfo[r][c].viewpoints = QPointFVector();
                    mHoleInfo[r][c].uipoints = QPointFVector();
                    mHoleInfo[r][c].isselected = false;
                    mHoleInfo[r][c].allcoordinate = QPoint2DVector();
                    mHoleInfo[r][c].allgroup = QSet<QString>();

                    mHoleInfo[r][c].medicine = QString();
                    mHoleInfo[r][c].doseunit = QString();
                    mHoleInfo[r][c].dose = QString();

                    //openviewact->trigger(); // 重新刷新一下
                    QVariantMap m;
                    m[HoleCoordinateField] = QPoint(r,c); // 告知视野当前孔坐标
                    m[HoleGroupNameField] = mHoleInfo[r][c].group; // 所在组
                    m[HoleGroupColorField] = mHoleInfo[r][c].color; // 组的颜色
                    mHoleInfo[r][c].allgroup = getAllGroups();
                    m[HoleAllGroupsField].setValue(mHoleInfo[r][c].allgroup); // 已有的所有组(每次设置分组信息时会更新)
                    m[HoleGroupCoordinatesField].setValue(getGroupHoles(mHoleInfo[r][c].group));
                    mHoleInfo[r][c].allcoordinate = getAllGroupHoles();
                    m[HoleAllCoordinatesField].setValue(mHoleInfo[r][c].allcoordinate);
                    emit openWellViewWindow(m);
                    emit removeHole(QPoint(r,c));
                }
            }
        }
    }

    emit toggleWellStack(); // 刷新后总是进入视野模式需要重复退出比较麻烦帮用户做好
    emit groupChanged(getAllGroups());
    update();
}

// (8) 切换点模式-区域模式
void WellPattern::selectModeChangedEvent(QCVariantMap m)
{
    auto coordinate = m[HoleCoordinateField].toPoint();
    auto viewpoints = m[HoleViewPointsField].value<QPointFVector>();// 电机坐标
    auto holeinfo = mHoleInfo[coordinate.x()][coordinate.y()];
    holeinfo.viewpoints = viewpoints;
    mHoleInfo[coordinate.x()][coordinate.y()] = holeinfo;
    update();
}

// (9) 导入外部配置信息
void WellPattern::importHoleInfo(const QHoleInfoVector& vec,ViewMode mode)
{
    setViewMode(mode);
//    if (!vec.isEmpty()) { // 还是不动电机,因为此时应该是禁用状态
//        mMousePos = vec.at(0).coordinate;
//        emit holeClicked(mMousePos);
//    }
    for(auto holeInfo: vec) {
        auto x = holeInfo.coordinate.x();
        auto y = holeInfo.coordinate.y();
        mHoleInfo[x][y] = holeInfo; // importHoleInfo之前已经调用setPatternSize不会越界
    }
    auto allgroups = getAllGroups(); // 把组名信息和所有孔信息导入后重新计算
    auto allholes = getAllGroupHoles();
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            mHoleInfo[row][col].allcoordinate = allholes; // 再赋值
            mHoleInfo[row][col].allgroup = allgroups;
        }
    }
    update();
}

WellPatternInfo WellPattern::patternInfo() const
{
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
    return info;
}

WellPattern::WellPattern(int rows, int cols, QWidget *parent) : Pattern(rows,cols,parent)
{
    mMouseEvent = true;
    mHoleClickEvent = false;
    mEnterViewEvent = false;
    opengroupact = new QAction(tr(GroupHoleActTitle));
    openviewact = new QAction(tr(EnterViewActTitle));
    removeholeact = new QAction(tr(RemoveHoleActTitle));
    addAction(opengroupact);
    addAction(removeholeact);
    addAction(openviewact);
    openviewact->setEnabled(false);
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

ViewMode WellPattern::viewMode() const
{
    return mSelectMode;
}

void WellPattern::setOpenViewEnabled(bool enable)
{ // location select按钮的信号
    setHoleClickEvent(enable); // 是否允许触发电机预览事件
    setEnterViewEvent(enable); // 是否允许进入视野模式
    openviewact->setEnabled(enable); // come into view的动作禁用
    update();
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

QPoint2DVector WellPattern::getAllGroupHoles() const
{ // 获取所有选过的孔坐标
    QPoint2DVector points;

    auto allgroups = getAllGroups();

    for(auto group:allgroups) { // 把所有组的孔向量列表添加
        auto holes = getGroupHoles(group);
        points.append(holes);
    }

    return points;
}

QPointVector WellPattern::getAllHoles() const
{
    QPointVector vec;
    for(int r = 0; r < mrows; ++r) {
        for(int c = 0; c < mcols; ++c) {
            auto holeinfo = mHoleInfo[r][c];
            if (!holeinfo.group.isEmpty() && !mDisableHoles[r][c]) {
                vec.append(holeinfo.coordinate);
            }
        }
    }

    return vec;
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
            info.color = WhiteColor;
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


