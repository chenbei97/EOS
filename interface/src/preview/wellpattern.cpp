/*** 
 * @Author: chenbei97 chenbei_electric@163.com
 * @Date: 2023-10-27 08:53:17
 * @LastEditors: chenbei97 chenbei_electric@163.com
 * @LastEditTime: 2023-10-27 08:54:02
 * @FilePath: \EOS\interface\src\preview\wellpattern.cpp
 * @Copyright (c) 2023 by ${chenbei}, All Rights Reserved. 
 */
#include "wellpattern.h"

void WellPattern::paintEvent(QPaintEvent *event)
{
    Pattern::paintEvent(event);

    auto cell_size = getChildSize();
    int cell_w = cell_size.width();
    int cell_h = cell_size.height();
    int radius = cell_w>=cell_h? cell_h/2: cell_w/2; // 选较小的确保圆在矩形内
    auto  centerPts = getCenterPoints();

    QPainter painter(this);
    QPainterPath path;

    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            auto center = centerPts[row][col];

            // (1) 绘制鼠标点击的高亮
            //启用了鼠标事件mMousePos才会被赋值,否则是(-1,-1),所以这里不用再判断是否启用了鼠标事件
            if (mMousePos.x() == row && mMousePos.y() == col && !mHoleInfo[row][col].isselected) {
                path.clear(); // 已绘制的点不要绘制鼠标选中高亮
                path.moveTo(center);
                path.addEllipse(center, radius * 0.75, radius * 0.75);
                painter.fillPath(path, mMouseClickColor);
            }

            // (2) 绘制框选的所有孔
            if (mDrapPoints[row][col]) { // && !mHoleInfo[row][col].isSelected 绘制拖拽临时的点,如果有已被选中的不需要再绘制
                path.clear(); // 不过这样取消选中时不能绘制拖拽的点感受不好,还是恢复
                path.moveTo(center);
                path.addEllipse(center, radius * 0.75, radius * 0.75);
                painter.fillPath(path, mMouseClickColor);
            }

            // (3) 绘制确实选中的孔和绘制孔内选中的点
            if (mHoleInfo[row][col].isselected) //  绘制确定选中的点
            {
                path.clear();
                path.moveTo(center);
                path.addEllipse(center,radius*0.75,radius*0.75);
                painter.fillPath(path,mHoleInfo[row][col].color);
//                auto viewrows = mHoleInfo[row][col].viewsize.width();
//                auto viewcols = mHoleInfo[row][col].viewsize.height();
//                auto holerects = getHoleRects(QPoint(row,col));
//                for(int i = 0; i < viewrows; ++i ) {
//                    for(int j = 0; j < viewcols; ++j) {
//                        painter.fillRect(holerects[i][j],Qt::black);
//                    }
//                }
                auto points = mHoleInfo[row][col].points;
                if (!points.isEmpty()) {
                    for(int i = 0; i < points.count(); ++i) {
                        //painter.drawPoint(points[i]);// 这个不是当前孔的实际物理坐标
                        auto holerects = getHoleRects(QPoint(row,col));
                        auto pt = points[i];
                        painter.fillRect(holerects[pt.x()][pt.y()],Qt::black);
                    }
                }
            }


        }
    }
    // (4) 绘制框选框(放最后画,防止被填充颜色覆盖)
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

void WellPattern::mouseReleaseEvent(QMouseEvent *event)
{
    if (mMouseEvent) {
        //LOG<<"mouse point = "<<mMousePos;
        if (mMousePos == QPoint(-1,-1)){
            viewact->setEnabled(false);
            groupact->setEnabled(false);
            return; // 可能会点到边缘位置
        }

        groupact->setEnabled(true);

        int count = drapPointCount();
        if (count > 1) // 只框选了1个不会弹
            viewact->setEnabled(false);
        else viewact->setEnabled(true);
        //emit drapEvent(mHoleInfo[mMousePos.x()][mMousePos.y()].color);

//        if (event->button() == Qt::LeftButton) {
//            mDrapRect.setWidth(0); // 清除拖拽矩形(不需要了,pressevent是左键就会清除了)
//            mDrapRect.setHeight(0);
//            mDrapRect = QRectF();
//        }
    }
    event->accept();
}

void WellPattern::mouseMoveEvent(QMouseEvent *event)
{
    if (mMouseEvent) {
        if (event->buttons() & Qt::LeftButton)
        {
            initDrapPoints(); // 清除拖拽区域
            auto end = event->pos(); // 鼠标停下的点
            mDrapRect = QRectF(mLastPos,end); // 鼠标形成的矩形框
            auto rects = getChildRects();
            for(int row = 0; row < mrows; ++row)
                for(int col = 0; col < mcols; ++col) {
                    if(mDrapRect.intersects(rects[row][col])){ // 小矩形区域在这个推拽区域内有交集
                        mDrapPoints[row][col] = true;
                    }
                }

        }
    }
    update();
    event->accept();
}

void WellPattern::mousePressEvent(QMouseEvent *event)
{
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

void WellPattern::setHoleInfoByGroupInfo(QCVariantMap m)
{ // 分组窗口的信息拿去更新数据(不需要更新points,viewsize)
    auto gcolor = m[GroupColorField].toString();
    auto gname = m[GroupNameField].toString();
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col){
            auto pt = mDrapPoints[row][col]; // 所有拖拽的点都分为一组
            if (pt){
                mHoleInfo[row][col].isselected = true;//框选内对应的点都设为选中
                mHoleInfo[row][col].color = gcolor; // 颜色跟随窗口设置的颜色
                mHoleInfo[row][col].group = gname;
                mDrapPoints[row][col] = false; // 拖拽区域内的点也要更新为false,不然还会绘制这个区域
            }
        }
    }

    // 框选的时候会遗漏鼠标当前选中的点
    if (mMousePos != QPoint(-1,-1)) {// 没启用鼠标事件或者点击外围,这是{-1,-1}会越界
        mHoleInfo[mMousePos.x()][mMousePos.y()].isselected = true; // 鼠标点击的这个
        mHoleInfo[mMousePos.x()][mMousePos.y()].color = gcolor;
        mHoleInfo[mMousePos.x()][mMousePos.y()].group = gname;
        mDrapPoints[mMousePos.x()][mMousePos.y()] = false;
    }

    //LOG<<"mHoleInfo = "<<mHoleInfo[mMousePos.x()][mMousePos.y()];
    update();
}

void WellPattern::setHoleInfoByViewInfo(QCVariantMap m)
{// 视野窗口的信息拿去更新数据(PreviewPhotoCanvas::onApplyGroupAct())
   // (不需要更新color,isselected,group)
    auto groupName = m[GroupNameField].toString();
    auto groupColor = m[GroupColorField].toString();
    auto points = m[PointsField].value<QPointVector>();
    auto viewsize = m[ViewSizeField].toSize();
    auto coordinate = m[CoordinateField].toPoint();
    //LOG<<groupName<<coordinate<<points<<viewsize;

    // 把coordinate对应的组都更新
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col) {
            auto holeinfo = mHoleInfo[row][col];
            if (holeinfo.group == groupName) {
                //Q_ASSERT(holeinfo.coordinate == coordinate); // 视野窗口的孔坐标是双击的那个孔坐标,组内其他空不一定是这个坐标,不相等
               // Q_ASSERT(holeinfo.color == groupColor); // 颜色本组一定相同,好像也不是,有可能就是颜色不一样
                Q_ASSERT(holeinfo.isselected == true); // 肯定是被选中的孔
                holeinfo.color = groupColor; // 让本组内其他的孔都要跟随本孔颜色
                holeinfo.points = points; // 那个孔选择的所有视野都应用到组
                holeinfo.viewsize = viewsize; // 这个孔划分的视野尺寸(这是为了绘图画点时,视野坐标=>视野物理坐标)
                mHoleInfo[row][col] = holeinfo;
            }
        }
    }
    update();
}

void WellPattern::initDrapPoints()
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

QPointVector WellPattern::holeGroupPoints(const QString &groupName) const
{ // 获取组内的所有孔坐标
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

QRectF2DVector WellPattern::getHoleRects(const QPoint &coordinate) const
{// 拿到某个孔的所有划分的小矩形区域
    auto cell_size = getChildSize();
    double cell_w = cell_size.width();
    double cell_h = cell_size.height();
    double radius = cell_w>=cell_h? cell_h/2: cell_w/2; // 选较小的确保圆在矩形内
    radius = radius * 0.75;
    auto  centerPts = getCenterPoints();
    auto center  = centerPts[coordinate.x()][coordinate.y()]; // 这个孔的坐标

    auto rect = QRectF(center,QSize(radius*2,radius*2)); // 这个孔的外接正方形

    auto viewrows = mHoleInfo[coordinate.x()][coordinate.y()].viewsize.width();
    auto viewcols = mHoleInfo[coordinate.x()][coordinate.y()].viewsize.height();

    QRectF2DVector m;
    auto hoffset = 2 * radius / viewrows *1.0;
    auto voffset = 2 * radius / viewcols *1.0;

    auto start = center-QPointF(radius,radius); // 矩形区域左上角顶点
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

void WellPattern::initHoleInfo()
{ // 初始化所有孔内的包含信息
    mHoleInfo.clear();
    for(int row = 0 ; row < mrows; ++ row) {
        QHoleInfoVector var;
        for (int col = 0; col < mcols; ++col){
            HoleInfo info;
            info.coordinate = QPoint(row,col);
            info.color = Qt::red;
            info.isselected = false;
            var.append(info);
        }
        mHoleInfo.append(var);
    }
    update();
}

void WellPattern::onSetViewAct()
{ // 打开选择视野窗口的事件,需要传递当前孔的坐标,孔的分组
    if (mMousePos != QPoint(-1,-1)){
        // 把孔的信息也带出去更新视野窗口的框选颜色,坐标,所在组
        QVariantMap m;
        Q_ASSERT(mHoleInfo[mMousePos.x()][mMousePos.y()].coordinate == mMousePos);
        m[CoordinateField] = mMousePos; // 告知视野当前孔坐标
        m[GroupNameField] = mHoleInfo[mMousePos.x()][mMousePos.y()].group; // 所在组
        m[GroupColorField] = mHoleInfo[mMousePos.x()][mMousePos.y()].color; // 组的颜色
        QVariant v;
        v.setValue(holeGroupPoints(mHoleInfo[mMousePos.x()][mMousePos.y()].group));
        m[GroupPointsField] = v;
        emit viewEvent(m);
    }
}

void WellPattern::mouseDoubleClickEvent(QMouseEvent*event)
{
    Pattern::mouseDoubleClickEvent(event);
    viewact->trigger(); // 触发一下
}

void WellPattern::onSetGroupAct()
{ // 颜色和名称信息去更新拖动事件窗口的信息
    auto color = mHoleInfo[mMousePos.x()][mMousePos.y()].color;
    auto name = mHoleInfo[mMousePos.x()][mMousePos.y()].group;
    QVariantMap m;
    m[GroupColorField] = color;
    m[GroupNameField] = name;
    emit drapEvent(m);
}

WellPattern::WellPattern(int rows, int cols, QWidget *parent) : Pattern(rows,cols,parent)
{
    mMouseEvent = true;
    groupact = new QAction(tr("分组"));
    viewact = new QAction(tr("选点"));
    addAction(groupact);
    addAction(viewact);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    initDrapPoints();
    initHoleInfo();

    connect(groupact,&QAction::triggered,this,&WellPattern::onSetGroupAct);
    connect(viewact,&QAction::triggered,this,&WellPattern::onSetViewAct);
}

void WellPattern::setPatternSize(int rows, int cols)
{
    Pattern::setPatternSize(rows,cols);
    initDrapPoints();
    initHoleInfo();
}