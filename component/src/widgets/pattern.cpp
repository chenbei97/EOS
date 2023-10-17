#include "pattern.h"

void Pattern::drawHighlight(QPainter&painter)
{
    auto cell_size = getChildSize();
    int cell_w = cell_size.width();
    int cell_h = cell_size.height();

    int radius = cell_w>=cell_h? cell_h/2: cell_w/2; // 选较小的确保圆在矩形内

    auto  centerPts = getCenterPoints();
    QPainterPath path;

    for(int row = 0 ; row < mrows; ++ row)
    {
        for(int col = 0; col < mcols; ++ col)
        {
            auto center = centerPts[row][col];
            painter.drawEllipse(center,radius,radius); // 绘制外圆
            painter.drawEllipse(center,radius*0.75,radius*0.75); // 绘制内圆

            if (mState != UnInitialState) // 常规状态只有内外圆,内圆没有填充
            {
                path.clear();
                path.addEllipse(center,radius*0.75,radius*0.75); // 填充内圆
                painter.fillPath(path,mInnerCircleColor);
            }

            // 启用了鼠标事件mMousePos才会被赋值,否则是(-1,-1),所以这里不用再判断是否启用了鼠标事件
            if (mMousePos.x() == row && mMousePos.y() == col && !mSelectPoints[row][col].first)
            { // 已绘制的点不要绘制鼠标选中高亮
                path.clear();
                path.moveTo(center);
                path.addEllipse(center,radius*0.75,radius*0.75);
                painter.fillPath(path,mMouseClickColor);
            }

            if (!mDrapRect.isNull()) {
                auto pen = painter.pen();
                pen.setColor(Qt::blue);
                painter.setPen(pen);
                painter.drawRect(mDrapRect);
                pen.setColor(Qt::black); // 恢复,否则绘制其他的都变颜色了
                painter.setPen(pen);
            }

            if (mDrapPoints[row][col] ) { // && !mSelectPoints[row][col] 绘制拖拽临时的点,如果有已被选中的不需要再绘制
                path.clear(); // 不过这样取消选中时不能绘制拖拽的点感受不好,还是恢复
                path.moveTo(center);
                path.addEllipse(center,radius*0.75,radius*0.75);
                painter.fillPath(path,mMouseClickColor);
            }

            if (mSelectPoints[row][col].first) //  绘制确定选中的点
            {
                path.clear();
                path.moveTo(center);
                path.addEllipse(center,radius*0.75,radius*0.75);
                painter.fillPath(path,mSelectPoints[row][col].second);
            }

        }
    }
}

void Pattern::mousePressEvent(QMouseEvent *event)
{
    if (mMouseEvent) {
        if (event->button() == Qt::LeftButton) // 框选完不会自动清除颜色,但是右键还需要菜单,不能清除掉
            initDrapPoints();

        mLastPos = event->pos();
        auto rects = getChildRects(); // 所有小正方形区域匹配这个坐标
        for(int row = 0; row < mrows; ++row)
            for(int col = 0; col < mcols; ++col)
                if (rects[row][col].contains(mLastPos))
                    mMousePos = {row,col};
        update();
        emit mouseClicked(mMousePos);
    }

    event->accept();
}

void Pattern::mouseMoveEvent(QMouseEvent *event)
{
    if (mMouseEvent) {
        if (event->buttons() & Qt::LeftButton) {
            initDrapPoints();

            auto end = event->pos(); // 鼠标停下的点
            mDrapRect = QRectF(mLastPos,end); // 鼠标形成的矩形框
            auto rects = getChildRects();
            for(int row = 0; row < mrows; ++row)
                for(int col = 0; col < mcols; ++col) {
                    if(mDrapRect.intersects(rects[row][col])){ // 小矩形区域在这个推拽区域内有交集
                        mDrapPoints[row][col] = true;
                    }
                }

        } else if (event->button() == Qt::RightButton) {

        }
    }
    update();
    event->accept();
}

void Pattern::mouseReleaseEvent(QMouseEvent *event)
{
    if (mMouseEvent) {
        if (event->button() == Qt::LeftButton) {
            auto c = mSelectPoints[mMousePos.x()][mMousePos.y()].second;
            auto dlg = new GroupInfo;
            dlg->setBtnColor(c); // 鼠标单击时可以让按钮跟随当前的孔颜色
            //dlg->setAttribute(Qt::WA_DeleteOnClose);
            int ret = dlg->exec();
            if (ret == QDialog::Accepted)
            {
                select(dlg->groupColor());
            }
            delete dlg;
            mDrapRect.setWidth(0); // 清除矩形
            mDrapRect.setHeight(0);

        } else if (event->button() == Qt::RightButton) {

        }
    }
    event->accept();
}

void Pattern::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    auto pen = painter.pen();
    pen.setWidth(2);
    painter.setPen(pen);

    drawLine(painter);
    drawText(painter);
    drawHighlight(painter);

    event->accept();
}

void Pattern::drawLine(QPainter&painter)
{ // 绘制线
    auto points = getBorderPoints();
    for(int row = 1; row < mrows; ++row) // 首尾2个边的线不再绘制
        painter.drawLine(points[row][0],
                         points[row][mcols]);
    for(int col = 1; col < mcols; ++col)
        painter.drawLine(points[0][col],points[mrows][col]);
}

void Pattern::drawText(QPainter &painter)
{ // 绘制文字
    auto pointSize = mrows>10? 8:DefaultFontSize;

    auto font = painter.font();
    font.setPointSize(pointSize);
    font.setFamily(DefaultFontFamily);
    painter.setFont(font);

    auto rowpts = getRowHeaderPoints();
    for(int col = 0 ; col < mcols; ++ col)
    {
        auto center = rowpts[col];
        painter.drawText(center,QString::number(col+1));
    }

    auto colpts = getColHeaderPoints();
    for(int row = 0 ; row < mrows; ++ row)
    {
        auto center = colpts[row];
        painter.drawText(center,QChar(row+65));
    }
}

QSize Pattern::getChildSize() const
{ // 每个圆都在划分的小正方形区域内,计算正方形的长度
    int w = width() ;
    int h = height() ;
    // 总宽度-2个到边界的gap-圆之间留出的mSpace,边界sapce留出2个,共(mCol-1+2)个
    int cell_w = (w-2*mGap-mSpace*(mcols+1))/mcols;
    int cell_h = (h-2*mGap-mSpace*(mrows+1)) / mrows;
    return  QSize(cell_w,cell_h);
}

QRectF2DVector Pattern::getChildRects() const
{ // 拿到所有小正方形的区域
    auto points = getBorderPoints();

    QRectF2DVector rects;
    for(int row = 0; row < mrows; ++row)
    {
        QRectFVector r;
        for(int col = 0; col < mcols; ++col)
        {
            auto topleft = points[row][col];
            auto bottomright = points[row+1][col+1];
            r.append(QRectF(topleft,bottomright));
        }
        rects.append(r);
    }
    return rects;
}

QPointF2DVector Pattern::getBorderPoints() const
{ // 按行从左到右拿到所有分隔线上的点,可用于画线
    auto cell_size = getChildSize();
    int cell_w = cell_size.width();
    int cell_h = cell_size.height();

    QPointF2DVector points;
    for(int row = 0; row < mrows+1; ++row) // 4x6,4行5条边,所以需要mrows+1多加1边
    {
        QPointFVector p; // 拿到每行的分隔点
        for(int col = 0; col < mcols+1; ++col)
            p.append(QPointF(mGap+mSpace/2+(cell_w+mSpace)*col, // 分隔点坐标从起始位置(mgap+maspace/2)开始
                             mGap+mSpace/2+(cell_h+mSpace)*row));
        points.append(p);
    }

    return points;
}

QPointFVector Pattern::getRowHeaderPoints()
{// 获取行表头文字的点,用于绘制文字
    auto cell_size = getChildSize();
    int cell_w = cell_size.width();
    QPointFVector points;
    for(int col = 0; col < mcols; ++col)
        points.append(QPointF(mGap+mSpace/2+cell_w/2+(cell_w+mSpace)*col,
                              mGap-mSpace/2)); // 列y坐标不变就是mgap-space/2
    return points;
}

QPointFVector Pattern::getColHeaderPoints()
{// 获取列表头文字的点,用于绘制文字
    auto cell_size = getChildSize();
    int cell_h = cell_size.height();
    QPointFVector points;
    for(int row = 0; row < mrows; ++row)
        points.append(QPointF(mGap-mSpace, // 行x坐标不变就是mgap-space
                              mGap+mSpace+cell_h/2+(cell_h+mSpace)*row));
    return points;
}

QPointF2DVector Pattern::getCenterPoints() const
{// 获取小正方形区域的中心用于绘制圆用
    auto cell_size = getChildSize();
    int cell_w = cell_size.width();
    int cell_h = cell_size.height();

    QPointF2DVector centerPts;
    for(int row = 0; row < mrows; ++row)
    {
        QVector<QPointF> p;
        for(int col = 0; col < mcols; ++col)
            // 中心位置相对起点左上角多了cell_w和cell_h的一半,还多了mSpace
            p.append(QPointF(mGap+cell_w/2+mSpace+(cell_w+mSpace)*col,
                             mGap+cell_h/2+mSpace+(cell_h+mSpace)*row));
        centerPts.append(p);
    }

    return centerPts;
}

void Pattern::selectPoint(int row,int col, bool isSelected)
{ // 选中某个点
    if (row <0 || row >= mrows) return ;
    if (col <0 || col >= mcols) return ;
    mSelectPoints[row][col].first = isSelected;
    mSelectPoints[row][col].second = Qt::red;
    update();
}

void Pattern::selectPoints(QCPointVector points, bool isSelected)
{// 选中点列表的所有点
    foreach (auto p ,points) {
        selectPoint(p.x(),p.y(),isSelected);
    }
}

void Pattern::clearPoints()
{
    initSelectPoints();
}

void Pattern::init()
{
    mState = UnInitialState;
    mMouseEvent = false;
    mMousePos = {-1,-1};
    mInnerCircleColor.setAlpha(PatternColorAlpha);
    mMouseClickColor.setAlpha(PatternColorAlpha);

    initSelectPoints();
    initDrapPoints();
}

Pattern::Pattern(int rows, int cols,QWidget*parent):mrows(rows),mcols(cols),QWidget(parent)
{
    init();
}

void Pattern::setPatternSize(int rows,int cols)
{
    mrows = rows;
    mcols = cols;
    initSelectPoints();
    initDrapPoints();
    update();
}

void Pattern::toggleState(PatternState state)
{
    mState = state;
    switch (mState) {
        case UnInitialState:
            mMouseEvent = false;
            break;
        case TickState:
            mMouseEvent = true;
            break;
    }
}

void Pattern::clearMousePoint()
{
    mMousePos = QPoint(-1,-1);
    update();
}

void Pattern::initDrapPoints()
{
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

void Pattern::initSelectPoints()
{
    mSelectPoints.clear();
    for(int row = 0 ; row < mrows; ++ row) {
        QBoolColorPairVector var;
        for (int col = 0; col < mcols; ++col){
            var.append(qMakePair(false,Qt::red));
        }
        mSelectPoints.append(var);
    }
    update();
}

void Pattern::select(QCColor color)
{ // 把拖拽的点都选中
    for(int row = 0 ; row < mrows; ++ row) {
        for (int col = 0; col < mcols; ++col){
            auto pt = mDrapPoints[row][col];
            if (pt){
                mSelectPoints[row][col].first = true;
                mSelectPoints[row][col].second = color;
                mDrapPoints[row][col] = false; // 被选中的点不要再视为拖拽区域
            }
        }
    }
    auto ret = mMousePos.x()<0 || mMousePos.x()>mrows-1
               || mMousePos.y()<0 || mMousePos.y()>mcols-1;
    if (!ret) {// 防止越界,选择和取消选择需要在未初始化时禁用动作,
        mSelectPoints[mMousePos.x()][mMousePos.y()].first = true; // 没启用鼠标事件,这是{-1,-1}会越界
        mSelectPoints[mMousePos.x()][mMousePos.y()].second = color;
        mDrapPoints[mMousePos.x()][mMousePos.y()] = false;
    }
    update();
}

void Pattern::unselect()
{
//    for(int row = 0 ; row < mrows; ++ row) {
//        for (int col = 0; col < mcols; ++col){
//            auto pt = mDrapPoints[row][col];
//            if (pt){
//                mSelectPoints[row][col] = false;
//                mDrapPoints[row][col] = false;
//            }
//        }
//    }
//    auto ret = mMousePos.x()<0 || mMousePos.x()>mrows-1
//            || mMousePos.y()<0 || mMousePos.y()>mcols-1;
//    if (!ret) {// 防止越界,选择和取消选择需要在未初始化时禁用动作,
//        mSelectPoints[mMousePos.x()][mMousePos.y()] = false; // 没启用鼠标事件,这是{-1,-1}会越界
//        mDrapPoints[mMousePos.x()][mMousePos.y()] = false;
//    }
//    mMousePos = {-1,-1}; // 短暂的清除一下
//    update();
}
