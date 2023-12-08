/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-12-05 10:32:23
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-12-05 10:42:17
 * @FilePath: \EOS\component\src\widgets\wellpatternv1.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */

#include "WellPatternv1.h"

namespace obsoleteClass {

    WellPatternInfo WellPatternV1::patternInfo() const
    {
        WellPatternInfo info;

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

    WellPatternV1::WellPatternV1(int rows, int cols, QWidget *parent) : Pattern(rows,cols,parent)
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

        connect(setgroupact,&QAction::triggered,this,&WellPatternV1::onSetGroupAct);
        connect(openviewact,&QAction::triggered,this,&WellPatternV1::onOpenViewAct);
        connect(removeholeact,&QAction::triggered,this,&WellPatternV1::onRemoveHoleAct);
    }

    void WellPatternV1::setPatternSize(int rows, int cols)
    {
        Pattern::setPatternSize(rows,cols);
        initDrapPoints();
        initDisablePoints();
        initHoleInfo();
    }

    void WellPatternV1::initDrapPoints()
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

    void WellPatternV1::initDisablePoints()
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

    void WellPatternV1::setDisablePoint(QCPoint point, bool enable)
    { // 2x3,x只能取0,1,y取0,1,2,不能取等号
        if (point.x()< 0 || point.x() >= mrows // 防止越界,外部也有义务不许越界
            || point.y()<0 || point.y() >= mcols)
            return;
        mDisablePoints[point.x()][point.y()] = enable;
        update();
    }

    void WellPatternV1::setDisablePoints(QCPointVector points, bool enable)
    {// 对指定的有限坐标进行设置
        foreach(auto pt , points) {
            setDisablePoint(pt,enable);
        }
    }

    void WellPatternV1::setDisablePoints(bool enable)
    {// 对所有的坐标进行设置
        for(int r = 0; r < mrows; ++r) {
            for(int c = 0; c < mcols; ++c) {
                mDisablePoints[r][c] = enable;
            }
        }
        update();
    }

    int WellPatternV1::drapPointCount() const
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

    int WellPatternV1::numberOfViews() const
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

    int WellPatternV1::numberOfViews(const QPoint &holePoint)
    { // 外部调用holePoint有义务不越界
        int count = 0;
        if (mHoleInfo[holePoint.x()][holePoint.y()].isselected &&
            !mDisablePoints[holePoint.x()][holePoint.y()]) // 多层保护没坏处,虽然置灰点mHoleInfo对应的isselected一定是false
            count = mHoleInfo[holePoint.x()][holePoint.y()].viewpoints.count();
        return count;
    }

    int WellPatternV1::numberOfViews(const QString &group)
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

    QSet<QString> WellPatternV1::getAllWellGroupNames() const
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

    QPointVector WellPatternV1::getHoleGroupCoordinates(const QString &groupName) const
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

    QPoint2DVector WellPatternV1::getAllWellHoleCoordinates() const
    {
        QPoint2DVector points;

        auto allgroups = getAllWellGroupNames();

        foreach(auto group,allgroups) { // 把所有组的孔向量列表添加
            auto holes = getHoleGroupCoordinates(group);
            points.append(holes);
        }

        return points;
    }

    void WellPatternV1::initHoleInfo()
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
                info.viewrects = QRectFVector();
                info.viewpoints_v1 = ViewPointVector();
                info.uipoints_v1 = ViewPointVector();

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

    void WellPatternV1::clearAllHoleViewPoints()
    { // 切换物镜时调用,需要清理掉视野窗口映射的4个信息
        for(int row = 0 ; row < mrows; ++ row) {
            for (int col = 0; col < mcols; ++col) {
                mHoleInfo[row][col].viewrects = QRectFVector();
                mHoleInfo[row][col].viewpoints_v1 = ViewPointVector();
                mHoleInfo[row][col].uipoints_v1 = ViewPointVector();
                mHoleInfo[row][col].viewsize = 0;
            }
        }
        update();
    }

    void WellPatternV1::updateHoleInfo(QCPoint point,QCString group,QCPointFVector viewpoints,int viewsize)
    { // 更新指定孔的信息,用于导入实验配置时逐孔更新

        // 1. viewpoints存储的时候是电机坐标,并非UI坐标,需要转换
        ViewPointVector points;
        QRectFVector rects; // 基于电机坐标的等比例区域
        for(auto pt: viewpoints)
        {
            points.append(ViewPoint(pt.x(),pt.y())); // 当初电机坐标是按照1.0/viewsize去存的,现在的比例不变
            auto rect = QRectF(pt.x(),pt.y(),1.0/viewsize,1.0/viewsize);
            rects.append(rect);
        }

        // 2. 转换ui坐标需要 UI区域和掩码矩阵
        ViewPointVector uipoints; // 要转换的UI坐标
        for(int r = 0; r < mUiViewMaskSize; ++r) {
            for (int c = 0; c < mUiViewMaskSize; ++c) {
                for(auto viewRect: rects) {
                    auto topleft = viewRect.topLeft();
                    auto size = viewRect.size(); // 每个等比例UI区域要放大到固定的UI掩码矩阵尺寸

                    auto x = topleft.x() * mUiViewMaskSize ;
                    auto y = topleft.y() * mUiViewMaskSize ;
                    auto w = size.width() * mUiViewMaskSize;
                    auto h = size.height() * mUiViewMaskSize;

                    auto rect = QRectF(x,y,w,h); // 等比例放大尺寸到mUiViewMaskSize
                    if (rect.intersects(QRectF(c,r,1.0,1.0))) { // 掩码区域包含的点都视为UI点
                        ViewPoint point;
                        point.x = convertPrecision((r+0.5)/mUiViewMaskSize);
                        point.y = convertPrecision((c+0.5)/mUiViewMaskSize);
                        uipoints.append(point);
                    }
                }  // end 3层for
            }
        }


        Q_ASSERT(mHoleInfo[point.x()][point.y()].coordinate == point);

        // 3.颜色不更新,默认就红色,不同组的颜色会一样,因为当初没有存不同的颜色
        mHoleInfo[point.x()][point.y()].group = group;
        mHoleInfo[point.x()][point.y()].viewpoints_v1 = points;
        mHoleInfo[point.x()][point.y()].uipoints_v1 = uipoints;

        mHoleInfo[point.x()][point.y()].viewsize = viewsize;
        mHoleInfo[point.x()][point.y()].isselected = true;

        mHoleInfo[point.x()][point.y()].allgroup = getAllWellGroupNames();
        mHoleInfo[point.x()][point.y()].allcoordinate = getAllWellHoleCoordinates();
        // 不能借助onOpenViewAct打开视野窗口更新,因为不传递viewpoints给视野窗口,而且更新仅仅更新1个孔的
        update();
    }


// (1) 打开分组的对话框,以原有的信息去更新分组对话框的UI信息
    void WellPatternV1::onSetGroupAct()
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
    void WellPatternV1::updateHoleInfoByGroupInfo(QCVariantMap m)
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
    void WellPatternV1::onOpenViewAct()
    { // WellPatternV1组装6个信息,viewpattern自行组装2个信息,共8个
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
    void WellPatternV1::updateHoleInfoByViewInfoApplyHole(QCVariantMap m)
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
        holeinfo.viewpoints_v1 = viewpoints; // 本组应用的电机坐标
        holeinfo.uipoints_v1 = uipoints; // 绘图使用的坐标
        mHoleInfo[coordinate.x()][coordinate.y()] = holeinfo;
        update();
    }

// (5) 应用到组事件
    void WellPatternV1::updateHoleInfoByViewInfoApplyGroup(QCVariantMap m)
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
                    holeinfo.viewpoints_v1 = viewpoints;
                    holeinfo.uipoints_v1 = uipoints;
                    holeinfo.viewsize = viewsize; // 本组应用的视野尺寸
                    mHoleInfo[row][col] = holeinfo;
                }
            }
        }
        update();
    }

// (6) 应用到所有事件
    void WellPatternV1::updateHoleInfoByViewInfoApplyAll(QCVariantMap m)
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
                holeinfo.viewpoints_v1 = viewpoints;
                holeinfo.uipoints_v1 = uipoints;
                holeinfo.viewsize = viewsize;
                mHoleInfo[hole.x()][hole.y()] = holeinfo;
            }
        }
        update();
    }

// (7) 删孔逻辑-需要清除viewPattern对应的临时信息
    void WellPatternV1::onRemoveHoleAct()
    {
        mHoleInfo[mMousePos.x()][mMousePos.y()].group = QString();
        mHoleInfo[mMousePos.x()][mMousePos.y()].color = Qt::red;
        mHoleInfo[mMousePos.x()][mMousePos.y()].coordinate = mMousePos;
        mHoleInfo[mMousePos.x()][mMousePos.y()].viewsize = 0;
        mHoleInfo[mMousePos.x()][mMousePos.y()].viewrects = QRectFVector();
        mHoleInfo[mMousePos.x()][mMousePos.y()].viewpoints_v1 = ViewPointVector();
        mHoleInfo[mMousePos.x()][mMousePos.y()].uipoints_v1 = ViewPointVector();
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

    void WellPatternV1::mouseReleaseEvent(QMouseEvent *event)
    { // 框选数量大于1不允许打开视野窗口,只能基于特定孔
        if (mMouseEvent) {

            // 1. 点到边缘位置不能打开视野窗口和设置分组信息
            if (mMousePos == QPoint(-1,-1)){
                openviewact->setEnabled(false);
                setgroupact->setEnabled(false);
                removeholeact->setEnabled(false);
                return;
            }

            // 2.点到不可选的孔也不能打开
            if (mDisablePoints[mMousePos.x()][mMousePos.y()]) {
                openviewact->setEnabled(false);
                setgroupact->setEnabled(false);
                removeholeact->setEnabled(false);
                return;
            }

            // 3. 点到其它可用的孔且非边缘位置就可以打开分组窗口
            setgroupact->setEnabled(true);
            removeholeact->setEnabled(true);

            // 4. 框选的数量大于1个不允许打开视野动作使能,打开视野只能基于特定孔打开
            int count = drapPointCount();
            if (count > 1) // 只框选了1个不会弹
                openviewact->setEnabled(false);
            else openviewact->setEnabled(true);
        }
        event->accept();
    }

    void WellPatternV1::mouseMoveEvent(QMouseEvent *event)
    {// 得到框选孔的区域
        if (mMouseEvent) {
            if (event->buttons() & Qt::LeftButton)
            {
                initDrapPoints(); // 清除拖拽区域
                auto end = event->pos(); // 鼠标停下的点
                mDrapRect = QRectF(mLastPos,end); // 鼠标形成的矩形框
                auto rects = getChildRects();
                for(int row = 0; row < mrows; ++row)
                    for(int col = 0; col < mcols; ++col) {
                        if(mDrapRect.intersects(rects[row][col])// 小矩形区域在这个推拽区域内有交集
                           && !mDisablePoints[row][col]){ // 根源在mDrapPoints直接限制好,paintEvent不加
                            mDrapPoints[row][col] = true; //!mDisablePoints[row][col]判断也可以
                        }
                    }

            }
        }
        update();
        event->accept();
    }

    void WellPatternV1::mousePressEvent(QMouseEvent *event)
    { // 点左键要取消框选
        Pattern::mousePressEvent(event);
        if (event->button() == Qt::LeftButton) {
            initDrapPoints(); // 框选后，如果左键点一下应该取消框选
            mDrapRect.setWidth(0);
            mDrapRect.setHeight(0);
            mDrapRect = QRectF();
            update();
        } // 右键是菜单

        event->accept();
    }

    void WellPatternV1::mouseDoubleClickEvent(QMouseEvent*event)
    { // 双击也能打开视野窗口
        Pattern::mouseDoubleClickEvent(event);

        // 点到不可选的孔也不能打开,这样WellPatternV1的限制就禁止了viewpattern的打开,viewpattern不再需要针对置灰功能写逻辑
        if (mDisablePoints[mMousePos.x()][mMousePos.y()]) {
            openviewact->setEnabled(false);
            setgroupact->setEnabled(false);
            removeholeact->setEnabled(false);
            return;
        }
        openviewact->setEnabled(true);
        setgroupact->setEnabled(true);
        removeholeact->setEnabled(true);
        openviewact->trigger(); // 触发一下
    }

    QRectF2DVector WellPatternV1::getHoleRectsOnViewSize(const QPoint &coordinate) const
    {// 拿到某个孔基于视野窗口尺寸来划分的所有小矩形区域

        // 1. 获取孔内圆的半径+圆心坐标
        auto cell_size = getChildSize();
        double cell_w = cell_size.width();
        double cell_h = cell_size.height();
        double radius = cell_w>=cell_h? cell_h/2: cell_w/2; // 选较小的确保圆在矩形内
        radius = radius * 0.75;
        auto  centerPts = getCenterPoints();
        auto center  = centerPts[coordinate.x()][coordinate.y()]; // 圆心

        // 2. 根据圆心和内圆半径拿到内圆外接正方形区域
        auto rect = QRectF(center,QSize(radius*2,radius*2)); // 这个孔内圆的外接正方形

        // 3. 拿到这个孔存储的视野尺寸信息
        auto viewrows = mHoleInfo[coordinate.x()][coordinate.y()].viewsize;
        auto viewcols = viewrows;

        // 4. 得到外接正方形根据视野尺寸划分后每个小矩形的长度和宽度
        QRectF2DVector m;
        auto hoffset = 2 * radius / viewrows *1.0; // 整个外接正方形的尺寸就是2*radius
        auto voffset = 2 * radius / viewcols *1.0;

        // 4.拿到外界正方形的左上角顶点
        auto start = center-QPointF(radius,radius); // 圆心坐标减去水平垂直距离radius即可

        // 5. 有了左上角起点+每次水平垂直移动的小矩形尺寸就可以得到所有小矩形的区域
        for(int i = 0 ; i < viewrows; ++i) {
            QRectFVector vec;
            for(int j = 0; j < viewcols; ++j) { // j*offset加在x坐标也就是水平方向
                auto topleft = start + QPointF(j*hoffset,i*voffset); // x, x+d，j依次取0,1; y先不变在后
                auto bottomright = topleft + QPointF(hoffset,voffset);
                vec.append(QRectF(topleft,bottomright));
            }
            m.append(vec);
        }
        return m;
    }

    QRectF2DVector WellPatternV1::getHoleRectsOnViewSize(int x,int y) const
    {
        return getHoleRectsOnViewSize(QPoint(x,y));
    }

    void WellPatternV1::paintEvent(QPaintEvent *event)
    {
        //LOG<<mrows<<mcols<<mHoleInfo.count()<<mHoleInfo[0].count()<<mMousePos;
        Pattern::paintEvent(event);

        auto cell_size = getChildSize();
        int cell_w = cell_size.width();
        int cell_h = cell_size.height();
        int radius = cell_w>=cell_h? cell_h/2: cell_w/2; // 选较小的确保圆在矩形内
        //auto  centerPts = getCenterPoints();// 改为直接计算,减少循环

        QPainter painter(this);
        QPainterPath path;

        for(int row = 0 ; row < mrows; ++ row) {
            for (int col = 0; col < mcols; ++col) {
                //auto center = centerPts[row][col]; // 改为直接计算,减少循环
                auto center  = QPointF(mGap+cell_w/2+mSpace+(cell_w+mSpace)*col,
                                       mGap+cell_h/2+mSpace+(cell_h+mSpace)*row);
                // (1) 绘制鼠标点击的高亮
                //启用了鼠标事件mMousePos才会被赋值,否则是(-1,-1),所以这里不用再判断是否启用了鼠标事件
                if (mMousePos.x() == row && mMousePos.y() == col
                    && !mHoleInfo[row][col].isselected// 已绘制的点不要绘制鼠标选中高亮
                    && !mDisablePoints[row][col]) { // 置灰不可选的点不要绘制鼠标高亮
                    path.clear();
                    path.moveTo(center);
                    path.addEllipse(center, radius * 0.75, radius * 0.75);
                    painter.fillPath(path, mMouseClickColor);
                }

                // (2) 绘制框选的所有孔
                if (mDrapPoints[row][col]
                    // && !mHoleInfo[row][col].isSelected //绘制拖拽临时的点,如果有已被选中的不需要再绘制,不过这样取消选中时不能绘制拖拽的点感受不好,还是恢复
                    && !mDisablePoints[row][col]) {  // 不可选的孔不允许绘制
                    path.clear(); //
                    path.moveTo(center);
                    path.addEllipse(center, radius * 0.75, radius * 0.75);
                    painter.fillPath(path, mMouseClickColor);
                }

                // (3) 绘制确实选中的孔和绘制孔内选中的点
                if (mHoleInfo[row][col].isselected && !mDisablePoints[row][col]) //  绘制确定选中的点,不可选的孔不允许绘制
                {
                    // (3.1) 绘制确实选中的孔
                    path.clear();
                    path.moveTo(center);
                    path.addEllipse(center,radius*0.75,radius*0.75);
                    painter.fillPath(path,mHoleInfo[row][col].color);

                    // (3.2) 第1种绘制法: 直接绘制视野区域
                    auto rects = mHoleInfo[row][col].viewrects; // 视野选中区域不为空
                    if (!rects.isEmpty()) {
                        auto topleft = center-QPointF(radius*0.75,radius*0.75);// 圆孔内圆的外接正方形的左上角顶点
                        for(auto rect: rects) { // 可对照ViewPattern::mapToSize的逻辑
                            auto x = rect.topLeft().x() * radius * 1.5+ topleft.x();
                            auto y = rect.topLeft().y() * radius * 1.5 + topleft.y();
                            auto w = rect.size().width() * radius * 1.5;
                            auto h = rect.size().height() * radius * 1.5;
                            painter.fillRect(QRectF(x,y,w,h),Qt::black);
                        }
                    }
//                    // (3.3) 第2种绘制法: 只是绘制点,相比于第3种比较有效率,以点带面
//                    auto points = mHoleInfo[row][col].uipoints;
//                    if (!points.isEmpty()) {
//                        auto pen = painter.pen();
//                        pen.setWidth(4);
//                        painter.setPen(pen);
//                        auto topleft = center-QPointF(radius*0.75,radius*0.75);// 圆孔内圆的外接正方形的左上角顶点
//                        for(auto point: points) {
//                            auto x = point.x.toDouble() * radius * 1.5 + topleft.x();
//                            auto y = point.y.toDouble() * radius * 1.5 + topleft.y();
//                            painter.drawPoint(QPointF(x,y));
//                        }
//                        pen = painter.pen();
//                        pen.setWidth(2);
//                        painter.setPen(pen);
//                    }
//                }
                    // (3.4) 第3种绘制法: 把视野点整体缩放,单位小矩形的尺寸取决于viewPattern是怎么定义缩放比例的
//                auto points = mHoleInfo[row][col].uipoints;
//                if (!points.isEmpty()) {
//                    auto topleft = center-QPointF(radius*0.75,radius*0.75);// 圆孔内圆的外接正方形的左上角顶点
//                    if ((points.count() > mUiViewMaskSize*mUiViewMaskSize*0.2 && mrows == 16) // 384 视野上孔很小没必要做循环了,数量太多直接画个黑圆就得
//                         || (points.count() > mUiViewMaskSize*mUiViewMaskSize*0.9 && mrows == 8)) { // 96 这个可以超过9000太多时忽略掉,其他时候不卡正常绘制
//                        painter.fillRect(QRectF(topleft,QSizeF(radius*1.5,radius*1.5)),Qt::black);
//                    } else { // 对于384和96的一些优化,因为计算量比较大,而且WellPatternV1的孔很小时就没有必要按点画了,点特别多时看不出来直接画区域
//                        for(auto point: points) {
//                            auto x = point.x.toDouble() * radius * 1.5 + topleft.x();
//                            auto y = point.y.toDouble() * radius * 1.5 + topleft.y();
//                            auto w = 1.0 / mUiViewMaskSize * radius * 1.5; // viewPattern传来的坐标就是根据mViewMaskSize去缩放的
//                            auto h = 1.0 / mUiViewMaskSize * radius * 1.5; // 那么w,h也是根据这个倍数去缩放
//                            painter.fillRect(QRectF(x,y,w,h),Qt::black);
//                        }
//                    }
//                }
                }

                // (4) 绘制不可选置灰的点,将其高亮颜色变为灰色,并画个x
                if (mDisablePoints[row][col]) {
                    path.clear();
                    path.moveTo(center);
                    path.addEllipse(center,radius,radius);
                    painter.fillPath(path,mInnerCircleColor);

                    auto p11 = center + QPointF(radius*sqrt(2)/2,-radius* sqrt(2)/2);
                    auto p12 = center - QPointF(radius*sqrt(2)/2,-radius* sqrt(2)/2);
                    auto p21 = center + QPointF(radius*sqrt(2)/2,radius* sqrt(2)/2);
                    auto p22 = center - QPointF(radius*sqrt(2)/2,radius* sqrt(2)/2);
                    painter.drawLine(p11,p12);
                    painter.drawLine(p21,p22);
                }
            }
        }
        // (5) 绘制框选框(放最后画,防止被填充颜色覆盖)
        if (!mDrapRect.isNull()) {
            auto pen = painter.pen();
            pen.setColor(Qt::blue);
            painter.setPen(pen);
            painter.drawRect(mDrapRect);
            pen.setColor(Qt::black); // 恢复,否则绘制其他的都变颜色了
            painter.setPen(pen);
        }
        event->accept();
    }



}